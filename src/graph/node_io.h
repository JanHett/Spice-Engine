#ifndef SPICE_NODE_IO
#define SPICE_NODE_IO

#include <string>
#include <memory>
#include <vector>

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

	void connect(std::weak_ptr<Output<T>> source) {
		const auto source_sp = source.lock();
		_connect(source_sp);
		source_sp->_connect(this->shared_from_this());
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

	void connect(std::weak_ptr<Input<T>> consumer) {
		const auto consumer_sptr = consumer.lock();
		_connect(consumer_sptr);
		consumer_sptr->_connect(this->shared_from_this());
	}
};

#endif // SPICE_NODE_IO
