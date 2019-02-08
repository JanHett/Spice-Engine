#include <gtest/gtest.h>

#include "../../src/file_formats/pbm.h"

TEST(PPM, from_pixel_matrix) {
    const matrix<pixel<4>> m(4, 2);

    auto img = ppm::from_pixel_matrix<4>(m);
    EXPECT_EQ(4, img.width);
    EXPECT_EQ(2, img.height);
    EXPECT_EQ(256, img.maxval);
    for (size_t i = 0; i < img.width * img.height * 3; ++i) {
        EXPECT_EQ(0, img.data[i]);
    }
}

TEST(PPM, to_pixel_matrix) {

}

TEST(PPM, StreamInsertion) {

}

TEST(PPM, StreamExtraction) {

}
