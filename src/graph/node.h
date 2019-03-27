#ifndef SPICE_NODE
#define SPICE_NODE

#include <string>
#include <vector>

#include "node_io.h"
#include "observable.h"
#include "../containers/image.h"
#include "../containers/pixel.h"

#include "../file_formats/pbm.h"

// Tests if T is a specialization of Template
template <typename T, template <typename...> class Template>
struct is_specialization_of : std::false_type {};
template <template <typename...> class Template, typename... Args>
struct is_specialization_of<Template<Args...>, Template> : std::true_type {};

enum class node_flags: uint8_t {
    clear               = 0,
    computed_subgraph   = 1,        // the node is connected to a data sink, thus part of the computed subtree
    input_missing       = 1 << 1    // the node is missing a required input
};

class basic_node {
public:
    std::string name;
    const bool is_data_sink;    // TODO: guarantee this to be compile-time evaluated

    basic_node(const bool is_data_sink = false):
    is_data_sink(is_data_sink)
    {}
    basic_node(const char * name, const bool is_data_sink = false):
    name(name),
    is_data_sink(is_data_sink)
    {}
};

template <class T>
constexpr auto make_callback_lambda(T* & target) {
    return [&target](T * const value) {
        target = value;
    };
}

template<typename SourceTuple, std::size_t... I>
constexpr auto make_callback_tuple_impl(const SourceTuple& src, std::index_sequence<I...>) {
    return std::make_tuple(
        make_callback_lambda(std::get<I>(src))...
    );
}

template<typename SourceTuple, typename Indices = std::make_index_sequence<std::tuple_size<SourceTuple>::value>>
constexpr auto make_callback_tuple(SourceTuple const & src) {
    return make_callback_tuple_impl(src, Indices{});
}

template <class InputTuple, class OutputTuple, bool is_data_sink_t = false>
class obs_node: public basic_node {
protected:
    InputTuple p_inputs;
    OutputTuple p_outputs;
    std::array<
        // TODO: find a way to type the argument (e.g. tuple map)!
        std::function<void(void * const)>,
        std::tuple_size<InputTuple>::value
    > subscribers;

public:
    obs_node(InputTuple inputs = {}, OutputTuple outputs = {}):
    basic_node(is_data_sink_t)
    {}
    obs_node(const char * name, InputTuple inputs = {}, OutputTuple outputs = {}):
    basic_node(name, is_data_sink_t)
    {}

    //
    // i/o getters
    //

    constexpr InputTuple const & inputs() const {
        return p_inputs;
    }

    template<unsigned int index>
    constexpr auto const & inputs() const {
        return std::get<index>(p_inputs);
    }

    constexpr OutputTuple const & outputs() const {
        return p_outputs;
    }

    template<unsigned int index>
    constexpr auto const & outputs() const {
        return std::get<index>(p_outputs);
    }

    //
    // non-const i/o getters
    //

    constexpr InputTuple & inputs() {
        return p_inputs;
    }

    template<unsigned int index>
    constexpr auto & inputs() {
        return std::get<index>(p_inputs);
    }

    constexpr OutputTuple & outputs() {
        return p_outputs;
    }

    template<unsigned int index>
    constexpr auto & outputs() {
        return std::get<index>(p_outputs);
    }

    //
    // subscription management
    //

    /**
     * Links the value of the input at index `input_index` to the observable
     * `source`.
     */
    template<size_t input_index>
    bool subscribe(
        observable<decltype(std::get<input_index>(p_inputs))> & source
    ) {

    }
};

/**
 * Base class for all nodes.
 * InputTuple should be a std::tuple containing only inputs and vector<input>s.
 * OutputTuple should be a std::tuple containing only outputs.
 * TODO: static_assert that InputTuple and OutputTuple conatain the correct types.
 */
template <class InputTuple, class OutputTuple, bool is_data_sink_t = false>
class node: public basic_node {
    static_assert(is_specialization_of<InputTuple, std::tuple>::value,
        "Inputs can only be tuples of shared_ptr<inputs> or vector<shared_ptr<input>>s.");
    static_assert(is_specialization_of<OutputTuple, std::tuple>::value,
        "Outputs can only be tuples of outputs.");
protected:
    const std::string _type;

    InputTuple p_inputs;
    OutputTuple p_outputs;

public:
    std::string id;

    node(const char * id, InputTuple inputs = {}, OutputTuple outputs = {}):
    basic_node(id, is_data_sink_t)
    {}

    // i/o getters

    constexpr InputTuple const & inputs() const {
        return p_inputs;
    }

