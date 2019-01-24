#include <gtest/gtest.h>
#include <iostream>
#include <array>
#include <cstdlib>
#include <type_traits>

#include "../../src/containers/pixel.h"

TEST(Pixel, OperatorSubscriptConst) {
	Pixel<3> pxl({0.5, 0.25, 0.125});
	const auto pxl0 = pxl[0];
	static_assert(
		std::is_const<std::remove_reference<decltype(pxl0)>::type>::value,
		"Not using const version of [] in test for const []."
	);
	const auto pxl1 = pxl[1];
	static_assert(
		std::is_const<std::remove_reference<decltype(pxl1)>::type>::value,
		"Not using const version of [] in test for const []."
	);
	const auto pxl2 = pxl[2];
	static_assert(
		std::is_const<std::remove_reference<decltype(pxl2)>::type>::value,
		"Not using const version of [] in test for const []."
	);
	EXPECT_EQ(0.5, pxl0);
	EXPECT_EQ(0.25, pxl1);
	EXPECT_EQ(0.125, pxl2);
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

TEST(Pixel, OperatorSubscriptConstWithRGBChannelEnum) {
	Pixel<4> pxl({0.5, 0.25, 0.125, 0.0625});
	const auto pxl0 = pxl[RGBChannel::red];
	static_assert(
		std::is_const<std::remove_reference<decltype(pxl0)>::type>::value,
		"Not using const version of [] in test for const []."
	);
	const auto pxl1 = pxl[RGBChannel::green];
	static_assert(
		std::is_const<std::remove_reference<decltype(pxl1)>::type>::value,
		"Not using const version of [] in test for const []."
	);
	const auto pxl2 = pxl[RGBChannel::blue];
	static_assert(
		std::is_const<std::remove_reference<decltype(pxl2)>::type>::value,
		"Not using const version of [] in test for const []."
	);
	const auto pxl3 = pxl[RGBChannel::alpha];
	static_assert(
		std::is_const<std::remove_reference<decltype(pxl2)>::type>::value,
		"Not using const version of [] in test for const []."
	);
	EXPECT_EQ(0.5, pxl0);
	EXPECT_EQ(0.25, pxl1);
	EXPECT_EQ(0.125, pxl2);
	EXPECT_EQ(0.0625, pxl3);
}

TEST(Pixel, OperatorSubscriptWithRGBChannelEnum) {
	Pixel<4> pxl({0.5, 0.25, 0.125, 0.625});
	pxl[RGBChannel::red] = 0.42;
	pxl[RGBChannel::green] = 0.47;
	pxl[RGBChannel::blue] = 0.75;
	pxl[RGBChannel::alpha] = 0.92;
	EXPECT_EQ(0.42f, pxl[RGBChannel::red]);
	EXPECT_EQ(0.47f, pxl[RGBChannel::green]);
	EXPECT_EQ(0.75f, pxl[RGBChannel::blue]);
	EXPECT_EQ(0.92f, pxl[RGBChannel::alpha]);
}

TEST(Pixel, OperatorSubscriptConstWithCMYKChannelEnum) {
	Pixel<5> pxl({0.5, 0.25, 0.125, 0.0625, 0.03125});
	const auto pxl0 = pxl[CMYKChannel::cyan];
	static_assert(
		std::is_const<std::remove_reference<decltype(pxl0)>::type>::value,
		"Not using const version of [] in test for const []."
	);
	const auto pxl1 = pxl[CMYKChannel::magenta];
	static_assert(
		std::is_const<std::remove_reference<decltype(pxl1)>::type>::value,
		"Not using const version of [] in test for const []."
	);
	const auto pxl2 = pxl[CMYKChannel::yellow];
	static_assert(
		std::is_const<std::remove_reference<decltype(pxl2)>::type>::value,
		"Not using const version of [] in test for const []."
	);
	const auto pxl3 = pxl[CMYKChannel::black];
	static_assert(
		std::is_const<std::remove_reference<decltype(pxl2)>::type>::value,
		"Not using const version of [] in test for const []."
	);
	const auto pxl4 = pxl[CMYKChannel::alpha];
	static_assert(
		std::is_const<std::remove_reference<decltype(pxl2)>::type>::value,
		"Not using const version of [] in test for const []."
	);
	EXPECT_EQ(0.5, pxl0);
	EXPECT_EQ(0.25, pxl1);
	EXPECT_EQ(0.125, pxl2);
	EXPECT_EQ(0.0625, pxl3);
	EXPECT_EQ(0.03125, pxl4);
}

TEST(Pixel, OperatorSubscriptWithCMYKChannelEnum) {
	Pixel<5> pxl({0.5, 0.25, 0.125, 0.625, 0.03125});
	pxl[CMYKChannel::cyan] = 0.42;
	pxl[CMYKChannel::magenta] = 0.47;
	pxl[CMYKChannel::yellow] = 0.75;
	pxl[CMYKChannel::black] = 0.92;
	pxl[CMYKChannel::alpha] = 0.94;
	EXPECT_EQ(0.42f, pxl[CMYKChannel::cyan]);
	EXPECT_EQ(0.47f, pxl[CMYKChannel::magenta]);
	EXPECT_EQ(0.75f, pxl[CMYKChannel::yellow]);
	EXPECT_EQ(0.92f, pxl[CMYKChannel::black]);
	EXPECT_EQ(0.94f, pxl[CMYKChannel::alpha]);
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

TEST(Pixel, OperatorMinusEquals) {
	Pixel<3> pxl1({0.5, 0.5, 0.5});
	Pixel<3> pxl2({0.5, 0.25, 0.125});

	pxl1 -= pxl2;
	EXPECT_EQ(0, pxl1[0]);
	EXPECT_EQ(0.25, pxl1[1]);
	EXPECT_EQ(0.375, pxl1[2]);

	EXPECT_EQ(0.5, pxl2[0]);
	EXPECT_EQ(0.25, pxl2[1]);
	EXPECT_EQ(0.125, pxl2[2]);
}

TEST(Pixel, OperatorMinus) {
	Pixel<3> pxl1({0.5, 0.5, 0.5});
	Pixel<3> pxl2({0.5, 0.25, 0.125});

	Pixel pxl_result = pxl1 - pxl2;
	EXPECT_EQ(0, pxl_result[0]);
	EXPECT_EQ(0.25, pxl_result[1]);
	EXPECT_EQ(0.375, pxl_result[2]);

	EXPECT_EQ(0.5, pxl1[0]);
	EXPECT_EQ(0.5, pxl1[1]);
	EXPECT_EQ(0.5, pxl1[2]);

	EXPECT_EQ(0.5, pxl2[0]);
	EXPECT_EQ(0.25, pxl2[1]);
	EXPECT_EQ(0.125, pxl2[2]);
}

TEST(Pixel, OperatorMultiplyEquals) {
	Pixel<3> pxl1({0.5, 0.5, 0.5});
	Pixel<3> pxl2({0.5, 0.25, 0.125});

	pxl1 *= pxl2;
	EXPECT_EQ(0.25, pxl1[0]);
	EXPECT_EQ(0.125, pxl1[1]);
	EXPECT_EQ(0.0625, pxl1[2]);

	EXPECT_EQ(0.5, pxl2[0]);
	EXPECT_EQ(0.25, pxl2[1]);
	EXPECT_EQ(0.125, pxl2[2]);
}

TEST(Pixel, OperatorMultiply) {
	Pixel<3> pxl1({0.5, 0.5, 0.5});
	Pixel<3> pxl2({0.5, 0.25, 0.125});

	Pixel pxl_result = pxl1 * pxl2;
	EXPECT_EQ(0.25, pxl_result[0]);
	EXPECT_EQ(0.125, pxl_result[1]);
	EXPECT_EQ(0.0625, pxl_result[2]);

	EXPECT_EQ(0.5, pxl1[0]);
	EXPECT_EQ(0.5, pxl1[1]);
	EXPECT_EQ(0.5, pxl1[2]);

	EXPECT_EQ(0.5, pxl2[0]);
	EXPECT_EQ(0.25, pxl2[1]);
	EXPECT_EQ(0.125, pxl2[2]);
}

TEST(Pixel, OperatorDivideEquals) {
	Pixel<3> pxl1({0.5, 0.5, 0.5});
	Pixel<3> pxl2({0.5, 0.25, 0.125});

	pxl1 /= pxl2;
	EXPECT_EQ(1, pxl1[0]);
	EXPECT_EQ(2, pxl1[1]);
	EXPECT_EQ(4, pxl1[2]);

	EXPECT_EQ(0.5, pxl2[0]);
	EXPECT_EQ(0.25, pxl2[1]);
	EXPECT_EQ(0.125, pxl2[2]);
}

TEST(Pixel, OperatorDivide) {
	Pixel<3> pxl1({0.5, 0.5, 0.5});
	Pixel<3> pxl2({0.5, 0.25, 0.125});

	Pixel pxl_result = pxl1 / pxl2;
	EXPECT_EQ(1, pxl_result[0]);
	EXPECT_EQ(2, pxl_result[1]);
	EXPECT_EQ(4, pxl_result[2]);

	EXPECT_EQ(0.5, pxl1[0]);
	EXPECT_EQ(0.5, pxl1[1]);
	EXPECT_EQ(0.5, pxl1[2]);

	EXPECT_EQ(0.5, pxl2[0]);
	EXPECT_EQ(0.25, pxl2[1]);
	EXPECT_EQ(0.125, pxl2[2]);
}
