#ifndef SPICE_GRAPH
#define SPICE_GRAPH

#include <vector>

#include "./node.h"

/// A graph manages a collection of nodes and keeps a record of connections in
/// the form of a distance matrix. This means that a graph is aware of which
/// individual nodes are connected, but it does not concern itself with the
/// specificities of the data flow (i.e. which outputs are connected).
class graph {
private:
    std::vector<std::unique_ptr<basic_node>> p_nodes;
    std::vector<basic_node*> p_data_sinks;

public:
    /**
     * Creates a node of type NodeT and registers it as being part of this graph.
     */
    template <
        class NodeT,
        class... Args,
        std::enable_if_t<std::is_constructible<NodeT, Args&&...>::value, int> = 0>
    NodeT& create_node(Args&&... args)
    {
        static_assert(std::is_base_of<basic_node, NodeT>::value,
            "create_node() can only create types derived from node");

        auto n = std::make_unique<NodeT>(std::forward<Args>(args)...);
        auto n_ptr = n.get();

        register_node(std::move(n));

        return *n;
    }

    /**
     * Adds a preexisting node to the internal list of managed nodes.
     * Returns `false` if the node could not be added (e.g. because it
     * was already present) and `true` otherwise.
     */
    bool register_node(std::unique_ptr<basic_node> node)
    {
        if (std::find(p_nodes.begin(), p_nodes.end(), node) != p_nodes.end())
            return false;
        
        // TODO: make this constexpr
        if (node->is_data_sink)
            p_data_sinks.push_back(node.get());
        p_nodes.push_back(std::move(node));
        
        return true;
    }

    /**
     * Removes the node from the list of managed nodes.
     * Returns `false` if the node could not be removed (e.g. because it
     * could not be found) and `true` otherwise.
     */
    bool deregister_node(basic_node& node)
    {
        // if this is a data sink, we don't return true unless it was also
        // removed from data_sinks.
        if (node.is_data_sink) { // TODO: make this contexpr
            auto found_in_data_sinks = std::find(
                p_data_sinks.begin(),
                p_data_sinks.end(),
                &node);
            if (found_in_data_sinks == p_data_sinks.end())
                return false;
            p_data_sinks.erase(found_in_data_sinks);
        }

        auto found = std::find_if(p_nodes.begin(), p_nodes.end(),
            [&node](std::unique_ptr<basic_node> const & arg){
                return arg.get() == &node; });
        if (found == p_nodes.end())
            return false;
        p_nodes.erase(found);

        return true;
    }

    size_t size()
    {
        return p_nodes.size();
    }

    std::vector<basic_node*> const& data_sinks()
    {
        return p_data_sinks;
    }
};

#endif // SPICE_GRAPH
