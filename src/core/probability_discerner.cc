#include <core/probability_discerner.h>
#include <map>
#include <tuple>
#include "core/colors.h"

namespace naivebayes {
using std::tuple;
using std::map;

probability_discerner::probability_discerner() {
  num_training_images_ = 0;
  InitializeProbabilitySet();
  InitializeDataSet();
}



} // namespace naivebayes