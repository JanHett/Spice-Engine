#ifndef SPICE_RX_FUNCTION
#define SPICE_RX_FUNCTION

#include <type_traits>
#include <tuple>
#include <optional>
#include <variant>

#include "./observable.h"

struct key_not_found_t {};

template<typename K, typename ... Vs>
class tuple_map {
private:
    std::tuple<
        std::pair<std::optional<K>, key_not_found_t>,
        std::pair<std::optional<K>, std::optional<Vs>>...> p_map;

    template<
        size_t Idx = 1,
        typename std::enable_if<
            Idx < std::tuple_size<decltype(p_map)>::value, int>::type = 0>
    constexpr std::variant<key_not_found_t, std::optional<Vs>...> operator_subscript_impl_ (
        const K & key)
    {
        if (std::get<Idx>(p_map).first &&
            (*std::get<Idx>(p_map).first) == key)
            return std::get<Idx>(p_map).second;
        return operator_subscript_impl_<Idx + 1>(key);
    }

    template<
        size_t Idx,
        typename std::enable_if<
            !(Idx < std::tuple_size<decltype(p_map)>::value), int>::type = 0>
    constexpr key_not_found_t operator_subscript_impl_ (
        const K & key)
    {
        return std::get<0>(p_map).second;
    }
public:
    tuple_map(): p_map({
        {{}, key_not_found_t()},
        {{}, std::optional<Vs>{}}...}) {}

    tuple_map(std::pair<K, Vs> ... values): p_map({
        {{}, key_not_found_t()},
        {{values.first}, std::optional<Vs>{values.second}}...}) {}

    constexpr auto operator[] (
        const K & key)
    {
        auto val = operator_subscript_impl_(key);
        return std::get<val.index>(val);
    }

    // constexpr auto operator[] (const K & key) {
    //     for (constexpr size_t idx = 1; idx < std::tuple_size<decltype(p_map)>::value; ++idx) {
    //         if (std::get<idx>(p_map).first && *std::get<idx>(p_map).first == key)
    //             return std::get<idx>(p_map).second;
    //     }
    //     return std::get<0>(p_map).second;
    // }

    // constexpr size_t index_of_key(const K & key) {
    //     for (size_t idx = 1; idx < std::tuple_size<decltype(p_map)>::value; ++idx) {
    //         if (std::get<idx>(p_map).first && *std::get<idx>(p_map).first == key)
    //             return idx;
    //     }

    //     // TODO: TERMINATE IN CASE IT'S NOT FOUND!
    // }

    constexpr const auto data() const {
        return p_map;
    }

    constexpr const auto size() const {
        return std::tuple_size<decltype(p_map)>::value;
    }
};

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
