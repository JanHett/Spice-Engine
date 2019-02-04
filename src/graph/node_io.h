#ifndef SPICE_NODE_IO
#define SPICE_NODE_IO

#include <string>
#include <memory>
#include <vector>

#include <iostream>

template <class T>
class Output;

template <class T>
class Input: public std::enable_shared_from_this<Input<T>> {
public:
    std::string label;
    std::weak_ptr<T> data;
    std::weak_ptr<Output<T>> source;

    void _connect(const std::shared_ptr<Output<T>> source) {
        this->source = source;
        data = source->data;
    }

    void connect(std::shared_ptr<Output<T>> source) {
        _connect(source);
        source->_connect(this->shared_from_this());
    }

    void _disconnect() {
        source.reset();
        data.reset();
    }

    void disconnect() {
        source.lock()->_disconnect(this->shared_from_this());
        _disconnect();
    }
};

template <class T>
class Output: public std::enable_shared_from_this<Output<T>> {
public:
    std::string label;
    std::shared_ptr<T> data;
    std::vector<std::weak_ptr<Input<T>>> consumers;

    void _connect(const std::shared_ptr<Input<T>> consumer) {
        consumers.push_back(consumer);
    }

    void connect(std::shared_ptr<Input<T>> consumer) {
        _connect(consumer);
        consumer->_connect(this->shared_from_this());
    }

    void _disconnect(std::shared_ptr<Input<T>> consumer) {
        for (auto it = consumers.begin(); it != consumers.end(); ) {
            if (it->lock() == consumer) {
                consumers.erase(it);
                break;
            } else ++it;
        }
    }

    void disconnect(std::shared_ptr<Input<T>> consumer) {
        _disconnect(consumer);
        consumer->disconnect();
    }
};

#endif // SPICE_NODE_IO
