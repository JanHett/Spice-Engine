#include <gtest/gtest.h>

#include <memory>

#include "../../src/graph/node_io.h"

// --------- //
//   Input   //
// --------- //

TEST(Input, _Connect) {
    auto i = std::make_shared<Input<int>>(Input<int>());
    auto o = std::make_shared<Output<int>>(Output<int>());

    i->_connect(o);

    // input's source address should equal output's address
    EXPECT_EQ(o.get(), i->source.lock().get());
    // both input and output should reference the same address with their data property
    EXPECT_EQ(o->data.get(), i->data.lock().get());
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

TEST(Input, _Disconnect) {
    auto i = std::make_shared<Input<int>>(Input<int>());
    auto o = std::make_shared<Output<int>>(Output<int>());
    i->connect(o);

    i->_disconnect();

    // input's source address should be nullptr
    EXPECT_EQ(nullptr, i->source.lock());
    // output should still have exactly one consumer
    EXPECT_EQ(1, o->consumers.size());
    // input's address should still be referred to by output's first entry into consumers vector
    EXPECT_EQ(i.get(), o->consumers[0].lock().get());
    // input should not hold a reference to data anymore
    EXPECT_EQ(nullptr, i->data.lock());
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

TEST(Output, _Connect) {
    auto i = std::make_shared<Input<int>>(Input<int>());
    auto o = std::make_shared<Output<int>>(Output<int>());

    o->_connect(i);

    // output should have exactly one consumer
    EXPECT_EQ(1, o->consumers.size());
    // input's address should be referred to by output's first entry into consumers vector
    EXPECT_EQ(i.get(), o->consumers[0].lock().get());
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

TEST(Output, _Disconnect) {
    auto i = std::make_shared<Input<int>>(Input<int>());
    auto o = std::make_shared<Output<int>>(Output<int>());
    o->connect(i);

    o->_disconnect(i);

    // input's source address should still equal output's address
    EXPECT_EQ(o.get(), i->source.lock().get());
    // output should now have zero consumers
    EXPECT_EQ(0, o->consumers.size());
    // both input and output should still reference the same address with their data property
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
