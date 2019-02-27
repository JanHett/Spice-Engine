#include <gtest/gtest.h>

#include <fstream>

#include "../../src/file_formats/pbm.h"

TEST(PPM, ConstructorFromPixelMatrix) {
    matrix<pixel<4>> m(4, 2);

    for (pixel<4> & pxl: m.data) {
        pxl[0] = 0.42;
        pxl[1] = 0.42;
        pxl[2] = 0.42;
        pxl[3] = 0.42;
    }

    ppm<4> img(m);

    EXPECT_EQ(4, img.width);
    EXPECT_EQ(2, img.height);
    EXPECT_EQ(256, img.maxval);
    for (size_t i = 0; i < img.width * img.height * 3; ++i) {
        // 107 is 0.42 * 255 rounded to an int
        EXPECT_EQ(107, img.data[i]);
    }
}

TEST(PPM, ConstructorFromFile) {
    try {
        ppm img("../tests/_data/checker_3x3.pbm");

        EXPECT_EQ('P', img.magic_number[0]);
        EXPECT_EQ('6', img.magic_number[1]);
        ASSERT_EQ(3, img.width);
        ASSERT_EQ(3, img.height);
        EXPECT_EQ(255, img.maxval);
        for (unsigned int i = 0; i < img.width * img.height * 3; ++i) {
            EXPECT_EQ((i / 3) % 2 == 0 ? 0 : 255, img.data[i]);
        }
    } catch (char const * err) {
        fprintf(stderr, "%s\n", err);
    }
}

TEST(PPM, from_pixel_matrix) {
    matrix<pixel<4>> m(4, 2);

    for (pixel<4> & pxl: m.data) {
        pxl[0] = 0.42;
        pxl[1] = 0.42;
        pxl[2] = 0.42;
        pxl[3] = 0.42;
    }

    auto img = ppm<4>::from_pixel_matrix(m);

    EXPECT_EQ(4, img.width);
    EXPECT_EQ(2, img.height);
    EXPECT_EQ(256, img.maxval);
    for (size_t i = 0; i < img.width * img.height * 3; ++i) {
        // 107 is 0.42 * 255 rounded to an int
        EXPECT_EQ(107, img.data[i]);
    }
}

TEST(PPM, to_pixel_matrix) {
    const matrix<pixel<4>> m(4, 2);

    ppm<4> img(m);

    auto result = img.to_pixel_matrix();

    EXPECT_EQ(m.get_width(), result.get_width());
    EXPECT_EQ(m.get_height(), result.get_height());
    for (unsigned int i = 0; i < m.data.size(); ++i) {
        for (unsigned int channel = 0; channel < 3; ++channel) {
            EXPECT_EQ(m.data[i][channel], result.data[i][channel]);
        }
    }
}

TEST(PPM, StreamInsertion) {

}
