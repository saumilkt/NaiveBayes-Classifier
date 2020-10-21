#ifndef SRC_MODEL_ANALYZE_H
#define SRC_MODEL_ANALYZE_H

#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "model_classify.h"
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

// formatting for models
const char kPixelSeparator = ' ';
const char kCoordSeparator = ',';

class Model {
 public:
  int num_training_images_;
  // Maps representing class probabilites, raw pixel data, and pixel
  // probabilities
  map<int, double> class_probabilities_;
  map<int, map<Coordinate, tuple<int, int, int>>> data_set_;
  map<int, map<Coordinate, tuple<double, double, double>>> probability_set_;


  // Constructor, initializes datasets
  Model();


  // Stream operator overloads
  // Overloads the input operator >> to input a Color enum instead of a char
  friend std::istream& operator>>(std::istream& is, char color);
  //Overloads the output operator << to help with
  friend std::ostream &operator<<(std::ostream& os,
                                   const std::tuple<int,int,int>& dataValue);

  /**
   * FILE IO METHODS
   * */

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

  /**
   * MAIN CLASSIFICATION FUNCTION
   */

  // Classifies the given images and prints out the correctnes percentage
  // Takes 2 std::strings that represent the label and data files path.
  // Returns a confusion matrix as described in the assignment sheet.
  void ClassifyImages(const std::string &file_path,
                                 const std::string &label_path);

 private:
  int image_size_;

  /**
   * DATA ANALYSIS METHODS
   * */

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

  // Splits a string into different strings by the given char.
  // Basically is the .split() method in Java strings.
  // Takes a string and where we want to split it as the parameters.
  // Returns a vector containing the string split by the given char.
  // Used when reading from a pre-existing model
  std::vector<std::string> SplitString(const std::string &string,
                                       const char &split_point);

  /**
   * DATA CLASSIFICATION METHODS
   */

  // Returns the most likely digit for an image.
  // Takes an std::map that represents an image as the only parameter.
  // Returns a value of int that represents what the image most likely is.
  int GetMostLikelyDigit(std::map<Coordinate,int> &image_set);

  // Checks the probability of a given image being the given digit.
  // Takes an std::map that represents an image and a digit as parameters.
  // Returns a double value that represents the probability of the image
  // being  the  given digit.
  double GetDigitProbability(const int &digit,
                             std::map<Coordinate,int> &image_set);

  // Returns the log value of the probability at given parameters.
  // Takes the digit, color of the pixel and the coordinates as the parameter.
  // Gets the value specified by the parameters from prob_set and returns the
  // log of that value.
  double GetPixelProbability(const int &digit, const int &color,
                             const Coordinate &coord);

};


} // namespace naivebayes

#endif //SRC_PROBABILITY_DISCERNER_H