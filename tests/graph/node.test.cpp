#include <gtest/gtest.h>

#include <iostream>

#include "../../src/graph/node.h"

TEST(Node, get_outputs) {
	Loader l("test");

	auto& outs = l.get_outputs();
	static_assert(
		std::is_const<std::remove_reference<decltype(outs)>::type>::value,
		"Node::get_outputs() is returning non-const vector"
	);
	// std::cout << outs[0].data << std::endl;
	EXPECT_EQ(1, outs.size());
	EXPECT_NE(nullptr, outs[0].data);
}

TEST(Loader, ConstructorEmpty) {
	Loader l("MyLoader");

	EXPECT_NE(nullptr, l.get_outputs()[0].data);
	EXPECT_EQ(0.0f, l.get_outputs()[0].data->data[0][0]);
}

TEST(Loader, ConstructorWithPathPBM) {
    Loader l("MyLoader", "../tests/_data/checker_3x3.pbm");

	// should be a 3x3 checker pattern
	auto img = l.get_outputs()[0].data;
	EXPECT_EQ(3, img->get_width());
	EXPECT_EQ(3, img->get_height());
    for (int i = 0; i < 9; ++i) {
	    EXPECT_EQ(i % 2 == 0 ? 0 : 1, img->data[i][0]);
    }
}

TEST(Loader, OpenPBM) {
    Loader l("MyLoader");
    l.open("../tests/_data/checker_3x3.pbm");

	// should be a 3x3 checker pattern
    auto img = l.get_outputs()[0].data;
	EXPECT_EQ(3, img->get_width());
	EXPECT_EQ(3, img->get_height());
    for (int i = 0; i < 9; ++i) {
	    EXPECT_EQ(i % 2 == 0 ? 0 : 1, img->data[i][0]);
    }
}
