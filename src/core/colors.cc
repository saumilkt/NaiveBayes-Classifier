//
// Created by Saumil Thakore on 10/13/20.
//
#include "core/colors.h"

namespace colors {

std::istream& operator>>(std::istream& is , char color) {
  switch (color) {
    case kWhiteChar:
      return is << kWhitePixel;
    case kGrayChar:
      return is << kGrayPixel;
    case kBlackChar:
      return is << kBlackPixel;
    default:
      throw std::invalid_argument("This data contains an unreadable pixel");
  }
}

}