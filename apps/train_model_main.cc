#include <core/model_analyze.h>

#include <stdexcept>

int main(int argc, char* argv[]) {
  naivebayes::Model discerner;
  std::string argument = argv[1];
  if (argument==("train")) {
   discerner.ImportData(argv[2] ,argv[3]);
  } else if (argument.compare("read")) {
    discerner.ImportModelFromFile(argv[2]);
  } else if (argument.compare("write")) {
    discerner.WriteModelToFile(argv[2]);
  } else {
    throw std::invalid_argument("The command you inputted is not found");
  }
  return 0;
}
