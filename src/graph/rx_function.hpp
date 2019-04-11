#ifndef SPICE_RX_FUNCTION
#define SPICE_RX_FUNCTION

#include <type_traits>

#include "./observable.h"

template<typename arg_enum, typename function_t>
class _rx_function {
private:
    function_t p_function;
public:
    using args = arg_enum;

    template<typename ReturnT>
    void arg(arg_enum arg, ReturnT arg_val);

    template<typename ReturnT>
    friend _rx_function<arg_enum, function_t> operator>>(
        _rx_function<arg_enum, function_t> fn,
        ReturnT ret_val);
};

template<typename Callable, typename ... ArgTs>
class rx_function {
private:
    Callable p_function;
public:
    using ReturnT = observable<
        typename std::invoke_result<Callable, ArgTs...>::type>;

    template<typename ReturnT>
    friend rx_function<Callable, ArgTs...> operator>>(
        rx_function<Callable, ArgTs...> fn,
        ReturnT ret_val);
};

template<typename Callable>
constexpr auto make_rx(Callable fn) {

}

#endif // SPICE_RX_FUNCTION
