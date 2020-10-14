#ifndef SRC_PROBABILITY_DISCERNER_H
#define SRC_PROBABILITY_DISCERNER_H


#include <string>
#include <map>
#include <tuple>

namespace naivebayes {
using std::map;

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
  int num_training_images_;

  // Maps representing class probabilites, raw pixel data, and pixel
  // probabilities
  map<int, double> class_probabilities_;
  map<int, map<Coordinate, std::tuple<int, int, int>>> data_set_;
  map<int, map<Coordinate,
      std::tuple<double, double, double>>> probability_set_;

  // formatting for models
  const char pixel_separator_ = ' ';
  const char coord_separator_ = ',';

  // Constructor, initializes datasets
  probability_discerner();


  // These methods are set to public for testing.

  // Converts data from files to a model for naive bayes algorithm.
  // Takes 2 std::strings that represent the label and data files path.
  // Values in data_set_ are updated according to the data and labels.
  void ImportData(const std::string &data_path,
                  const std::string &label_path);

  // Writes the existing data_set_ and number of examples to a file.
  // Takes an std::string that represents the file path as the only argument.
  // Returns a boolean that represents if the file writing was succesfull.
  bool WriteModelToFile(const std::string &file_path);

  // Uses the same file format as WriteModelToFile to import a data_set_.
  // Takes an std::string that represents the file path as the only argument.
  // Returns a boolean that represents if the file reading was succesfull.
  bool ImportModelFromFile(const std::string &file_path);


};


} // namespace naivebayes

#endif //SRC_PROBABILITY_DISCERNER_H