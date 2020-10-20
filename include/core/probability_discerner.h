#ifndef SRC_PROBABILITY_DISCERNER_H
#define SRC_PROBABILITY_DISCERNER_H

#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "model_io.h"

namespace naivebayes {
using std::map;
using std::tuple;

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
  map<int, map<Coordinate, tuple<int, int, int>>> data_set_;
  map<int, map<Coordinate, tuple<double, double, double>>> probability_set_;

  // formatting for models
  const char pixel_separator_ = ' ';
  const char coord_separator_ = ',';

  // Constructor, initializes datasets
  probability_discerner();


  // Stream operator overloads
  // Overloads the input operator >> to input a Color enum instead of a char
  friend std::istream& operator>>(std::istream& is, char color);
  //Overloads the output operator << to help with
  friend std::ostream& operator<<(std::ostream& os,
                                  map<Coordinate, tuple<int,int,int>> digit);

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

 private:
  int image_size_;
  // method for handling the CLI interactions of the program
  void CommandLineInterface();

  // method for processing User input from command line
  void ProcessInput(const int &user_choice);

  // Initializes data_set_.
  // Sets every value inside pairs to 0 by default.
  void InitializeDataSet();

  // Initializes probability_set_.
  // Sets every value inside pairs to 0 by default.
  void InitializeProbabilitySet();

  // Calculates the probability of each pixel of each digit using data_set_.
  // Calculates the probability of each pixel and each class and stores them.
  void CalculateProbabilities();

  // Converts the part of the data_set_ associated with a digit to a string.
  // Takes an int representing a digit as the only parameter.
  // Returns an std::string that represents data_set_[digit].
  // Used to write a digit's requisite information to a new model
  std::string GetDigitString(const int &digit);

  // Splits a string into different strings by the given char.
  // Basically is the .split() method in Java strings.
  // Takes a string and where we want to split it as the parameters.
  // Returns a vector containing the string split by the given char.
  // Used when reading from a pre-existing model
  std::vector<std::string> SplitString(const std::string &string,
                                       const char &split_point);
};


} // namespace naivebayes

#endif //SRC_PROBABILITY_DISCERNER_H