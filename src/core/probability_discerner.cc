#include <core/probability_discerner.h>

#include <fstream>
#include <map>
#include <sstream>
#include <tuple>

#include "core/colors.h"

namespace naivebayes {
using std::tuple;
using std::map;

probability_discerner::probability_discerner() {
  num_training_images_ = 0;
  InitializeProbabilitySet();
  InitializeDataSet();
}

void probability_discerner::InitializeDataSet() {
  data_set_ = map<int, map<Coordinate, tuple<int, int, int>>>();
  // for each digit, initialize each coordinate to zero tuple
  for (int digit = kFirstDigit; digit <= kLastDigit; digit++) {
    map<Coordinate, tuple<int, int, int>> numRepeatance;
    for (int row = 0; row < kImageSize; row++) {
      for (int col = 0; col < kImageSize; col++) {
        // Adds a pair with two zeroes for each key in the map.
        // First value in each pair represents number of white pixels.
        // Second value in the pair is the number of grey or dark pixels
        numRepeatance[std::make_pair(col, row)] = std::make_tuple(0, 0, 0);
      }
    }

    data_set_[digit] = numRepeatance;
  }
}

void probability_discerner::InitializeProbabilitySet() {
  class_probabilities_ = map<int, double>();
  probability_set_ = map<int, map<Coordinate, tuple<double, double, double>>>();
  // for each digit, intitialize each image coordinate to 0 tuple
  for (int digit = kFirstDigit; digit <= kLastDigit; digit++) {
    map<Coordinate, tuple<double, double, double>> numRepeatance;
    for (int row = 0; row < kImageSize; row++) {
      for (int col = 0; col < kImageSize; col++) {
        // Adds a pair with two zeroes for each key in the map.
        numRepeatance[std::make_pair(col, row)] =
            std::make_tuple(0, 0, 0);
      }
    }
    probability_set_[digit] = numRepeatance;
  }

}

void probability_discerner::CommandLineInterface(){
  while (true) {
    std::cout<< "Please Enter 0 to read training data to create a model, "
                "1 to write an existing model to a file, 2 to read an existing "
                "model from a file, or 3 to end" << std::endl;
    int user_choice;
    try {
      std::string input;
      getline(std::cin, input);
      user_choice = std::stoi(input);
      if(user_choice == 3) break;
    } catch (std::invalid_argument& e) {
      std::cout << "Please give a valid input" << std::endl;
      continue;
    }

    ProcessInput(user_choice);
  }
}

void probability_discerner::ProcessInput(const int &user_choice){
  switch (user_choice) {
    case 0:{
      std::cout << "Please provide a file path for data" << std::endl;
      std::string data_path;
      getline(std::cin, data_path);
      if (data_path == ""){
        //Default values are used when value entered is empty.
        ImportData(kDefTrainDataPath,kDefTrainLabelPath);
      } else {
        std::cout << "Please provide a file path"
                     " for training labels" << std::endl;
        std::string label_path;
        getline(std::cin, label_path);
        ImportData(data_path, label_path);
      }
      return;

    } case 1:{
      std::cout << "Please provide a file path to write a model " << std::endl;
      std::string file_path;
      getline(std::cin, file_path);
      if(file_path == ""){
        //Default value is used when value entered is empty.
        WriteModelToFile(kDefModelWriteReadPath);
      } else {
        WriteModelToFile(file_path);
      }
      return;

    } case 2:{
      std::cout << "Please provide a path to read a model from " << std::endl;
      std::string file_path;
      getline(std::cin, file_path);
      if(file_path == ""){
        //Default value is used when value entered is empty.
        ImportModelFromFile(kDefModelWriteReadPath);
      } else {
        ImportModelFromFile(file_path);
      }
      return;
    } default:
      std::cout << "Please enter a valid input"<< std::endl;
      return;
  }
}

void probability_discerner::ImportData(const std::string &data_path,
                                       const std::string &label_path) {
  std::ifstream train_file(data_path);
  std::string line;
  std::ifstream label_file(label_path);
  std::string label;

  while (std::getline(label_file, label)) {

    if (!(label_file >> label)) {
      break;
    }
    // Loop ends when labels are finished.
    // There are 28 lines in data file for each line in label file.
    int label_ind = stoi(label);
    // num_train_exmp increases by 1 with each loop.
    num_training_images_++;
    for (int row = 0; row < kImageSize; row++) {
      std::getline(train_file, line);
      for (int col = 0; col < kImageSize; col++) {
        Coordinate coord = std::make_pair(col, row);
        if (line.at(col) == Color::kGrayPixel) {
          // The number of dark pixels at given coordinates is
          // increased by one.
          std::get<kGrayIndex>(data_set_[label_ind][coord])++;
        } else if (line.at(col) == Color::kBlackPixel) {
          std::get<kBlackIndex>(data_set_[label_ind][coord])++;
        } else {
          // The number of white pixels at given coordinates is
          // increased by one.
          std::get<kWhiteIndex>(data_set_[label_ind][coord])++;
        }
      }
    }

  }

  CalculateProbabilities();
}

void probability_discerner::CalculateProbabilities() {
  for (int digit = kFirstDigit; digit <= kLastDigit; digit++) {
    // Addition of elements of the tuple in any coordinate
    // with same digit will always be the same and reflect the number of
    // examples that digit has.
    int num_digit_repeat =
        std::get<kBlackIndex>(data_set_[digit][std::make_pair(0, 0)]) +
        std::get<kWhiteIndex>(data_set_[digit][std::make_pair(0, 0)]) +
        std::get<kGrayIndex>(data_set_[digit][std::make_pair(0, 0)]) ;

    // Probability of each digit is calculated and added to a class var.
    double prob_digit = (kClassifyConst + num_digit_repeat) /
                        (num_training_images_ + 2 * kClassifyConst);
    class_probabilities_[digit] = prob_digit;

    for (int row = 0; row < kImageSize; row++) {
      for (int col = 0; col < kImageSize; col++) {
        Coordinate coord = std::make_pair(col, row);
        int num_white = std::get<kWhiteIndex>(data_set_[digit][coord]);
        int num_gray = std::get<kGrayIndex>(data_set_[digit][coord]);
        int num_black = std::get<kBlackIndex>(data_set_[digit][coord]);
        int num_repeatence = num_white + num_gray + num_black;

        // Formula provided in the assignment sheet is used to calculate
        // the probability of each coordinate in each digit for white
        // and black/gray pixels are calculated and stored in a class
        // variable.
        double prob_white = (num_white + kClassifyConst) /
                            (num_repeatence + 2 * kClassifyConst);
        double prob_gray = (num_gray + kClassifyConst) /
                           (num_repeatence + 2 * kClassifyConst);
        double prob_black = (num_black + kClassifyConst) /
                            (num_repeatence + 2 * kClassifyConst);
        probability_set_[digit][coord] = std::make_tuple(
            log(prob_white),
            log(prob_gray),
            log(prob_black));
      }
    }
  }
}

bool probability_discerner::WriteModelToFile(const std::string &file_path) {
  //If the data_set is empty or there are 0 examples then a false is returned.
  if (data_set_.empty() || num_training_images_ == 0) {
    return false;
  }

  std::ofstream output_file;
  output_file.open(file_path);

  // First line in the file is number of training examples.
  output_file << num_training_images_ << std::endl;

  for (int digit = kFirstDigit; digit <= kLastDigit; digit++) {
    // Each line after the first represents each coordinate's corresponding
    // values in the given digit.
    output_file << GetDigitString(digit) << std::endl;

  }
  output_file.close();
  return true;
}

bool probability_discerner::ImportModelFromFile(const std::string &file_path) {
  // data_Set and prob_set are reset first.
  InitializeProbabilitySet();
  InitializeDataSet();

  std::ifstream input_file(file_path);
  std::string line;
  // Same formatting as file writing is used for reading, and split method
  // is used to achieve that.
  std::getline(input_file, line);
  num_training_images_ = std::stoi(line);
  for (int digit = kFirstDigit; digit <= kLastDigit; digit++) {
    std::getline(input_file, line);
    std::vector<std::string> pairs = SplitString(line, coord_separator_);

    for (int row = 0; row < kImageSize; row++) {
      for (int col = 0; col < kImageSize; col++) {
        std::string pixel_data = pairs[col + kImageSize * row];
        std::vector<std::string> bwg_freq =
            SplitString(pixel_data, pixel_separator_);
        int num_white = std::stoi(bwg_freq[0]);
        int num_gray = std::stoi(bwg_freq[1]);
        int num_black = std::stoi(bwg_freq[2]);
        Coordinate coord = std::make_pair(col, row);

        for (int i = 0; i < num_white; i++) {
          std::get<kWhiteIndex>(data_set_[digit][coord])++;
        }

        for (int i = 0; i < num_gray; i++) {
          std::get<kGrayIndex>(data_set_[digit][coord])++;
        }

        for (int i = 0; i < num_black; i++) {
          std::get<kBlackIndex>(data_set_[digit][coord])++;
        }
      }
    }
  }

  // After the new data_set is implemented, probabilities are re-calculated.
  CalculateProbabilities();
  return true;
}

std::string probability_discerner::GetDigitString(const int &digit) {
  std::string digit_string;
  // Each coordinates pair values have a blank space between them and each
  // tuple has a comma between them.
  for (int row = 0; row < kImageSize; row++) {
    for (int col = 0; col < kImageSize; col++) {
      Coordinate coord = std::make_pair(col, row);
      int num_white = std::get<kWhiteIndex>
          (data_set_[digit][coord]);
      int num_gray = std::get<kGrayIndex>
          (data_set_[digit][coord]);
      int num_black = std::get<kBlackIndex>
          (data_set_[digit][coord]);
      digit_string += std::to_string(num_white) + pixel_separator_ +
                      std::to_string(num_gray) + pixel_separator_ +
                      std::to_string(num_black) + coord_separator_;
    }
  }

  // Last element in the string is removed because it's an extra.
  digit_string.pop_back();
  return digit_string;
}

std::vector<std::string> probability_discerner::SplitString(
    const std::string &string,
    const char &split_point) {
  std::vector<std::string> strings;
  std::stringstream ss(string);
  std::string part;
  while (getline(ss, part, split_point)) {
    strings.push_back(part);
  }
  return strings;
}

} // namespace naivebayes