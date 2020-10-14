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

}

