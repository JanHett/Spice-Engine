#include <gtest/gtest.h>

#include <memory>

#include "../../src/graph/node_io.h"

TEST(Input, _Connect) {
	auto i = std::make_shared<Input<int>>(Input<int>());
	auto o = std::make_shared<Output<int>>(Output<int>());

	i->_connect(o);

	// input's source address should equal output's address
	EXPECT_EQ(o.get(), i->source.lock().get());
	// input's address should be referred to by output's first entry into consumers vector
	EXPECT_EQ(i.get(), o->consumers[0].lock().get());
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

// --------------------

TEST(Output, _Connect) {
	auto i = std::make_shared<Input<int>>(Input<int>());
	auto o = std::make_shared<Output<int>>(Output<int>());

	o->_connect(i);

	// output should have exactly one consumer
	EXPECT_EQ(1, o->consumers.size());
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
