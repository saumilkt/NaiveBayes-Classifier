//
// Created by Saumil Thakore on 10/20/20.
//

#include "core/model_classify.h"

#include <fstream>
#include <iostream>

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
  // value is the second +third in the tuple.
  return std::get<kGrayIndex>(probability_set_[digit][coord]) +
      std::get<kBlackIndex>(probability_set_[digit][coord]);
}

int Model::GetMostLikelyDigit(std::map<Coordinate ,int> &image_set){
  double highest_prob = GetDigitProbability(kFirstDigit, image_set);
  int digit_highest_prob = kFirstDigit;
  for (int digit = kFirstDigit + 1; digit <= kLastDigit; digit++) {
    // The probability of each digit is determined and then the highest one
    // is stored using a variable outside the loop.
    double digit_prob = GetDigitProbability(digit ,image_set);

    if (digit_prob > highest_prob) {
      highest_prob = digit_prob;
      digit_highest_prob = digit;
    }
  }

  // Digit with the highest probability is returned.
  return digit_highest_prob;
}

double Model::ClassifyImages(const std::string &file_path,
                                                const std::string &label_path) {
  std::ifstream train_file(file_path);
  std::string line;
  std::ifstream label_file(label_path);
  std::string label;

  // These two values are used to calculate the number of correct guesses.
  int correct_digits = 0;
  int num_test_img = 0;

  while(!label_file.eof()) {
    num_test_img++;
    int label_ind = stoi(label);
    std::map<Coordinate ,int> img;
    for (int row = 0; row < kImageSize; row++) {
      std::getline(train_file,line);
      for (int col = 0; col < kImageSize; col++) {
        Coordinate coord = std::make_pair(col, row);
        if (line.at(col) == kGrayPixel ||
           line.at(col) == kBlackPixel){
          img[coord] = '#';
        } else{
          img[coord] = ' ';
        }
      }
    }
    // Most likely digit is found is used to add values to confusion matrix
    // and to calculate correctness percentage.
    int most_likely_digit = GetMostLikelyDigit(img);
    if (most_likely_digit == label_ind) {
      correct_digits++;
    }
  }

  // A percentage of how correct the algorithm is printed to inform the user.
  double percentage = 100 * correct_digits/num_test_img;
  std::cout << percentage <<"% accurate" <<std::endl;
  return percentage;
}
} // namespace naivebayes