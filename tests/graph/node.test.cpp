#include <gtest/gtest.h>

#include <tuple>

#include "../../src/graph/node.h"

TEST(Node, outputs) {
	loader l("test");

	auto& outs = l.outputs();
	static_assert(
		std::is_const<std::remove_reference<decltype(outs)>::type>::value,
		"Node::outputs() is returning non-const tuple"
	);
	// std::cout << outs[0].data << std::endl;
	EXPECT_EQ(1, std::tuple_size<std::remove_reference<decltype(outs)>::type>::value);
	EXPECT_NE(nullptr, std::get<0>(outs).data);
}

TEST(loader, ConstructorEmpty) {
	loader l("MyLoader");

	EXPECT_NE(nullptr, std::get<0>(l.outputs()).data);
	EXPECT_EQ(0.0f, std::get<0>(l.outputs()).data->data[0][0]);
}

TEST(Loader, ConstructorWithPathPBM) {
    loader l("MyLoader", "../tests/_data/checker_3x3.pbm");

	// should be a 3x3 checker pattern
	auto img = std::get<0>(l.outputs()).data;
	EXPECT_EQ(3, img->width());
	EXPECT_EQ(3, img->height());
    for (int i = 0; i < 9; ++i) {
	    EXPECT_EQ(i % 2 == 0 ? 0 : 1, img->data[i][0]);
    }
}

TEST(Loader, OpenPBM) {
    loader l("MyLoader");
    l.open("../tests/_data/checker_3x3.pbm");

	// should be a 3x3 checker pattern
    auto img = std::get<0>(l.outputs()).data;
	EXPECT_EQ(3, img->width());
	EXPECT_EQ(3, img->height());
    for (int i = 0; i < 9; ++i) {
	    EXPECT_EQ(i % 2 == 0 ? 0 : 1, img->data[i][0]);
    }
}