    template<unsigned int index>
    constexpr auto const & inputs() const {
        return std::get<index>(p_inputs);
    }

    constexpr OutputTuple const & outputs() const {
        return p_outputs;
    }

    template<unsigned int index>
    constexpr auto const & outputs() const {
        return std::get<index>(p_outputs);
    }

    // non-const i/o getters

    constexpr InputTuple & inputs() {
        return p_inputs;
    }

    template<unsigned int index>
    constexpr auto & inputs() {
        return std::get<index>(p_inputs);
    }

    constexpr OutputTuple & outputs() {
        return p_outputs;
    }

    template<unsigned int index>
    constexpr auto & outputs() {
        return std::get<index>(p_outputs);
    }

    template<class T>
    void connect(input<T>& in, output<T>& out) {
        in.connect(out);
    }
};

class loader: public node<
    std::tuple<>,                    // no inputs
    std::tuple<output<rgb_image>>    // one output
> {
private:
    std::shared_ptr<rgb_image> data;
public:
    loader(const char * id):
    node(id, {}, {output<rgb_image>("Image")}),
    data(nullptr)
    {
    }
    
    loader(const char * id, const char * path): node(id, {}, {output<rgb_image>("Image")}) {
        open(path);
    }

    /**
     * Sets the source path and loads the specified file as an image.
     */
    bool open(const char * path) {
        try {
            auto m = ppm(path).to_pixel_matrix<3>();
            // if there is not yet a data matrix, create one for this image
            if (data == nullptr) {
                data = std::make_shared<rgb_image>(m);
                std::get<0>(p_outputs).data = data;
            }
            else *data = m;
        } catch (char const * err) {
            fprintf(stderr, "%s\n", err);
            return false;
        }
        return true;
    }
};

class fast_blur: public node<
    std::tuple<input<rgb_image>, input<float>, input<unsigned int>>,
    std::tuple<output<rgb_image>>
> {
private:
    // aliases for in-/outputs contained in tuples
    input<rgb_image>& in_image;
    input<float>& ctrl_radius;
    input<unsigned int>& ctrl_passes;
    output<rgb_image>& out_image;

    std::shared_ptr<rgb_image> data;
public:
    fast_blur(const char * id):
    node(
        id,
        { // inputs
            input<rgb_image>("Image"),
            input<float>("Radius"),
            input<unsigned int>("Passes")
        }, { // outputs
            output<rgb_image>("Image")
        }
    ),
    // in-/output aliases
    in_image(std::get<0>(p_inputs)),
    ctrl_radius(std::get<1>(p_inputs)),
    ctrl_passes(std::get<2>(p_inputs)),
    out_image(std::get<0>(p_outputs))
    {
        *data = in_image.data.lock()->fast_blur(*ctrl_radius.data.lock(), *ctrl_passes.data.lock());
        out_image.data = data;
    }

    bool apply() {
        *data = data->fast_blur(*ctrl_radius.data.lock(), *ctrl_passes.data.lock());
        return false;
    }
};

class out_stream: public node<
    std::tuple<input<rgb_image>>,
    std::tuple<>,
    true
> {
private:
    std::ostream & p_stream;

    std::shared_ptr<rgb_image> data;

    std::function<void(std::weak_ptr<rgb_image>)> p_on_input_updated = [&](std::weak_ptr<rgb_image> new_img) {
        std::cout << "Input data changed. ";
        data = new_img.lock();
        std::cout << "New image is at " << data << "\n";
    };
public:
    out_stream(const char * id, std::ostream & stream = std::cout):
    node(
        id,
        { input<rgb_image>("Image") },
        {}
    ),
    p_stream(stream),
    data(std::get<0>(p_inputs).data.lock())
    {
        inputs<0>().subscribe(p_on_input_updated);
    }

    auto _data() { return data; }

    bool apply() {
        print_color(*data);
        return true;
    }
};

/* class convolution: public node<rgb_image> {
private:
    std::shared_ptr<rgb_image> data;
    std::shared_ptr<rgb_image> convolution_matrix;

public:
    convolution(const char * id): node(id) {
        // no actual data yet, but we need an empty image to point the output to to create a valid state
        data = std::make_shared<rgb_image>();
        // 
        inputs.push_back(input<rgb_image>("Image"));
        data = inputs[0].data.lock();
        inputs.push_back(input<rgb_image>("Convolution Matrix"));
        convolution_matrix = inputs[1].data.lock();

        outputs.push_back(output<rgb_image>("Image"));
        outputs[0].data = data;
    }
}; */

#endif // SPICE_NODE
