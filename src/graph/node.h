#ifndef SPICE_NODE
#define SPICE_NODE
#include <string>
#include <vector>

#include "node_io.h"

template <class T>
class Node {
private:
    const std::string _type;

public:
    std::string id;

    // TODO: allow these to be of arbitrary type
    std::vector<Input<T>> inputs;
    // TODO: allow these to be of arbitrary type
    std::vector<Output<T>> outputs;
};

#endif // SPICE_NODE
