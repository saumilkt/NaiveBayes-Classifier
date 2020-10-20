//
// Created by Saumil Thakore on 10/20/20.
//

#ifndef SRC_MODEL_IO_H
#define SRC_MODEL_IO_H

#include <vector>
namespace naivebayes {
using std::vector;

struct Image {
  vector<vector<char>> pixelArray;
};

const char kWhiteChar = ' ';
const char kGrayChar = '+';
const char kBlackChar = '#';

enum Color : char {
  kWhitePixel = kWhiteChar,
  kGrayPixel = kGrayChar,
  kBlackPixel = kWhiteChar,
};

/*
 * No new methods are declared here, the source file is implementing file
 * read/write methods declared in probability_discerner.h
 */
} //namespace naivebayes
#endif  // NAIVE_BAYES_MODEL_IO_H
