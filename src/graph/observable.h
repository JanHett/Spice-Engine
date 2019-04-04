#ifndef SPICE_OBSERVABLE
#define SPICE_OBSERVABLE

#include <functional>
#include <memory>
#include <vector>

/**
 * Mutable value container which notifies a set of listeners
 * whenever the internal value is changed via observable::set()
 */
template <class T>
class observable {
private:
    std::unique_ptr<T> p_value = nullptr;
    std::vector<std::function<void(T* const)>*> p_subscribers {};

public:
    /**
     * Overwrites the current value and notifies subscribers.
     */
    void set(T&& value)
    {
        if (p_value == nullptr)
            p_value = std::make_unique<T>(value);
        else
            *p_value = value;

        for (auto sub : p_subscribers)
            (*sub)(p_value.get());
    }

    /**
     * Resets the internal data pointer to a nullptr.
     */
    void reset()
    {
        p_value.reset(nullptr);
        for (auto sub : p_subscribers)
            (*sub)(p_value.get());
    }

    /**
     * Gets a pointer to the current state of the observable's value.
     */
    T const* const get() const
    {
        return p_value.get();
    }

    /**
     * Registers this function as a subscriber and will call it
     * when the data is updated.
     * Returns `true` if the subscriber was added and `false`
     * otherwise.
     */
    bool subscribe(std::function<void(T* const)>& subscriber)
    {
        if (std::find(
                std::begin(p_subscribers),
                std::end(p_subscribers),
                &subscriber)
            != std::end(p_subscribers))
            return false;
        p_subscribers.push_back(&subscriber);
        subscriber(p_value.get());
        return true;
    }

    /**
     * Stops notifications to the subscriber.
     * Returns `true` if the function was found and removed
     * and `false` otherwise.
     */
    bool unsubscribe(std::function<void(T* const)>& subscriber)
    {
        auto sub = std::find(
            std::begin(p_subscribers),
            std::end(p_subscribers),
            &subscriber);
        if (sub == std::end(p_subscribers))
            return false;
        p_subscribers.erase(sub);
        return true;
    }
};

#endif // SPICE_OBSERVABLE
