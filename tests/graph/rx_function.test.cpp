#include <gtest/gtest.h>

#include "../../src/graph/rx_function.hpp"

TEST(tuple_map, constructor) {
    tuple_map<const char *, int, float, std::string> empty_tm;

    static_assert(std::is_same<
        key_not_found_t,
        std::remove_reference<decltype(
            std::get<0>(empty_tm.data()).second)>::type>::value,
        "First element's value of internal tuple is not of type key_not_found_t");
    EXPECT_FALSE(std::get<0>(empty_tm.data()).first);

    EXPECT_EQ(
        (std::pair {
                    std::optional<const char *>{},
                    std::optional<int>{}
                }),
        std::get<1>(empty_tm.data()));
    EXPECT_EQ(
        (std::pair {
                    std::optional<const char *>{},
                    std::optional<float>{}
                }),
        std::get<2>(empty_tm.data()));
    EXPECT_EQ(
        (std::pair {
                    std::optional<const char *>{},
                    std::optional<std::string>{}
                }),
        std::get<3>(empty_tm.data()));

    tuple_map<const char *, int, float, std::string> initialised_tm(
        {"The answer", 42,},
        {"The exact answer", 42.47,},
        {"The honest answer", "The cake is a lie"}
        );

    static_assert(std::is_same<
        key_not_found_t,
        std::remove_reference<decltype(
            std::get<0>(initialised_tm.data()).second)>::type>::value,
        "First element's value of internal tuple is not of type key_not_found_t");
    EXPECT_FALSE(std::get<0>(initialised_tm.data()).first);

    EXPECT_EQ(
        (std::pair {
                    std::optional<const char *>{"The answer"},
                    std::optional<int>{42}
                }),
        std::get<1>(initialised_tm.data()));
    EXPECT_EQ(
        (std::pair {
                    std::optional<const char *>{"The exact answer"},
                    std::optional<float>{42.47}
                }),
        std::get<2>(initialised_tm.data()));
    EXPECT_EQ(
        (std::pair {
                    std::optional<const char *>{"The honest answer"},
                    std::optional<std::string>{"The cake is a lie"}
                }),
        std::get<3>(initialised_tm.data()));
}

TEST(tuple_map, operatorSubscript) {
    tuple_map<std::string, int, float, std::string> tm(
        {"The answer", 42,},
        {"The exact answer", 42.47,},
        {"The honest answer", "The cake is a lie"}
        );

    static_assert(tm["The answer"] == 42, "The universe is broken");
    static_assert(tm["The exact answer"] == 42.47, "The universe is broken");
    static_assert(tm["The honest answer"] == "The cake is a lie", "The universe is broken");

    EXPECT_EQ(42, tm["The answer"]);
    EXPECT_EQ(42.47, tm["The exact answer"]);
    EXPECT_EQ("The cake is a lie", tm["The honest answer"]);
}

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
