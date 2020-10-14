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

void probability_discerner::InitializeDataSet() {
  data_set_ = map<int, map<Coordinate, tuple<int, int, int>>>();
  // for each digit, initialize each coordinate to zero tuple
  for (int digit = kFirstDigit; digit <= kLastDigit; digit++) {
    map<Coordinate, tuple<int, int, int>> numRepeatance;
    for (int row = 0; row < kImageSize; row++) {
      for (int col = 0; col < kImageSize; col++) {
        // Adds a pair with two zeroes for each key in the map.
        // First value in each pair represents number of white pixels.
        // Second value in the pair is the number of grey or dark pixels
        numRepeatance[std::make_pair(col, row)] = std::make_tuple(0, 0, 0);
      }
    }

    data_set_[digit] = numRepeatance;
  }
}

void probability_discerner::InitializeProbabilitySet() {
  class_probabilities_ = map<int, double>();
  probability_set_ = map<int, map<Coordinate, tuple<double, double, double>>>();
  // for each digit, intitialize each image coordinate to 0 tuple
  for (int digit = kFirstDigit; digit <= kLastDigit; digit++) {
    map<Coordinate, tuple<double, double, double>> numRepeatance;
    for (int row = 0; row < kImageSize; row++) {
      for (int col = 0; col < kImageSize; col++) {
        // Adds a pair with two zeroes for each key in the map.
        numRepeatance[std::make_pair(col, row)] =
            std::make_tuple(0, 0, 0);
      }
    }
    probability_set_[digit] = numRepeatance;
  }

}



} // namespace naivebayes