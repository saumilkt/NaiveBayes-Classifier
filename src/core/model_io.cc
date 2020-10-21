//
// Created by Saumil Thakore on 10/20/20.
//

#include "core/model_io.h"
#include <core/probability_discerner.h>
#include <tuple>
#include <fstream>

namespace naivebayes {
std::istream& operator>>(std::istream& is , char color) {
  switch (color) {
    case kWhiteChar:
      return is >> kWhitePixel;
    case kGrayChar:
      return is >> kGrayPixel;
    case kBlackChar:
      return is >> kBlackPixel;
    default:
      throw std::invalid_argument("This data contains an unreadable pixel");
  }
}

std::ofstream& operator<<(std::ofstream& os,
                          const std::tuple<int,int,int>& dataValue) {
  os << std::get<kBlackIndex>(dataValue);
  os << kPixelSeparator;
  os << std::get<kWhiteIndex>(dataValue);
  os << kPixelSeparator;
  os << std::get<kGrayIndex>(dataValue);
  os << kCoordSeparator;
  return os;
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
    for (int row = 0; row < image_size_; row++) {
      std::getline(train_file, line);
      for (int col = 0; col < image_size_; col++) {
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
    std::vector<std::string> pairs = SplitString(line, kCoordSeparator);

    for (int row = 0; row < image_size_; row++) {
      for (int col = 0; col < image_size_; col++) {
        std::string pixel_data = pairs[col + image_size_ * row];
        std::vector<std::string> bwg_freq =
            SplitString(pixel_data, kPixelSeparator);
        int num_white = std::stoi(bwg_freq[0]);
        int num_gray = std::stoi(bwg_freq[1]);
        //int num_black = std::stoi(bwg_freq[2]);
        Coordinate coord = std::make_pair(col, row);

        std::get<kWhiteIndex>(data_set_[digit][coord])=num_white;
        std::get<kGrayIndex>(data_set_[digit][coord])=num_gray;
        //std::get<kBlackIndex>(data_set_[digit][coord])=num_gray;
      }
    }
  }

  // After the new data_set is implemented, probabilities are re-calculated.
  CalculateProbabilities();
  return true;
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
    for (int row = 0; row < image_size_; row++) {
      for (int col = 0; col < image_size_; col++) {
        Coordinate coord = std::make_pair(col, row);
        std::tuple<int,int,int> dataValue =data_set_[digit][coord];
        output_file << dataValue;
      }
    }
  }
  output_file.close();
  return true;
}

}