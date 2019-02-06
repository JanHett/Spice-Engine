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

    std::vector<Input<T>> const & get_inputs() const {
        return inputs;
    }

    std::vector<Output<T>> const & get_outputs() const {
        return outputs;
    }

    /**
     * Apply the node's filter.
     */
    virtual bool apply() = 0;
};

class Loader: public Node<Matrix<Pixel<4>>> {
private:
	std::shared_ptr<Matrix<Pixel<4>>> data;
public:
    Loader(const std::string id): Node(id) {
    	// no actual data yet, but we need an empty pointer to point the output to
        data = std::make_shared<Matrix<Pixel<4>>>();
        outputs.push_back(Output<Matrix<Pixel<4>>>("Image"));
        outputs[0].data = data;
    }
    
    Loader(const std::string id, const std::string path): Node(id) {
        open(path);
        outputs.push_back(Output<Matrix<Pixel<4>>>("Image"));
        outputs[0].data = data;
    }

    /**
     * Sets the source path and loads the file as an image.
     */
    bool open(const std::string path) {
    	data = std::make_shared<Matrix<Pixel<4>>>();
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
