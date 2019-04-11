#include <gtest/gtest.h>

#include "../../src/graph/rx_function.hpp"

TEST(rx_function, constructor) {
    // enum arg_enum { arg };
    // auto fn = [](int arg){ return arg; };
    // using simple_rx = rx_function<decltype(fn), arg_enum>;

    // simple_rx sx;
}

TEST(rx_function, operatorStreamExtraction) {
    // enum arg_enum { to_repeat };
    // auto fn = [](int to_repeat){ return to_repeat; };
    // using int_repeater = rx_function<decltype(fn), arg_enum>;

    // int_repeater rep;
    // int some_val = 0;

    // rep >> some_val;

    // rep.arg(int_repeater::args::to_repeat, 42);
    // EXPECT_EQ(42, some_val);

    // rep.arg(int_repeater::args::to_repeat, 47);
    // EXPECT_EQ(47, some_val);

    // some_val = 13;
    // EXPECT_EQ(13, some_val);

    // rep.arg(int_repeater::args::to_repeat, 75);
    // EXPECT_EQ(75, some_val);
}
