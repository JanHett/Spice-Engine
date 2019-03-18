#ifndef SPICE_NODE
#define SPICE_NODE

#include <string>
#include <vector>

#include "node_io.h"
#include "../containers/matrix.h"
#include "../containers/pixel.h"

#include "../file_formats/pbm.h"

// Tests if T is a specialization of Template
template <typename T, template <typename...> class Template>
struct is_specialization_of : std::false_type {};
template <template <typename...> class Template, typename... Args>
struct is_specialization_of<Template<Args...>, Template> : std::true_type {};


/**
 * Base class for all nodes.
 * InputTuple should be a std::tuple containing only inputs and vector<input>s.
 * OutputTuple should be a std::tuple containing only outputs.
 * TODO: static_assert that InputTuple and OutputTuple are correct.
 */
template <class InputTuple, class OutputTuple>
class node {
    static_assert(is_specialization_of<InputTuple, std::tuple>::value, "Inputs can only be tuples of inputs or vector<input>s");
    static_assert(is_specialization_of<OutputTuple, std::tuple>::value, "Outputs can only be tuples of outputs or vector<output>s");
protected:
    const std::string _type;

    InputTuple p_inputs;
    OutputTuple p_outputs;

public:
    std::string id;

    node(const char * id, InputTuple inputs = {}, OutputTuple outputs = {}):
    id(id),
    p_inputs(inputs),
    p_outputs(outputs)
    {}

    InputTuple const & inputs() const {
        return p_inputs;
    }

    OutputTuple const & outputs() const {
        return p_outputs;
    }

    /**
     * Apply the node's filter.
     */
    virtual bool apply() = 0;
};

class loader: public node<
    std::tuple<>,                           // no inputs
    std::tuple<output<matrix<pixel<4>>>>    // one output
> {
private:
    std::shared_ptr<matrix<pixel<4>>> data;
public:
    loader(const char * id): node(id, {}, {output<matrix<pixel<4>>>("Image")}) {
        // no actual data yet, but we need an empty image to point the output to to create a valid state
        data = std::make_shared<matrix<pixel<4>>>();
        std::get<0>(p_outputs).data = data;
    }
    
    loader(const char * id, const char * path): node(id, {}, {output<matrix<pixel<4>>>("Image")}) {
        // no need to instanciate pixel matrix out here, this is done in open()
        open(path);
        std::get<0>(p_outputs).data = data;
    }

    /**
     * Sets the source path and loads the specified file as an image.
     */
    bool open(const char * path) {
        try {
            auto m = ppm(path).to_pixel_matrix<4>();
            // if there is not yet a data matrix, create one for this image
            if (data == nullptr) data = std::make_shared<matrix<pixel<4>>>(m);
            else *data = m;
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

/* class convolution: public node<matrix<pixel<4>>> {
private:
    std::shared_ptr<matrix<pixel<4>>> data;
    std::shared_ptr<matrix<pixel<4>>> convolution_matrix;

public:
    convolution(const char * id): node(id) {
        // no actual data yet, but we need an empty image to point the output to to create a valid state
        data = std::make_shared<matrix<pixel<4>>>();
        // 
        inputs.push_back(input<matrix<pixel<4>>>("Image"));
        data = inputs[0].data.lock();
        inputs.push_back(input<matrix<pixel<4>>>("Convolution Matrix"));
        convolution_matrix = inputs[1].data.lock();

        outputs.push_back(output<matrix<pixel<4>>>("Image"));
        outputs[0].data = data;
    }
}; */

#endif // SPICE_NODE
