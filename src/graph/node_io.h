#ifndef SPICE_NODE_IO
#define SPICE_NODE_IO

#include <string>
#include <memory>
#include <vector>

template <class T>
class Input {
public:
	std::string label;
	std::shared_ptr<T> data;
	std::weak_ptr<T> provider;
};

template <class T>
class Output {
public:
	std::string label;
	std::shared_ptr<T> data;
	std::vector<std::shared_ptr<T>> consumers;
};

#endif // SPICE_NODE_IO
