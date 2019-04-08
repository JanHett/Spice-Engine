#include <gtest/gtest.h>
#include <iostream>
#include <array>
#include <cstdlib>
#include <type_traits>

#include "../../src/containers/pixel.h"

TEST(Pixel, ConstructorEmpty) {
	pixel<3> pxl;
	EXPECT_EQ(0.0f, pxl[0]);
	EXPECT_EQ(0.0f, pxl[1]);
	EXPECT_EQ(0.0f, pxl[2]);
}

TEST(Pixel, ConstructorInitialValues) {
	pixel<3> pxl({0.2, 0.3, 0.4});
	EXPECT_EQ(0.2f, pxl[0]);
	EXPECT_EQ(0.3f, pxl[1]);
	EXPECT_EQ(0.4f, pxl[2]);
}

TEST(Pixel, OperatorSubscriptConst) {
	pixel<3> pxl({0.5, 0.25, 0.125});
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
	pixel<3> pxl({0.5, 0.25, 0.125});
	pxl[0] = 0.42;
	pxl[1] = 0.47;
	pxl[2] = 0.75;
	EXPECT_EQ(0.42f, pxl[0]);
	EXPECT_EQ(0.47f, pxl[1]);
	EXPECT_EQ(0.75f, pxl[2]);
}

TEST(Pixel, OperatorSubscriptConstWithRGBChannelEnum) {
	pixel<4> pxl({0.5, 0.25, 0.125, 0.0625});
	const auto pxl0 = pxl[rgb_channel::red];
	static_assert(
		std::is_const<std::remove_reference<decltype(pxl0)>::type>::value,
		"Not using const version of [] in test for const []."
	);
	const auto pxl1 = pxl[rgb_channel::green];
	static_assert(
		std::is_const<std::remove_reference<decltype(pxl1)>::type>::value,
		"Not using const version of [] in test for const []."
	);
	const auto pxl2 = pxl[rgb_channel::blue];
	static_assert(
		std::is_const<std::remove_reference<decltype(pxl2)>::type>::value,
		"Not using const version of [] in test for const []."
	);
	const auto pxl3 = pxl[rgb_channel::alpha];
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
	pixel<4> pxl({0.5, 0.25, 0.125, 0.625});
	pxl[rgb_channel::red] = 0.42;
	pxl[rgb_channel::green] = 0.47;
	pxl[rgb_channel::blue] = 0.75;
	pxl[rgb_channel::alpha] = 0.92;
	EXPECT_EQ(0.42f, pxl[rgb_channel::red]);
	EXPECT_EQ(0.47f, pxl[rgb_channel::green]);
	EXPECT_EQ(0.75f, pxl[rgb_channel::blue]);
	EXPECT_EQ(0.92f, pxl[rgb_channel::alpha]);
}

TEST(Pixel, OperatorSubscriptConstWithCMYKChannelEnum) {
	pixel<5> pxl({0.5, 0.25, 0.125, 0.0625, 0.03125});
	const auto pxl0 = pxl[cmyk_channel::cyan];
	static_assert(
		std::is_const<std::remove_reference<decltype(pxl0)>::type>::value,
		"Not using const version of [] in test for const []."
	);
	const auto pxl1 = pxl[cmyk_channel::magenta];
	static_assert(
		std::is_const<std::remove_reference<decltype(pxl1)>::type>::value,
		"Not using const version of [] in test for const []."
	);
	const auto pxl2 = pxl[cmyk_channel::yellow];
	static_assert(
		std::is_const<std::remove_reference<decltype(pxl2)>::type>::value,
		"Not using const version of [] in test for const []."
	);
	const auto pxl3 = pxl[cmyk_channel::black];
	static_assert(
		std::is_const<std::remove_reference<decltype(pxl2)>::type>::value,
		"Not using const version of [] in test for const []."
	);
	const auto pxl4 = pxl[cmyk_channel::alpha];
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
	pixel<5> pxl({0.5, 0.25, 0.125, 0.625, 0.03125});
	pxl[cmyk_channel::cyan] = 0.42;
	pxl[cmyk_channel::magenta] = 0.47;
	pxl[cmyk_channel::yellow] = 0.75;
	pxl[cmyk_channel::black] = 0.92;
	pxl[cmyk_channel::alpha] = 0.94;
	EXPECT_EQ(0.42f, pxl[cmyk_channel::cyan]);
	EXPECT_EQ(0.47f, pxl[cmyk_channel::magenta]);
	EXPECT_EQ(0.75f, pxl[cmyk_channel::yellow]);
	EXPECT_EQ(0.92f, pxl[cmyk_channel::black]);
	EXPECT_EQ(0.94f, pxl[cmyk_channel::alpha]);
}

TEST(Pixel, OperatorPlusEquals) {
	pixel<3> pxl1({0.5, 0.5, 0.5});
	pixel<3> pxl2({0.5, 0.25, 0.125});

	pxl1 += pxl2;
	EXPECT_EQ(1, pxl1[0]);
	EXPECT_EQ(0.75, pxl1[1]);
	EXPECT_EQ(0.625, pxl1[2]);

	EXPECT_EQ(0.5, pxl2[0]);
	EXPECT_EQ(0.25, pxl2[1]);
	EXPECT_EQ(0.125, pxl2[2]);
}

