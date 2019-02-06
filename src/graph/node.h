#ifndef SPICE_NODE
#define SPICE_NODE
#include <string>
#include <vector>

#include "node_io.h"
#include "../containers/matrix.h"
#include "../containers/pixel.h"

template <class T>
class Node {
protected:
    const std::string _type;

    std::vector<Input<T>> inputs;
    std::vector<Output<T>> outputs;

public:
    std::string id;

    Node(const std::string id): id(id) {}

    const std::vector<Input<T>>& get_inputs() const {
        return inputs;
    }

    const std::vector<Input<T>>& get_outputs() const {
        return outputs;
    }

    /**
     * Apply the node's filter.
     */
    virtual bool apply() = 0;
};

class Loader: public Node<Matrix<Pixel<4>>> {
public:
    Loader(const std::string id): Node(id) {
        outputs.push_back(Output<Matrix<Pixel<4>>>("Image"));
    }
    
    Loader(const std::string id, const std::string path): Node(id) {
        outputs.push_back(Output<Matrix<Pixel<4>>>("Image"));
        open(path);
    }

    /**
     * Sets the source path and loads the file as an image.
     */
    bool open(const std::string path) {
        return false;
    }

    /**
     * Reloads the source file.
     */
    bool apply() {
        return false;
    }
};

#endif // SPICE_NODE
