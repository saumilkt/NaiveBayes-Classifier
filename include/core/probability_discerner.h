#ifndef SRC_PROBABILITY_DISCERNER_H
#define SRC_PROBABILITY_DISCERNER_H


#include <string>
#include <map>
#include <tuple>

namespace naivebayes {


typedef std::pair<int, int> Coordinate;

// Ints representing the index of each color in tuple
const size_t kWhiteIndex = 0;
const size_t kGrayIndex = 1;
const size_t kBlackIndex = 2;

// Image width in pixels
const size_t kImageSize = 28;

// Range of numbers that training data represents
const size_t kFirstDigit = 0;
const size_t kLastDigit = 9;

// Laplace smoothing value
const size_t kClassifyConst = 1;

// Default file locations
const std::string kDefTrainDataPath = "../data.trainingimages";
const std::string kDefTrainLabelPath = "../data/traininglabels";
const std::string kDefModelWriteReadPath = "../data/datamodel.txt";

class probability_discerner {
 public:

};


} // namespace naivebayes

#endif //SRC_PROBABILITY_DISCERNER_H