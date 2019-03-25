#include <gtest/gtest.h>

#include <memory>

#include "../../src/graph/node_io.h"

// --------- //
//   Input   //
// --------- //

TEST(Input, ConstructWithLabel) {
    input<int> i("Constructed");

    EXPECT_EQ("Constructed", i.label);
}

TEST(Input, Connect) {
    auto i = std::make_shared<input<int>>();
    auto o = std::make_shared<output<int>>();

    i->connect(*o);

    // input's source address should equal output's address
    EXPECT_EQ(o.get(), i->source);
    // output should have exactly one consumer
    EXPECT_EQ(1, o->consumers.size());
    // input's address should be referred to by output's first entry into consumers vector
    EXPECT_EQ(i.get(), o->consumers[0]);
    // both input and output should reference the same address with their data property
    EXPECT_EQ(o->data.lock(), i->data.lock());
}

TEST(Input, Disconnect) {
    auto i = std::make_shared<input<int>>();
    auto o = std::make_shared<output<int>>();
    i->connect(*o);

    i->disconnect();

    // input's source address should be nullptr
    EXPECT_EQ(nullptr, i->source);
    // output should now have zero consumers
    EXPECT_EQ(0, o->consumers.size());
    // both input and output should reference the same address with their data property
    EXPECT_EQ(nullptr, i->data.lock());
}

// ---------- //
//   Output   //
// ---------- //

TEST(Output, ConstructWithLabel) {
    output<int> o("Constructed");

    EXPECT_EQ("Constructed", o.label);
}

TEST(Output, Connect) {
    auto i = std::make_shared<input<int>>();
    auto o = std::make_shared<output<int>>();

    o->connect(*i);

    // input's source address should equal output's address
    EXPECT_EQ(o.get(), i->source);
    // output should have exactly one consumer
    EXPECT_EQ(1, o->consumers.size());
    // input's address should be referred to by output's first entry into consumers vector
    EXPECT_EQ(i.get(), o->consumers[0]);
    // both input and output should reference the same address with their data property
    EXPECT_EQ(o->data.lock(), i->data.lock());
}

TEST(Output, Disconnect) {
    auto i = std::make_shared<input<int>>();
    auto o = std::make_shared<output<int>>();
    o->connect(*i);

    o->disconnect(*i);

    // input's source address should be nullptr
    EXPECT_EQ(nullptr, i->source);
    // output should now have zero consumers
    EXPECT_EQ(0, o->consumers.size());
    // both input and output should reference the same address with their data property
    EXPECT_EQ(nullptr, i->data.lock());
}
