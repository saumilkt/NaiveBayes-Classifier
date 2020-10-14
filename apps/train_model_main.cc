#include <core/probability_discerner.h>

int main() {
  naivebayes::probability_discerner().ImportData("data/trainingimages",
                                                 "data/traininglabels");
  naivebayes::probability_discerner().WriteModelToFile("data.model.txt");

  return 0;
}
