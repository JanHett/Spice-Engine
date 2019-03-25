#ifndef SPICE_NODE_IO
#define SPICE_NODE_IO

#include <string>
#include <memory>
#include <vector>

template <class T>
class output;

template <class T>
class input: public std::enable_shared_from_this<input<T>> {
private:
    template <class fT>
    friend class output;

    /**
     * The part of input::connect() that doesn't interact with output
     */
    void _connect(output<T> & source) {
        this->source = &source;
        data = source.data;
    }

    /**
     * The part of input::disconnect() that doesn't interact with output
     */
    void _disconnect() {
        source = nullptr;
        data.reset();
    }

public:
    std::string label;
    // TODO: make these private
    std::weak_ptr<T> data; // this is undefined after construction
    output<T> * source = nullptr;

    input() = default;
    input(const std::string label): label(label) {}
    ~input() { if (source != nullptr) source->_disconnect(*this); }

    /**
     * Hooks this input up to a source.
     */
    void connect(output<T> & source) {
        _connect(source);
        source._connect(*this);
    }

    /**
     * Breaks the link with this input's source.
     */
    void disconnect() {
        source->_disconnect(*this);
        _disconnect();
    }

    output<T> * const get_source() {
        return source;
    }
};

template <class T>
class output: public std::enable_shared_from_this<output<T>> {
private:
    template <class fT>
    friend class input;

    /**
     * The part of output::connect() that doesn't interact with input
     */
    void _connect(input<T> & consumer) {
        consumers.push_back(&consumer);
    }

    /**
     * The part of output::disconnect() that doesn't interact with input
     */
    void _disconnect(input<T> & consumer) {
        for (auto it = consumers.begin(); it != consumers.end(); ) {
            if (*it == &consumer) {
                consumers.erase(it);
                return;
            } else ++it;
        }
    }

public:
    std::string label;
    // TODO: make these private
    std::weak_ptr<T> data; // this is undefined after construction
    std::vector<input<T> *> consumers;

    output() = default;
    output(const std::string label): label(label) {}

    ~output() {
        for (auto consumer: consumers) {
            consumer->_disconnect();
        }
    }

    /**
     * Adds an input that will consume data from this output.
     */
    void connect(input<T> & consumer) {
        _connect(consumer);
        consumer._connect(*this);
    }

    /**
     * Removes a consumer from this output.
     */
    void disconnect(input<T> & consumer) {
        _disconnect(consumer);
        consumer._disconnect();
    }
};

#endif // SPICE_NODE_IO
