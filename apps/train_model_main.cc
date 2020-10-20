#include <core/probability_discerner.h>
#include <stdexcept>
#include <iostream>
const size_t kNumberOfArgs =4;

int main(int argc, char* argv[]) {
  naivebayes::probability_discerner discerner;
  std::string argument = argv[1];
  if (argument==("train")) {
   discerner.ImportData(argv[2] ,argv[3])
  } else if (argument.compare("read")) {
    discerner.ImportModelFromFile(argv[2]);
  } else if (argument.compare("write")) {
    discerner.WriteModelToFile(argv[2]);
  } else {
    throw std::invalid_argument("The command you inputted is not found");
  }
  return 0;
}
