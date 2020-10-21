#include <core/probability_discerner.h>
#include "core/model_io.h"
#include <fstream>
#include <map>
#include <sstream>
#include <tuple>
#include <iostream>


namespace naivebayes {
using std::tuple;
using std::map;

probability_discerner::probability_discerner() {
  num_training_images_ = 0;
  image_size_=0;
  InitializeProbabilitySet();
  InitializeDataSet();
}

void probability_discerner::InitializeDataSet() {
  data_set_ = map<int, map<Coordinate, tuple<int, int, int>>>();
  // for each digit, initialize each coordinate to zero tuple
  for (int digit = kFirstDigit; digit <= kLastDigit; digit++) {
    map<Coordinate, tuple<int, int, int>> numRepeatance;
    for (int row = 0; row < image_size_; row++) {
      for (int col = 0; col < image_size_; col++) {
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
    for (int row = 0; row < image_size_; row++) {
      for (int col = 0; col < image_size_; col++) {
        // Adds a pair with two zeroes for each key in the map.
        numRepeatance[std::make_pair(col, row)] =
            std::make_tuple(0, 0, 0);
      }
    }
    probability_set_[digit] = numRepeatance;
  }

}

void probability_discerner::CalculateProbabilities() {
  for (int digit = kFirstDigit; digit <= kLastDigit; digit++) {
    // Addition of elements of the tuple in any coordinate
    // with same digit will always be the same and reflect the number of
    // examples that digit has.
    int num_digit_repeat =
        std::get<kBlackIndex>(data_set_[digit][std::make_pair(0, 0)]) +
        std::get<kWhiteIndex>(data_set_[digit][std::make_pair(0, 0)]) +
        std::get<kGrayIndex>(data_set_[digit][std::make_pair(0, 0)]) ;

    // Probability of each digit is calculated and added to a class var.
    double prob_digit = (kClassifyConst + num_digit_repeat) /
                        (num_training_images_ + 2 * kClassifyConst);
    class_probabilities_[digit] = prob_digit;

    for (int row = 0; row < image_size_; row++) {
      for (int col = 0; col < image_size_; col++) {
        Coordinate coord = std::make_pair(col, row);
        int num_white = std::get<kWhiteIndex>(data_set_[digit][coord]);
        int num_gray = std::get<kGrayIndex>(data_set_[digit][coord]);
        int num_black = std::get<kBlackIndex>(data_set_[digit][coord]);
        int num_repeatence = num_white + num_gray + num_black;

        // Formula provided in the assignment sheet is used to calculate
        // the probability of each coordinate in each digit for white
        // and black/gray pixels are calculated and stored in a class
        // variable.
        double prob_white = (num_white + kClassifyConst) /
                            (num_repeatence + 2 * kClassifyConst);
        double prob_gray = (num_gray + kClassifyConst) /
                           (num_repeatence + 2 * kClassifyConst);
        double prob_black = (num_black + kClassifyConst) /
                            (num_repeatence + 2 * kClassifyConst);
        probability_set_[digit][coord] = std::make_tuple(
            log(prob_white),
            log(prob_gray),
            log(prob_black));
      }
    }
  }
}

std::vector<std::string> probability_discerner::SplitString(
    const std::string &string,
    const char &split_point) {
  std::vector<std::string> strings;
  std::stringstream ss(string);
  std::string part;
  while (getline(ss, part, split_point)) {
    strings.push_back(part);
  }
  return strings;
}
} // namespace naivebayes