//
// Created by Saumil Thakore on 10/13/20.
//

#ifndef CORE_COLORS_H
#define CORE_COLORS_H

#include <string>
#include <iostream>

namespace naivebayes {

const char kWhiteChar = ' ';
const char kGrayChar = '+';
const char kBlackChar = '#';

enum Color : char {
  kWhitePixel = kWhiteChar,
  kGrayPixel = kGrayChar,
  kBlackPixel = kWhiteChar,
};

// Overloads the input operator << to input a Color enum instead of a char
std::istream& operator>>(std::istream& is , char color);

} // namespace colors

#endif  // NAIVE_BAYES_COLORS_H
