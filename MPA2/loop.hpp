#ifndef MPA2_LOOP
#define MPA2_LOOP

#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include "condition.hpp"
#include "counter.hpp"
#include "iterator.hpp"
#include "operator.hpp"
#include "poly.hpp"
#include "utils.hpp"

// creating loop class
class Loop {

public:
  Loop();
  void tokenize(std::string file);
  void count();
  void printMembers();
  void printCount();

private:
  // Contents of the loop
  Iterator _iterator;                 // Object that stores the iterator
  Condition _condition;               // Object that stores the condition
  Counter _counter;                   // Object that stores the counter
  Operator _operator;                 // Object that stores the operator
  std::list<std::string> _procedures; // The list of all procedures in the loop

  Poly _polyCount; // The polynomial that stores the count

  // Tokenize the loop
  void _tokenizeParenContent(std::string &parenContent);
  void _tokenizeBracketContent(std::string &bracketContent);

  // Counts the loop
  int _countProcedures();
  int _countParametersInLoop();
  int _countParametersAfterLoop();
  int _countUpperLimit();
};

#endif
