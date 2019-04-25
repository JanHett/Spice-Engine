#ifndef SPICE_NODE_IO
#define SPICE_NODE_IO

#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <iostream>

template <class T>
class output;

template <class T>
class input: public std::enable_shared_from_this<input<T>> {
private:
    template <class fT>
    friend class output;

    std::vector<std::function<void(std::weak_ptr<T>)> * > p_subscribers;

    /**
     * The part of input::connect() that doesn't interact with output
     */
    void p_connect(output<T> & source) {
        this->source = &source;
        data = source.data;
        source.subscribe(p_on_data_updated);
    }

    /**
     * The part of input::disconnect() that doesn't interact with output
     */
    void p_disconnect() {
        source->unsubscribe(p_on_data_updated);
        source = nullptr;
        data.reset();
    }

    /** callback to be handed to connected output */
    std::function<void(std::weak_ptr<T>)> p_on_data_updated = [&](std::weak_ptr<T> new_data) {
        data = new_data;
    };

public:
    std::string label;
    // TODO: make these private
    std::weak_ptr<T> data; // this is undefined after construction
    output<T> * source = nullptr;

    input() = default;
    input(const std::string label): label(label) {}
    ~input() { if (source != nullptr) source->p_disconnect(*this); }

    /**
     * Hooks this input up to a source.
     */
    void connect(output<T> & source) {
        p_connect(source);
        source.p_connect(*this);
    }

    /**
     * Breaks the link with this input's source.
     */
    void disconnect() {
        source->p_disconnect(*this);
        p_disconnect();
    }

    output<T> * const get_source() {
        return source;
    }

    /**
     * Registers an event listener to be called when the data
     * transported by this output has changed.
     */
    void subscribe(std::function<void(std::weak_ptr<T>)> & callback) {
        callback(data);
        p_subscribers.push_back(&callback);
    }

    void unsubscribe(std::function<void(std::weak_ptr<T>)> & callback) {
        auto cb_to_remove = std::find(p_subscribers.begin(), p_subscribers.end(), &callback);
        if (cb_to_remove != p_subscribers.end()) p_subscribers.erase(cb_to_remove);
    }
};

template <class T>
class output: public std::enable_shared_from_this<output<T>> {
private:
    template <class fT>
    friend class input;

    std::vector<std::function<void(std::weak_ptr<T>)> *> p_subscribers {};

    /**
     * The part of output::connect() that doesn't interact with input
     */
    void p_connect(input<T> & consumer) {
        consumers.push_back(&consumer);
    }

    /**
     * The part of output::disconnect() that doesn't interact with input
     */
    void p_disconnect(input<T> & consumer) {
        auto consumer_to_remove = std::find(consumers.begin(), consumers.end(), &consumer);
        if (consumer_to_remove != consumers.end()) consumers.erase(consumer_to_remove);
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
            consumer->p_disconnect();
        }
    }

    /**
     * Adds an input that will consume data from this output.
     */
    void connect(input<T> & consumer) {
        p_connect(consumer);
        consumer.p_connect(*this);
    }

    /**
     * Removes a consumer from this output.
     */
    void disconnect(input<T> & consumer) {
        p_disconnect(consumer);
        consumer.p_disconnect();
    }

    /**
     * Registers an event listener to be called when the data
     * transported by this output has changed.
     */
    void subscribe(std::function<void(std::weak_ptr<T>)> & callback) {
        callback(data);
        p_subscribers.push_back(&callback);
    }

    void unsubscribe(std::function<void(std::weak_ptr<T>)> & callback) {
        auto cb_to_remove = std::find(p_subscribers.begin(), p_subscribers.end(), &callback);
        if (cb_to_remove != p_subscribers.end()) p_subscribers.erase(cb_to_remove);
    }

    /**
     * Calls all subscribers, passing them the current state of data.
     */
    void publish() const {
        for (auto cb: p_subscribers)
            cb(data);
    }
};

#endif // SPICE_NODE_IO
