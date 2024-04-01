#pragma once
#include <concepts>
#include <tuple>
#include <type_traits>
#include <variant>

#include "require.h"
//======================================================================================//
// Meta Methods:
// These structs typedefs and methods are to be treated as meta functions.
//======================================================================================//

//////////////////////////////////////////////////////////////////////////////////////////
// is_template_for
// Checks if a type is a template instantiation of a given template.
// Usage: is_template_for_v<std::vector, std::vector<int>> == true
//////////////////////////////////////////////////////////////////////////////////////////
template <template <typename...> typename tmpl, typename T>
struct is_template_for_impl : public std::false_type {};

template <template <typename...> typename tmpl, typename... Args>
struct is_template_for_impl<tmpl, tmpl<Args...>> : public std::true_type {};

template <template <typename...> typename tmpl, typename... Ts>
using is_template_for =
    std::conjunction<is_template_for_impl<tmpl, std::decay_t<Ts>>...>;

template <template <typename...> typename tmpl, typename... Ts>
constexpr bool is_template_for_v = is_template_for<tmpl, Ts...>::value;

static_assert(is_template_for_v<std::tuple, std::tuple<int, double>>,
              "[is_template_for] Implementation Failure.");

//////////////////////////////////////////////////////////////////////////////////////////
// is_unique_type_in_pack
// Checks all the types in a pack are unique.
// eg. See index_of_type_in_tuple implementation.
//////////////////////////////////////////////////////////////////////////////////////////
template <typename...>
inline constexpr auto is_unique_pack = std::true_type{};

template <typename T, typename... Rest>
inline constexpr auto is_unique_pack<T, Rest...> =
    std::bool_constant<(!std::is_same_v<T, Rest> && ...) &&
                       is_unique_pack<Rest...>>{};

static_assert(is_unique_pack<double, int, char>,
              "[is_unique_pack] Implementation Failure.");

//////////////////////////////////////////////////////////////////////////////////////////
// index_of_type_in_tuple
// Returns the index of a type in a tuple.
// eg. index_of_type_in_tuple<int, std::tuple<int, float, double>> == 0
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

template <typename T, typename U, typename... Us>
constexpr auto index_of_type_in_tuple(const std::tuple<U, Us...>&) {
  static_assert(is_unique_pack<U, Us...>,
                "[index_of_type_in_tuple] Should only be called on tuples with "
                "unique types.");
  return index_of_type_in_tuple_impl<T, U, Us...>();
}

static_assert(index_of_type_in_tuple<float>(std::tuple<int, float, double>()) ==
                  1,
              "[index_of_type_in_tuple] Implementation Failure.");

//////////////////////////////////////////////////////////////////////////////////////////
// tuple_to_variant
// Converts a tuple to a variant at compile time.
//////////////////////////////////////////////////////////////////////////////////////////
// Base case
template <typename... Ts>
constexpr std::variant<Ts...> tuple_to_variant(const std::tuple<Ts...>& tuple) {
  return std::get<0>(tuple);  // Return the first element of the tuple
}
// Recursive case
template <typename T, typename... Ts>
constexpr std::variant<T, Ts...> tuple_to_variant(
    const std::tuple<T, Ts...>& tuple) {
  return std::variant<T, Ts...>(std::get<0>(tuple));
}

static_assert(std::is_same_v<
                  std::variant<int, float, double>,
                  decltype(tuple_to_variant(std::tuple<int, float, double>()))>,
              "[tuple_to_variant] Implementation Failure.");

//======================================================================================//
// End Meta Methods:
//======================================================================================//


//======================================================================================//
// Meta Structures:
// These structs typedefs and methods are to be treated as meta structures.
//======================================================================================//

//////////////////////////////////////////////////////////////////////////////////////////
// compile_time_type_index_list
// Converts a tuple to a variant at compile time.
//////////////////////////////////////////////////////////////////////////////////////////
template <class... TypeTs>
struct compile_time_type_index_list {
  static constexpr inline size_t kNumTypes = sizeof...(TypeTs);
  using types_tuple = std::tuple<TypeTs...>;
  using types_variant = decltype(tuple_to_variant(types_tuple{}));

  // Type of element at index.
  template <std::size_t index>
  using type_of = typename std::tuple_element_t<index, types_tuple>;

  // Helps when needing to pass a type as a template parameter.
  template <class T>
  struct IndexOf {
    using type = T;
    static constexpr std::size_t idx = index_of_type_in_tuple<T>(types_tuple{});
  };

  // Index of a type in the list.
  template <class T>
  constexpr static std::size_t index_of() {
    return IndexOf<T>::idx;
  }
};

static_assert(compile_time_type_index_list<int, float, double>::kNumTypes == 3,
              "[compile_time_type_list] Implementation Failure.");
static_assert(
    compile_time_type_index_list<int, float, double>::index_of<float>() == 1,
    "[compile_time_type_list] Implementation Failure.");
static_assert(
    std::is_same_v<compile_time_type_index_list<int, float, double>::type_of<2>,
                   double>,
    "[compile_time_type_list] Implementation Failure.");

//////////////////////////////////////////////////////////////////////////////////////////
// string_constant , character_constant , null_string_constant
// Brief: Constant evaluated string constants. Passable as template parameters.
//        Utilizes new C++20 lambda features to allow passing a string as a
//        template parameter to the string_constant struct. constexpr const char
//        * value is then used to access the string value.
// Example:
//       string_constant<[]() consteval { return "Hello, World!"; }>
//       hello_world; static_assert(hello_world.value == "Hello, World!");
//
// Define a STRING_CONSTANT macro to make it easier to define.
//    - Always #UNDEFINE AFTER!.
//    - Always provide description of the macro.
//    - Always use the macro in the following format:
// // Macro: STRING_CONSTANT
// // Description: Creates a string_constant from a string literal.
// //              Access string value with string_constant<>::value.
//    #define STRING_CONSTANT(STR) string_constant<[]() consteval { return STR;
//    }>
//
//    STRING_CONSTANT("Hello, World!") hello_world;
// Undef after!
//    #undef STRING_CONSTANT
//////////////////////////////////////////////////////////////////////////////////////////
template <char CHAR>
using character_constant = std::integral_constant<char, CHAR>;

template <auto STR>
struct string_constant {
  static constexpr const char* value = STR();
  constexpr bool operator==(const string_constant& other) const {
    return value == other.value;
  }
};

using null_string_constant = string_constant<[]() consteval { return ""; }>;
// Macro: STRING_CONSTANT
// Description: Creates a string_constant from a string literal.
//              Access string value with string_constant<>::value.
#define STRING_CONSTANT(STR) string_constant<[]() consteval { return STR; }>
#undef STRING_CONSTANT