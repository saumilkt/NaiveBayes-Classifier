//
// Created by Saumil Thakore on 10/20/20.
//

#include "core/model_classify.h"
#include "core/model_analyze.h"

namespace naivebayes {
double Model::GetDigitProbability(const int &digit,
                                  std::map<Coordinate,int> &image_set) {

  double digit_prob = log(class_probabilities_[digit]);

  for (int row = 0; row < image_size_; row++) {
    for (int col = 0; col < image_size_; col++) {
      // log of probability of each coordinate is added together.
      Coordinate coord = std::make_pair(col, row);
      digit_prob += GetPixelProbability(digit, image_set[coord], coord);
    }
  }

  return digit_prob;
}

double Model::GetPixelProbability(const int &digit, const int &color,
                                            const Coordinate &coord) {
  if(color == kWhitePixel){
    // If the color of the pixel is white then the corresponding value is
    // the first value in the tuple with corresponding digit and coordinate.
    return std::get<kWhiteIndex>(probability_set_[digit][coord]);
  }

  // Otherwise the color of the pixel is black or gray, so the corresponding
  // value is the second +thrid in the tuple.
  return std::get<kGrayIndex>(probability_set_[digit][coord]) +
      std::get<kBlackIndex>(probability_set_[digit][coord]);
}


} // namespace naivebayes