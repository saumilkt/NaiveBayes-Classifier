#include <core/probability_discerner.h>
#include <catch2/catch2.h>
#include "core/probability_discerner.h"

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