TEST(Pixel, OperatorPlus) {
	pixel<3> pxl1({0.5, 0.5, 0.5});
	pixel<3> pxl2({0.5, 0.25, 0.125});

	pixel pxl_result = pxl1 + pxl2;
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
	pixel<3> pxl1({0.5, 0.5, 0.5});
	pixel<3> pxl2({0.5, 0.25, 0.125});

	pxl1 -= pxl2;
	EXPECT_EQ(0, pxl1[0]);
	EXPECT_EQ(0.25, pxl1[1]);
	EXPECT_EQ(0.375, pxl1[2]);

	EXPECT_EQ(0.5, pxl2[0]);
	EXPECT_EQ(0.25, pxl2[1]);
	EXPECT_EQ(0.125, pxl2[2]);
}

TEST(Pixel, OperatorMinus) {
	pixel<3> pxl1({0.5, 0.5, 0.5});
	pixel<3> pxl2({0.5, 0.25, 0.125});

	pixel pxl_result = pxl1 - pxl2;
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
	pixel<3> pxl1({0.5, 0.5, 0.5});
	pixel<3> pxl2({0.5, 0.25, 0.125});

	pxl1 *= pxl2;
	EXPECT_EQ(0.25, pxl1[0]);
	EXPECT_EQ(0.125, pxl1[1]);
	EXPECT_EQ(0.0625, pxl1[2]);

	EXPECT_EQ(0.5, pxl2[0]);
	EXPECT_EQ(0.25, pxl2[1]);
	EXPECT_EQ(0.125, pxl2[2]);
}

TEST(Pixel, OperatorMultiply) {
	pixel<3> pxl1({0.5, 0.5, 0.5});
	pixel<3> pxl2({0.5, 0.25, 0.125});

	pixel pxl_result = pxl1 * pxl2;
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
	pixel<3> pxl1({0.5, 0.5, 0.5});
	pixel<3> pxl2({0.5, 0.25, 0.125});

	pxl1 /= pxl2;
	EXPECT_EQ(1, pxl1[0]);
	EXPECT_EQ(2, pxl1[1]);
	EXPECT_EQ(4, pxl1[2]);

	EXPECT_EQ(0.5, pxl2[0]);
	EXPECT_EQ(0.25, pxl2[1]);
	EXPECT_EQ(0.125, pxl2[2]);

	pxl2 /= 0.5;
	EXPECT_EQ(1, pxl2[0]);
	EXPECT_EQ(0.5, pxl2[1]);
	EXPECT_EQ(0.25, pxl2[2]);
}

TEST(Pixel, OperatorDivide) {
	pixel<3> pxl1({0.5, 0.5, 0.5});
	pixel<3> pxl2({0.5, 0.25, 0.125});

	pixel pxl_result = pxl1 / pxl2;
	EXPECT_EQ(1, pxl_result[0]);
	EXPECT_EQ(2, pxl_result[1]);
	EXPECT_EQ(4, pxl_result[2]);

	EXPECT_EQ(0.5, pxl1[0]);
	EXPECT_EQ(0.5, pxl1[1]);
	EXPECT_EQ(0.5, pxl1[2]);

	EXPECT_EQ(0.5, pxl2[0]);
	EXPECT_EQ(0.25, pxl2[1]);
	EXPECT_EQ(0.125, pxl2[2]);

	pixel pxl_result_div_float = pxl2 / 0.5;
	EXPECT_EQ(1, pxl_result_div_float[0]);
	EXPECT_EQ(0.5, pxl_result_div_float[1]);
	EXPECT_EQ(0.25, pxl_result_div_float[2]);

	EXPECT_EQ(0.5, pxl2[0]);
	EXPECT_EQ(0.25, pxl2[1]);
	EXPECT_EQ(0.125, pxl2[2]);
}

TEST(Pixel, OperatorEquals) {
	pixel<3> pxl1({0.5, 0.5, 0.5});
	pixel<3> pxl2({0.5, 0.25, 0.125});
	pixel<3> pxl3({0.5, 0.25, 0.125});
	pixel<3> pxl4({0.5, 0.25, 0.125});

	EXPECT_TRUE(pxl2 == pxl3);
	EXPECT_TRUE(pxl2 == pxl4);
	EXPECT_TRUE(pxl3 == pxl4);
	EXPECT_TRUE(pxl2 == pxl2);

	EXPECT_FALSE(pxl1 == pxl2);
	EXPECT_FALSE(pxl2 == pxl1);
}

TEST(Pixel, OperatorNotEquals) {
	pixel<3> pxl1({0.5, 0.5, 0.5});
	pixel<3> pxl2({0.5, 0.25, 0.125});
	pixel<3> pxl3({0.5, 0.25, 0.125});
	pixel<3> pxl4({0.5, 0.25, 0.125});

	EXPECT_FALSE(pxl2 != pxl3);
	EXPECT_FALSE(pxl2 != pxl4);
	EXPECT_FALSE(pxl3 != pxl4);
	EXPECT_FALSE(pxl2 != pxl2);

	EXPECT_TRUE(pxl1 != pxl2);
	EXPECT_TRUE(pxl2 != pxl1);
}
