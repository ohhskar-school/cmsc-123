#include "simulator.hpp"

// Constructor
Simulator::Simulator() { t = new FileTree(); }

// Deconstructor
Simulator::~Simulator() {
  delete t;
  t = nullptr;
}

void Simulator::executeFromFile() {
  // Open file
  std::ifstream infile("mp4.in");
  std::ofstream outfile("mp4.out", std::ofstream::out | std::ofstream::trunc);

  // Create pointer to commands
  Command *c = nullptr;

  std::string inputString;
  while (std::getline(infile, inputString)) {
    // If no input was passed, move to the next iteration
    if (inputString.empty()) {
      continue;
    }

    // Tokenize input to get command + params
    std::list<std::string> tokens = tokenize(inputString, ' ');

    // Get the command
    inputString = tokens.front();

    // Remove command so that the remaining tokens are params
    tokens.pop_front();

    // Commands
    if (inputString == "exit") {
      std::cout << "Bye!" << std::endl;
      break;
    } else if (inputString == "ls") {
      c = new ls(tokens);
    } else if (inputString == "mkdir") {
      c = new mkdir(tokens);
    } else if (inputString == ">") {
      c = new touch(tokens);
    } else if (inputString == ">>" || inputString == "edit") {
      c = new append(tokens);
    } else if (inputString == "cd") {
      c = new cd(tokens);
    } else if (inputString == "rn") {
      c = new rn(tokens);
    } else if (inputString == "show") {
      c = new show(tokens);
    } else if (inputString == "rm") {
      c = new rm(tokens);
    } else if (inputString == "rmdir") {
      c = new rmdir(tokens);
    } else if (inputString == "cp") {
      c = new cp(tokens);
    } else if (inputString == "mv") {
      c = new mv(tokens);
    } else if (inputString == "whereis") {
      c = new whereis(tokens);
    }

    // If c exists, execute it then deletes it
    if (c) {
      !c->execute(*(t), outfile);
      delete c;
      c = nullptr;
    }
  }

  // Final Cleanup
  if (c) {
    delete c;
    c = nullptr;
  }
}
void Simulator::execute() {
  // Create pointer to commands
  Command *c = nullptr;

  while (true) {
    // Print current path
    std::cout << t->nodePath(t->current()) << " > ";

    // Holder string and get input with spaces
    std::string inputString;
    std::getline(std::cin, inputString);

    // If no input was passed, move to the next iteration
    if (inputString.empty()) {
      continue;
    }

    // Tokenize input to get command + params
    std::list<std::string> tokens = tokenize(inputString, ' ');

    // Get the command
    inputString = tokens.front();

    // Remove command so that the remaining tokens are params
    tokens.pop_front();

    // Commands
    if (inputString == "exit") {
      std::cout << "Bye!" << std::endl;
      break;
    } else if (inputString == "ls") {
      c = new ls(tokens);
    } else if (inputString == "mkdir") {
      c = new mkdir(tokens);
    } else if (inputString == ">") {
      c = new touch(tokens);
    } else if (inputString == ">>" || inputString == "edit") {
      c = new append(tokens);
    } else if (inputString == "cd") {
      c = new cd(tokens);
    } else if (inputString == "rn") {
      c = new rn(tokens);
    } else if (inputString == "show") {
      c = new show(tokens);
    } else if (inputString == "rm") {
      c = new rm(tokens);
    } else if (inputString == "rmdir") {
      c = new rmdir(tokens);
    } else if (inputString == "cp") {
      c = new cp(tokens);
    } else if (inputString == "mv") {
      c = new mv(tokens);
    } else if (inputString == "whereis") {
      c = new whereis(tokens);
    }

    // If c exists, execute it then deletes it
    if (c) {
      c->execute(*(t));
      delete c;
      c = nullptr;
    }
  }

  // Final Cleanup
  if (c) {
    delete c;
    c = nullptr;
  }
}
