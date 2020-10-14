#include <core/probability_discerner.h>
#include <map>
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




} // namespace naivebayes