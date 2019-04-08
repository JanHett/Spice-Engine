#include <gtest/gtest.h>

#include "../../src/graph/graph.h"

class data_sink_t: public basic_node {
public:
    data_sink_t(): basic_node("data_sink_t", true) {}
};

TEST(graph, create_node) {
    graph g;
    EXPECT_EQ(0, g.size());

    auto node_id = "my_basic_node";
    auto& n = g.create_node<basic_node>(node_id);

    EXPECT_EQ(1, g.size());
    static_assert((std::is_same<basic_node&,
        decltype(n)>::value), "create_node did not return a reference");

    auto& loader_n = g.create_node<loader>("my_loader");
    EXPECT_EQ(2, g.size());
    static_assert((std::is_same<loader&,
        decltype(loader_n)>::value), "create_node did not return a reference");
}

TEST(graph, register_node) {
    graph g;
    EXPECT_EQ(0, g.size());

    auto n = std::make_unique<basic_node>("my_basic_node");
    auto registered = g.register_node(std::move(n));
    EXPECT_EQ(1, g.size());
    EXPECT_TRUE(registered);

    auto ds = std::make_unique<data_sink_t>();
    EXPECT_EQ(1, g.size());
    EXPECT_EQ(0, g.data_sinks().size());
    auto registered_ds = g.register_node(std::move(ds));
    EXPECT_EQ(2, g.size());
    EXPECT_EQ(1, g.data_sinks().size());
    EXPECT_TRUE(registered_ds);
}

TEST(graph, deregister_node) {
    graph g;
    EXPECT_EQ(0, g.size());

    auto n = std::make_unique<basic_node>("my_basic_node");
    auto n_ptr = n.get();
    auto deregistered = g.deregister_node(*n_ptr);
    EXPECT_EQ(0, g.size());
    EXPECT_FALSE(deregistered);

    auto registered = g.register_node(std::move(n));
    EXPECT_EQ(1, g.size());
    EXPECT_TRUE(registered);

    auto deregistered_again = g.deregister_node(*n_ptr);
    EXPECT_EQ(0, g.size());
    EXPECT_TRUE(deregistered_again);

    // check data sink peculiarities
    auto ds = std::make_unique<data_sink_t>();
    auto ds_ptr = ds.get();
    EXPECT_EQ(0, g.size());
    EXPECT_EQ(0, g.data_sinks().size());

    auto deregistered_ds = g.deregister_node(*ds_ptr);
    EXPECT_EQ(0, g.size());
    EXPECT_FALSE(deregistered_ds);
    EXPECT_EQ(0, g.data_sinks().size());

    auto registered_ds = g.register_node(std::move(ds));
    EXPECT_EQ(1, g.size());
    EXPECT_EQ(1, g.data_sinks().size());
    EXPECT_TRUE(registered_ds);

    auto deregistered_ds_again = g.deregister_node(*ds_ptr);
    EXPECT_EQ(0, g.size());
    EXPECT_EQ(0, g.data_sinks().size());
    EXPECT_TRUE(deregistered_ds_again);
}
