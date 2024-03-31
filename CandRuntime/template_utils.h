#pragma once
#include <concepts>
#include <tuple>
#include <type_traits>
//////////////////////////////////////////////////////////////////////////////////////////
// is_template_for
//////////////////////////////////////////////////////////////////////////////////////////
template <template <typename...> typename tmpl, typename T>
struct is_template_for_impl : public std::false_type {};

template <template <typename...> typename tmpl, typename... Args>
struct is_template_for_impl<tmpl, tmpl<Args...>> : public std::true_type {};

template <template <typename...> typename tmpl, typename... Ts>
using is_template_for =
    std::conjunction<is_template_for_impl<tmpl, std::decay_t<Ts>>...>;

// Checks if a type is a template instantiation of a given template.
// Usage: is_template_for_v<std::vector, std::vector<int>> == true
template <template <typename...> typename tmpl, typename... Ts>
constexpr bool is_template_for_v = is_template_for<tmpl, Ts...>::value;

//////////////////////////////////////////////////////////////////////////////////////////
// is_unique_type_in_pack
//////////////////////////////////////////////////////////////////////////////////////////
template <typename...>
inline constexpr auto is_unique_type_in_pack = std::true_type{};

// Checks if a type is unique in a template parameter pack.
// eg. See index_of_type_in_tuple implementation.
template <typename T, typename... Rest>
inline constexpr auto is_unique_type_in_pack<T, Rest...> =
    std::bool_constant<(!std::is_same_v<T, Rest> && ...) &&
                       is_unique_type_in_tuple<Rest...>>{};

//////////////////////////////////////////////////////////////////////////////////////////
// index_of_type_in_tuple
//////////////////////////////////////////////////////////////////////////////////////////
template <typename T, typename U, typename... Us>
constexpr auto index_of_type_in_tuple_impl() {
  if constexpr (std::is_same<T, U>::value) {
    return 0;
  } else {
    static_assert(
        sizeof...(Us) > 0,
        "index_of_type_in_tuple: This tuple does not contain requested type");
    return 1 + index_of_type_in_tuple_impl<T, Us...>();
  }
}

// Returns the index of a type in a tuple.
// eg. index_of_type_in_tuple<int, std::tuple<int, float, double>> == 0
template <typename T, typename U, typename... Us>
constexpr auto index_of_type_in_tuple(const std::tuple<U, Us...>&) {
  static_assert(is_unique_type_in_tuple<U, Us...>,
                "[index_of_type_in_tuple] Should only be called on tuples with "
                "unique types.");
  return index_of_type_in_tuple_impl<T, U, Us...>();
}