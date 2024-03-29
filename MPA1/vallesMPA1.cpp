#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
void includeMe(string, ofstream &);
int main() {

  // Creating output file
  ofstream output("outputFile.out", ios::binary);

  // Creating input file
  string initialName = "inputFile.cpp";
  includeMe(initialName, output);

  output.close();

  return 0;
}

void includeMe(string fileName, ofstream &output) {

  // Creating initial input stream
  ifstream input(fileName);

  // Working on file if it is open
  if (input.is_open()) {

    // Creating buffer string for each line
    string lines;

    // Iterating through the lines
    while (getline(input, lines)) {

      // Checking to see if #include is in the current line
      if (lines.find("#include") != string::npos) {

        // With help from
        // https://stackoverflow.com/questions/289347/using-strtok-with-a-stdstring
        // for tokenizing strings
        istringstream tempStream(lines);
        string token;

        int counter = 0;
        while (std::getline(tempStream, token, '"')) {
          // Skips the first tokenization as it is the #include" string
          if (counter == 1) {
            includeMe(token, output);
            break;
          } else {
            counter++;
          }
        }
      }

      // If #include is not found in the statement, the contents of the file is
      // written to the output
      else {
        output << lines << endl;
      }
    }
  }

  input.close();
}
