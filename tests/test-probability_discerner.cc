#include <catch2/catch2.h>
#include <core/model_analyze.h>

#include "core/model_analyze.h"

TEST_CASE("Import Data Tests"){
  probability_discerner digit;
  digit.ImportData("data/trainingimages");

  SECTION("Number of Images Tests"){
    REQUIRE(digit.num_training_images_ == 5000);
    int num_elements = 0;
    for (int num = 0; num <= 9; num++) {
      num_elements +=
          std::get<0>(digit.data_set_[num][std::make_pair(0,0)]) +
          std::get<1>(digit.data_set_[num][std::make_pair(0,0)]) +
          std::get<2>(digit.data_set_[num][std::make_pair(0,0)]);
    }
    REQUIRE(num_elements == 5000);

    //Since all images have 00 pixel as white.
    int num_white_pix_in00 = 0;
    for (int num = 0; num <= 9; num++) {
      num_white_pix_in00 +=
          std::get<0>(digit.data_set_[num][std::make_pair(0,0)]);

    }
    REQUIRE(num_white_pix_in00==5000);
  }
  SECTION("Probabilities Valid Tests"){
    for (int num = 0; num <= 9; num++) {
      for(int i=0;i<28;i++){
        for(int j=0;j<28;j++){
          double prob_white =
              std::get<0>(digit.probability_set_[num][std::make_pair(j,i)]);
          double prob_gray =
              std::get<1>(digit.probability_set_[num][std::make_pair(j,i)]);
          double prob_gray =
              std::get<2>(digit.probability_set_[num][std::make_pair(j,i)]);
          bool isValidWhite = prob_white <= 1 && prob_white >= 0;
          bool isValidBlackGray = prob_gray <= 1 && prob_gray >= 0;
          bool isValidBlackGray = prob_black <= 1 && prob_black >= 0;
          REQUIRE(isValidWhite && isValidGray && isValidBlack);
        }
      }
    }
  }
}

TEST_CASE("File Writing/ Reading Tests"){
  probability_discerner digit;
  digit.ImportData("data/trainingimages");

  SECTION("Check If File Writing/Reading is consistent") {
    digit.WriteModelToFile("data/datamodel.txt");
    int num_elem = digit.num_training_images_;
    std::map<int, map<Coordinate, std::tuple<int, int, int>>> data =
        digit.data_set_;
    map<int, map<Coordinate, std::tuple<double, double, double>>> prob =
        digit.probability_set_;

    probability_discerner new_digit;
    new_digit.ImportModelFromFile("data/datamodel.txt");
    REQUIRE(new_digit.num_training_images_ == num_elem);
    REQUIRE(new_digit.data_set_ == data);
    REQUIRE(new_digit.probability_set_ == prob);
  }

  SECTION("Check If reading/writing is successful"){

    REQUIRE(digit.WriteModelToFile("../data/datamodel.txt"));
    REQUIRE(digit.ImportModelFromFile("../data/datamodel.txt"));

  }

}
