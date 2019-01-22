#include <gtest/gtest.h>
#include <iostream>
#include <array>
#include <cstdlib>

#include "../../src/containers/pixel.h"

TEST(Pixel, OperatorSubscriptConst) {
	Pixel<3> pxl({0.5, 0.25, 0.125});
	EXPECT_EQ(0.5, pxl[0]);
	EXPECT_EQ(0.25, pxl[1]);
	EXPECT_EQ(0.125, pxl[2]);
}

TEST(Pixel, OperatorSubscript) {
	Pixel<3> pxl({0.5, 0.25, 0.125});
	pxl[0] = 0.42;
	pxl[1] = 0.47;
	pxl[2] = 0.75;
	EXPECT_EQ(0.42f, pxl[0]);
	EXPECT_EQ(0.47f, pxl[1]);
	EXPECT_EQ(0.75f, pxl[2]);
}

TEST(Pixel, OperatorPlusEquals) {
	Pixel<3> pxl1({0.5, 0.5, 0.5});
	Pixel<3> pxl2({0.5, 0.25, 0.125});

	pxl1 += pxl2;
	EXPECT_EQ(1, pxl1[0]);
	EXPECT_EQ(0.75, pxl1[1]);
	EXPECT_EQ(0.625, pxl1[2]);

	EXPECT_EQ(0.5, pxl2[0]);
	EXPECT_EQ(0.25, pxl2[1]);
	EXPECT_EQ(0.125, pxl2[2]);
}

TEST(Pixel, OperatorPlus) {
	Pixel<3> pxl1({0.5, 0.5, 0.5});
	Pixel<3> pxl2({0.5, 0.25, 0.125});

	Pixel pxl_result = pxl1 + pxl2;
	EXPECT_EQ(1, pxl_result[0]);
	EXPECT_EQ(0.75, pxl_result[1]);
	EXPECT_EQ(0.625, pxl_result[2]);

	EXPECT_EQ(0.5, pxl1[0]);
	EXPECT_EQ(0.5, pxl1[1]);
	EXPECT_EQ(0.5, pxl1[2]);

	EXPECT_EQ(0.5, pxl2[0]);
	EXPECT_EQ(0.25, pxl2[1]);
	EXPECT_EQ(0.125, pxl2[2]);
}
