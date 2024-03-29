#include "packet.hpp"

// All packets are created by calling it with a string as its parameter
Packet::Packet(const std::string &inputString) : _rawPacket(inputString) {
  _tokenize();
  _computeChecksum();
}

bool Packet::_tokenize() {

  // Initial field size is 32 as the source address is 32 bits
  int fieldSize = 32;

  // Creates a copy of the string to save the original packet for checksum
  // algorithm
  std::string packetCopy = _rawPacket;

  /* +--------------------------+ *
   * | i | Definition           | *
   * |---+----------------------| *
   * | 0 | Source Address       | *
   * | 1 | Destination Address  | *
   * | 2 | Sequence Number      | *
   * | 3 | Checksum             | *
   * | 4 | Length of Data       | *
   * | 5 | Data                 | *
   * +--------------------------+ */
  for (int i = 0; i < 6; i++) {

    // Creates the current string to be tokenized from the copy
    std::string currString = packetCopy.substr(0, fieldSize);

    // Deletes currString from packetCopy for the next iteration
    packetCopy.erase(0, fieldSize);

    // Refer to table above for description about the cases
    switch (i) {
    case 0:
    case 1:
      _tokenizeAddress(i == 0 ? true : false, currString);
      break;

    case 2:
      _tokenizeSequenceNumber(currString);
      break;

    case 3:
      _tokenizeChecksum(currString);
      break;

    case 4:
      _tokenizeLengthOfData(currString);
      break;

    case 5:
      _tokenizeData(currString);
      break;
    }

    // If Destination address is done, field size is 16 as the following fields
    // have a field size of 16
    if (i == 1) {
      fieldSize = 16;
    }

    // Except if the next field is the data. _lengthOfData is obtained before
    // reaching here
    else if (i == 4) {
      fieldSize = _lengthOfData;
    }
  }

  return true;
}

bool Packet::_tokenizeAddress(const bool &isSource, std::string &inputString) {

  // Creates a reference to either the source address or the destination address
  std::string &addressRef = isSource ? _sourceAddress : _destinationAddress;

  for (int i = 0; i < 4; i++) {

    // std::stoll converts a string to an unsigned long long. It accepts the
    // string, a pointer to the next position after converting, which I do not
    // use, and the base to convert from. This is then converted to a a string
    addressRef.append(
        std::to_string(std::stoll(inputString.substr(0, 8), nullptr, 2)));

    // Deletes part of the input string that was already tokenized
    inputString.erase(0, 8);

    // Appends a . after the number, unless it is the last number
    if (i != 3) {
      addressRef.append(".");
    }
  }

  return true;
}

bool Packet::_tokenizeSequenceNumber(std::string &inputString) {
  _sequenceNumber = std::stoi(_twosComplement(inputString), nullptr, 2);
  return true;
}

std::string Packet::_twosComplement(std::string &inputString) {
  if (inputString[0] == '0') {
    return inputString;
  } else {
    auto tempString = inputString;
    int lastOne = tempString.length() - 1;
    for (; lastOne >= 0; lastOne--) {
      if (tempString[lastOne] == '1') {
        break;
      }
    }

    for (int i = 0; i < lastOne; i++) {
      if (tempString[i] == '0') {
        tempString[i] = '1';
      } else {
        tempString[i] = '0';
      }
    }

    return "-" + tempString;
  }
}

bool Packet::_tokenizeChecksum(std::string &inputString) {
  // std::bitset converts a string to a bitset directly in its constructor
  std::bitset<17> checksum(inputString);
  _checksum = checksum;
  return true;
}

bool Packet::_tokenizeLengthOfData(std::string &inputString) {
  _lengthOfData = std::stoll(inputString, nullptr, 2);
  return true;
}

bool Packet::_tokenizeData(std::string &inputString) {
  while (inputString.length() > 0) {
    // bit magic where a an unsigned long long is pushed back into a string
    // object. This ull is typecast into a char which accurately represents the
    // data
    _data.push_back(std::stoll(inputString.substr(0, 8), nullptr, 2));
    inputString = inputString.erase(0, 8);
  }
  return true;
}

void Packet::_computeChecksum() {
  // Creating a copy to save the original packet
  std::string checksumData = _rawPacket;

  // Convert the string to a bitset
  int checksumDataLength = checksumData.length(), bitsLength = 0;
  std::list<std::bitset<17>> bits;
  while (bitsLength < checksumDataLength) {
    std::bitset<17> checksumPart(checksumData.substr(0, 16));
    if (bitsLength == 80) {
      checksumPart.reset();
    }
    checksumData = checksumData.erase(0, 16);
    bits.push_back(checksumPart);
    bitsLength += 16;
  }

  std::bitset<17> checksum;
  unsigned long long holder = 0;

  // a ull to be used to check if a carry was done
  unsigned long long checker = std::bitset<17>("10000000000000000").to_ullong();

  for (auto &bit : bits) {

    // holder is an ull that handles the addition of the current calculation of
    // the checksum and the current 2bytes that are being added
    holder = checksum.to_ullong() + bit.to_ullong();

    // checks if a carry was done through a bitwise and
    if (holder & checker) {

      // if carry was done, remove the carry and add 1
      holder -= checker;
      holder += 1;
    }

    // Convert ull back to bitset and set checksum to be the new computed
    // checksum
    std::bitset<17> newChecksum(holder);
    checksum = newChecksum;
  }

  // get the one's complement and set the most significant bit to 0 as the most
  // siginificant bit, the 17th bit is not part of the checksum, it is there for
  // carry purposes and should not affect the final computed checksum
  checksum.flip();
  checksum[16] = 0;

  // Saves the value if the computed checksum is the same as the checksum in the
  // packet
  _isChecksumPassing = checksum == _checksum;
}

// Getters
std::string Packet::sourceAddress() const { return _sourceAddress; }

std::string Packet::destinationAddress() const { return _destinationAddress; }

int Packet::sequenceNumber() const { return _sequenceNumber; }

std::string Packet::data() const {
  if (!_isChecksumPassing) {
    return "[line corrupted]";
  } else {
    return _data;
  }
}
