#include <gtest/gtest.h>

#include <memory>

#include "../../src/graph/node_io.h"

// --------- //
//   Input   //
// --------- //

TEST(Input, ConstructWithLabel) {
    Input<int> i("Constructed");

    EXPECT_EQ("Constructed", i.label);
}

TEST(Input, Connect) {
    auto i = std::make_shared<Input<int>>(Input<int>());
    auto o = std::make_shared<Output<int>>(Output<int>());

    i->connect(o);

    // input's source address should equal output's address
    EXPECT_EQ(o.get(), i->source.lock().get());
    // output should have exactly one consumer
    EXPECT_EQ(1, o->consumers.size());
    // input's address should be referred to by output's first entry into consumers vector
    EXPECT_EQ(i.get(), o->consumers[0].lock().get());
    // both input and output should reference the same address with their data property
    EXPECT_EQ(o->data.get(), i->data.lock().get());
}

TEST(Input, Disconnect) {
    auto i = std::make_shared<Input<int>>(Input<int>());
    auto o = std::make_shared<Output<int>>(Output<int>());
    i->connect(o);

    i->disconnect();

    // input's source address should be nullptr
    EXPECT_EQ(nullptr, i->source.lock());
    // output should now have zero consumers
    EXPECT_EQ(0, o->consumers.size());
    // both input and output should reference the same address with their data property
    EXPECT_EQ(nullptr, i->data.lock());
}

// ---------- //
//   Output   //
// ---------- //

TEST(Output, ConstructWithLabel) {
    Output<int> o("Constructed");

    EXPECT_EQ("Constructed", o.label);
}

TEST(Output, Connect) {
    auto i = std::make_shared<Input<int>>(Input<int>());
    auto o = std::make_shared<Output<int>>(Output<int>());

    o->connect(i);

    // input's source address should equal output's address
    EXPECT_EQ(o.get(), i->source.lock().get());
    // output should have exactly one consumer
    EXPECT_EQ(1, o->consumers.size());
    // input's address should be referred to by output's first entry into consumers vector
    EXPECT_EQ(i.get(), o->consumers[0].lock().get());
    // both input and output should reference the same address with their data property
    EXPECT_EQ(o->data.get(), i->data.lock().get());
}

TEST(Output, Disconnect) {
    auto i = std::make_shared<Input<int>>(Input<int>());
    auto o = std::make_shared<Output<int>>(Output<int>());
    o->connect(i);

    o->disconnect(i);

    // input's source address should be nullptr
    EXPECT_EQ(nullptr, i->source.lock());
    // output should now have zero consumers
    EXPECT_EQ(0, o->consumers.size());
    // both input and output should reference the same address with their data property
    EXPECT_EQ(nullptr, i->data.lock());
}
