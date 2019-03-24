#include <gtest/gtest.h>

#include <tuple>

#include "../../src/graph/node.h"

TEST(node, outputs) {
    loader l("test");

    auto& outs = l.outputs();
    static_assert(
        std::is_const<std::remove_reference<decltype(outs)>::type>::value,
        "node::outputs() is returning non-const tuple"
    );
    // std::cout << outs[0].data << std::endl;
    EXPECT_EQ(1, std::tuple_size<std::remove_reference<decltype(outs)>::type>::value);
    EXPECT_NE(nullptr, std::get<0>(outs).data);
}

TEST(node, outputsAtIndex) {
    loader l("test");

    auto& out = l.outputs<0>();
    static_assert(
        std::is_const<std::remove_reference<decltype(out)>::type>::value,
        "node::outputs<0>() is returning non-const input"
    );
    EXPECT_EQ(&std::get<0>(l.outputs()), &out);
    EXPECT_NE(nullptr, out.data);
}

TEST(node__loader, ConstructorEmpty) {
    loader l("MyLoader");

    EXPECT_NE(nullptr, l.outputs<0>().data);
    EXPECT_EQ(0.0f, l.outputs<0>().data->data[0][0]);
}

TEST(node__loader, ConstructorWithPathPBM) {
    loader l("MyLoader", "../tests/_data/checker_3x3.pbm");

    // should be a 3x3 checker pattern
    auto img = l.outputs<0>().data;
    EXPECT_EQ(3, img->width());
    EXPECT_EQ(3, img->height());
    for (int i = 0; i < 9; ++i) {
        EXPECT_EQ(i % 2 == 0 ? 0 : 1, img->data[i][0]);
    }
}

TEST(node__loader, OpenPBM) {
    loader l("MyLoader");
    l.open("../tests/_data/checker_3x3.pbm");

    // should be a 3x3 checker pattern
    auto img = l.outputs<0>().data;
    EXPECT_EQ(3, img->width());
    EXPECT_EQ(3, img->height());
    for (int i = 0; i < 9; ++i) {
        EXPECT_EQ(i % 2 == 0 ? 0 : 1, img->data[i][0]);
    }
}

/* TEST(node__out_stream, constructor) {
    loader l("MyLoader", "../tests/_data/checker_3x3.pbm");
    out_stream os_node("MyOutStream");
    os_node.inputs<0>().connect(l.outputs<0>());
} */

/* TEST(node__fast_blur, ConstructorEmpty) {
    fast_blur fb("MyBlur");

    // check that data is nullptr
} */
