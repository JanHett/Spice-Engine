#ifndef SPICE_NODE
#define SPICE_NODE

#include <string>
#include <vector>

#include "node_io.h"
#include "../containers/matrix.h"
#include "../containers/pixel.h"

#include "../file_formats/pbm.h"

template <class T>
class node {
protected:
    const std::string _type;

    std::vector<input<T>> inputs;
    std::vector<output<T>> outputs;

public:
    std::string id;

    node(const char * id): id(id) {}

    std::vector<input<T>> const & get_inputs() const {
        return inputs;
    }

    std::vector<output<T>> const & get_outputs() const {
        return outputs;
    }

    /**
     * Apply the node's filter.
     */
    virtual bool apply() = 0;
};

class Loader: public node<matrix<pixel<4>>> {
private:
    std::shared_ptr<matrix<pixel<4>>> data;
public:
    Loader(const char * id): node(id) {
        // no actual data yet, but we need an empty pointer to point the output to
        data = std::make_shared<matrix<pixel<4>>>();
        outputs.push_back(output<matrix<pixel<4>>>("Image"));
        outputs[0].data = data;
    }
    
    Loader(const char * id, const char * path): node(id) {
        open(path);
        outputs.push_back(output<matrix<pixel<4>>>("Image"));
        outputs[0].data = data;
    }

    /**
     * Sets the source path and loads the specified file as an image.
     */
    bool open(const char * path) {
        try {
            auto m = ppm(path).to_pixel_matrix<4>();
            data = std::make_shared<matrix<pixel<4>>>(m);
        } catch (char const * err) {
            fprintf(stderr, "%s\n", err);
            return false;
        }
        return true;
    }

    /**
     * Reloads the source file.
     */
    bool apply() {
        return false;
    }
};

#endif // SPICE_NODE
