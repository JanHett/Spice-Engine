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
    auto n = g.create_node<basic_node>(node_id);

    EXPECT_EQ(1, g.size());
    EXPECT_TRUE((std::is_same<std::shared_ptr<basic_node>, decltype(n)>::value));
    EXPECT_EQ(2, n.use_count());

    auto loader_n = g.create_node<loader>("my_loader");
    EXPECT_EQ(2, g.size());
    EXPECT_TRUE((std::is_same<std::shared_ptr<loader>, decltype(loader_n)>::value));
    EXPECT_EQ(2, loader_n.use_count());
}

TEST(graph, register_node) {
    graph g;
    EXPECT_EQ(0, g.size());

    auto n = std::make_shared<basic_node>("my_basic_node");
    EXPECT_EQ(1, n.use_count());
    auto registered = g.register_node(n);
    EXPECT_EQ(1, g.size());
    EXPECT_TRUE(registered);
    EXPECT_EQ(2, n.use_count());

    auto registered_again = g.register_node(n);
    EXPECT_EQ(1, g.size());
    EXPECT_FALSE(registered_again);
    EXPECT_EQ(2, n.use_count());

    auto ds = std::make_shared<data_sink_t>();
    EXPECT_EQ(1, g.size());
    EXPECT_EQ(0, g.data_sinks().size());
    EXPECT_EQ(1, ds.use_count());
    auto registered_ds = g.register_node(ds);
    EXPECT_EQ(2, g.size());
    EXPECT_EQ(1, g.data_sinks().size());
    EXPECT_TRUE(registered_ds);
    EXPECT_EQ(3, ds.use_count());
}

TEST(graph, deregister_node) {
    graph g;
    EXPECT_EQ(0, g.size());

    auto n = std::make_shared<basic_node>("my_basic_node");
    EXPECT_EQ(1, n.use_count());
    auto deregistered = g.deregister_node(n);
    EXPECT_EQ(0, g.size());
    EXPECT_FALSE(deregistered);
    EXPECT_EQ(1, n.use_count());

    auto registered = g.register_node(n);
    EXPECT_EQ(1, g.size());
    EXPECT_TRUE(registered);
    EXPECT_EQ(2, n.use_count());

    auto deregistered_again = g.deregister_node(n);
    EXPECT_EQ(0, g.size());
    EXPECT_TRUE(deregistered_again);
    EXPECT_EQ(1, n.use_count());

    // check data sink peculiarities
    auto ds = std::make_shared<data_sink_t>();
    EXPECT_EQ(0, g.size());
    EXPECT_EQ(0, g.data_sinks().size());
    EXPECT_EQ(1, ds.use_count());

    auto deregistered_ds = g.deregister_node(ds);
    EXPECT_EQ(0, g.size());
    EXPECT_FALSE(deregistered_ds);
    EXPECT_EQ(0, g.data_sinks().size());
    EXPECT_EQ(1, ds.use_count());

    auto registered_ds = g.register_node(ds);
    EXPECT_EQ(1, g.size());
    EXPECT_EQ(1, g.data_sinks().size());
    EXPECT_TRUE(registered_ds);
    EXPECT_EQ(3, ds.use_count());

    auto deregistered_ds_again = g.deregister_node(ds);
    EXPECT_EQ(0, g.size());
    EXPECT_EQ(0, g.data_sinks().size());
    EXPECT_TRUE(deregistered_ds_again);
    EXPECT_EQ(1, ds.use_count());
}
