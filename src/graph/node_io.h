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
    void _connect(const std::shared_ptr<output<T>> source) {
        this->source = source;
        data = source->data;
    }

    /**
     * The part of input::disconnect() that doesn't interact with output
     */
    void _disconnect() {
        source.reset();
        data.reset();
    }

public:
    std::string label;
    // TODO: make these private
    std::weak_ptr<T> data; // this is undefined after construction
    std::weak_ptr<output<T>> source;

    input() = default;
    input(const std::string label): label(label) {}

    /**
     * Hooks this input up to a source.
     */
    void connect(std::shared_ptr<output<T>> source) {
        _connect(source);
        source->_connect(this->shared_from_this());
    }

    /**
     * Breaks the link with this input's source.
     */
    void disconnect() {
        source.lock()->_disconnect(this->shared_from_this());
        _disconnect();
    }

    const std::shared_ptr<output<T>> get_source() {
        return source.lock();
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
    void _connect(const std::shared_ptr<input<T>> consumer) {
        consumers.push_back(consumer);
    }

    /**
     * The part of output::disconnect() that doesn't interact with input
     */
    void _disconnect(std::shared_ptr<input<T>> consumer) {
        for (auto it = consumers.begin(); it != consumers.end(); ) {
            if (it->lock() == consumer) {
                consumers.erase(it);
                return;
            } else ++it;
        }
    }

public:
    std::string label;
    // TODO: make these private
    std::shared_ptr<T> data; // this is undefined after construction
    std::vector<std::weak_ptr<input<T>>> consumers;

    output() = default;
    output(const std::string label): label(label) {}

    /**
     * Adds an input that will consume data from this output.
     */
    void connect(std::shared_ptr<input<T>> consumer) {
        _connect(consumer);
        consumer->_connect(this->shared_from_this());
    }

    /**
     * Removes a consumer from this output.
     */
    void disconnect(std::shared_ptr<input<T>> consumer) {
        _disconnect(consumer);
        consumer->disconnect();
    }
};

#endif // SPICE_NODE_IO
