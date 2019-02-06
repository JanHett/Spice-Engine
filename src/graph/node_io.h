#ifndef SPICE_NODE_IO
#define SPICE_NODE_IO

#include <string>
#include <memory>
#include <vector>

template <class T>
class Output;

template <class T>
class Input: public std::enable_shared_from_this<Input<T>> {
private:
    template <class fT>
    friend class Output;

    /**
     * The part of Input::connect() that doesn't interact with Output
     */
    void _connect(const std::shared_ptr<Output<T>> source) {
        this->source = source;
        data = source->data;
    }

    /**
     * The part of Input::disconnect() that doesn't interact with Output
     */
    void _disconnect() {
        source.reset();
        data.reset();
    }

public:
    std::string label;
    // TODO: make these private
    std::weak_ptr<T> data;
    std::weak_ptr<Output<T>> source;

    Input() = default;
    Input(const std::string label): label(label) {}

    /**
     * Hooks this Input up to a source.
     */
    void connect(std::shared_ptr<Output<T>> source) {
        _connect(source);
        source->_connect(this->shared_from_this());
    }

    /**
     * Breaks the link with this Input's source.
     */
    void disconnect() {
        source.lock()->_disconnect(this->shared_from_this());
        _disconnect();
    }

    const std::shared_ptr<Output<T>> get_source() {
        return source.lock();
    }
};

template <class T>
class Output: public std::enable_shared_from_this<Output<T>> {
private:
    template <class fT>
    friend class Input;

    /**
     * The part of Output::connect() that doesn't interact with Input
     */
    void _connect(const std::shared_ptr<Input<T>> consumer) {
        consumers.push_back(consumer);
    }

    /**
     * The part of Output::disconnect() that doesn't interact with Input
     */
    void _disconnect(std::shared_ptr<Input<T>> consumer) {
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
    std::shared_ptr<T> data;
    std::vector<std::weak_ptr<Input<T>>> consumers;

    Output() = default;
    Output(const std::string label): label(label) {}

    /**
     * Adds an Input that will consume data from this Output.
     */
    void connect(std::shared_ptr<Input<T>> consumer) {
        _connect(consumer);
        consumer->_connect(this->shared_from_this());
    }

    /**
     * Removes a consumer from this Output.
     */
    void disconnect(std::shared_ptr<Input<T>> consumer) {
        _disconnect(consumer);
        consumer->disconnect();
    }
};

#endif // SPICE_NODE_IO
