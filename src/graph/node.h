#ifndef SPICE_NODE
#define SPICE_NODE

#include <string>
#include <vector>
#include <type_traits>

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
constexpr std::function<void(T const)> make_callback_lambda(T * target) {
    static_assert(!std::is_null_pointer<decltype(target)>::value,
        "Cannot create a callback lambda for nullptr target.");
    // TODO: get rid of duplicate runtime check
    if (target == nullptr) throw "Cannot create a callback lambda for nullptr target.";
    // TODO: make lambda constexpr?
    return [target](T const value) {
        *target = value;
    };
}

template<typename SourceTuple, std::size_t... I>
constexpr auto make_callback_tuple_impl(SourceTuple & src, std::index_sequence<I...>) {
    return std::make_tuple(
        make_callback_lambda(&std::get<I>(src))...
    );
}

/**
 * Creates a tuple of lambdas with size equal to the size of the tuple passed as
 * the argument. The lambdas each take a single parameter of a type equal to the
 * type of their corresponding entry of the source tuple and set this entry
 * equal the value of the argument when called.
 */
template<
    typename SourceTuple,
    typename Indices = std::make_index_sequence<
        std::tuple_size<SourceTuple>::value
    >
>
constexpr auto make_callback_tuple(SourceTuple & src) {
    return make_callback_tuple_impl(src, Indices{});
}

template<class TupleT>
using callback_tuple_t = typename std::invoke_result<
        decltype(make_callback_tuple<TupleT>), TupleT*
    >::type;

template <class InputTuple, class OutputTuple, bool is_data_sink_t = false>
class node: public basic_node {
protected:
    InputTuple p_inputs;
    OutputTuple p_outputs;
    callback_tuple_t<InputTuple> p_subscribers;

public:
    node(InputTuple inputs = {}, OutputTuple outputs = {}):
    basic_node(is_data_sink_t),
    p_inputs{inputs},
    p_outputs{outputs},
    p_subscribers(make_callback_tuple(&p_inputs))
    {}
    node(const char * name, InputTuple inputs = {}, OutputTuple outputs = {}):
    basic_node(name, is_data_sink_t),
    p_inputs{inputs},
    p_outputs{outputs},
    p_subscribers(make_callback_tuple(p_inputs))
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
        source.subscribe(std::get<input_index>(p_subscribers));
    }

    /**
     * Stops the input at index `input_index` listening for changes to the
     * observable `source`.
     */
    template<size_t input_index>
    bool unsubscribe(
        observable<decltype(std::get<input_index>(p_inputs))> & source
    ) {
        source.unsubscribe(std::get<input_index>(p_subscribers));
    }
};

/**
 * Base class for all nodes.
 * InputTuple should be a std::tuple containing only inputs and vector<input>s.
 * OutputTuple should be a std::tuple containing only outputs.
 * TODO: static_assert that InputTuple and OutputTuple conatain the correct types.
 */
template <class InputTuple, class OutputTuple, bool is_data_sink_t = false>
class node_: public basic_node {
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

    node_(const char * id, InputTuple inputs = {}, OutputTuple outputs = {}):
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
    std::tuple<>,
    std::tuple<observable<rgb_image>>
> {
public:
    loader(const char * id):
    node(id, {}, { (observable<rgb_image>) })
    {
    }
    
    loader(const char * id, const char * path): node(id, {}, {observable<rgb_image>{}}) {
        open(path);
    }

    /**
     * Sets the source path and loads the specified file as an image.
     */
    bool open(const char * path) {
        try {
            outputs<0>().set(ppm(path).to_pixel_matrix<3>());
        } catch (char const * err) {
            fprintf(stderr, "%s\n", err);
            return false;
        }
        return true;
    }
};

class fast_blur: public node<
    std::tuple<rgb_image *, float *, unsigned int *>,
    std::tuple<observable<rgb_image>>
> {
private:
    // aliases for in-/outputs contained in tuples
    rgb_image*& in_image;
    float*& ctrl_radius;
    unsigned int*& ctrl_passes;
    observable<rgb_image>& out_image;

    std::shared_ptr<rgb_image> data;
public:
    fast_blur(const char * id):
    node(
        id,
        { // inputs
            new rgb_image{},
            new float{},
            new unsigned int{}
        }, { // outputs
            observable<rgb_image>{}
        }
    ),
    // in-/output aliases
    in_image(std::get<0>(p_inputs)),
    ctrl_radius(std::get<1>(p_inputs)),
    ctrl_passes(std::get<2>(p_inputs)),
    out_image(std::get<0>(p_outputs))
    {
    }

    bool apply() {
        out_image.set(in_image->fast_blur(*ctrl_radius, *ctrl_passes));
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
