#ifndef SPICE_GRAPH
#define SPICE_GRAPH

#include <vector>

#include "./node.h"

class graph {
private:
    std::vector<std::shared_ptr<primitive_node>> p_nodes;

public:
    /**
     * Will add the node to the internal list of managed nodes.
     * Returns `false` if the node could not be added (e.g. because it
     * was already present) and `true` otherwise.
     */
    bool register_node(std::shared_ptr<primitive_node> node) {
        if (std::find(p_nodes.begin(), p_nodes.end(), node) != p_nodes.end()) return false;
        p_nodes.push_back(node);
        return true;
    }

    /**
     * Removes the node from the list of managed nodes.
     * Returns `false` if the node could not be removed (e.g. because it
     * could not be found) and `true` otherwise.
     */
    bool deregister_node(std::shared_ptr<primitive_node> node) {
        auto found = std::find(p_nodes.begin(), p_nodes.end(), node);
        if (found == p_nodes.end()) return false;
        p_nodes.erase(found);
        return true;
    }
};

#endif // SPICE_GRAPH
