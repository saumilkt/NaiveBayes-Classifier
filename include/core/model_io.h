//
// Created by Saumil Thakore on 10/20/20.
//

#ifndef SRC_MODEL_IO_H
#define SRC_MODEL_IO_H

namespace naivebayes {

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
 * read/write methods declared in model_analyze.h
 */
} //namespace naivebayes
#endif  // NAIVE_BAYES_MODEL_IO_H
