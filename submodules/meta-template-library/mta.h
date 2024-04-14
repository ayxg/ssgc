//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: meta-template-archive
// File: mta.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_META_TEMPLATE_ARCHIVE_MTA_H
#define HEADER_GUARD_CALE_META_TEMPLATE_ARCHIVE_MTA_H
//---------------------------------------------------------------------------//
// Brief: Meta Template Programming Archive Module
//---------------------------------------------------------------------------//
// Includes:
#include <concepts>
#include <functional>
#include <tuple>
#include <type_traits>
#include <variant>

#include "require.h"
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Namespace:{mta}
// Brief:{Meta Template Programming Archive}
//---------------------------------------------------------------------------//
namespace mta {
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Section:{Meta Methods}
// Brief:{These structs typedefs and methods are to be treated as meta methods.}
//---------------------------------------------------------------------------//

//---------------------------------------------------------//
// Template:{function_traits}
// Brief:{
//  Provides information about a function type.
//  eg.
//          using fnT = function_traits<function<int(float, double)>>;
//          fnT::nargs == 2
//          fnT::result_type == int
//          fnT::arg_type<0> == float
//          fnT::arg_type<1> == double
// }
//---------------------------------------------------------//
template <typename T>
struct function_traits;

template <typename R, typename... Args>
struct function_traits<std::function<R(Args...)>> {
  static constexpr inline size_t nargs = sizeof...(Args);
  using result_type = R;
  using args_tuple_type = std::tuple<Args...>;
  template <std::size_t N>
  using arg_type = typename std::tuple_element<N, args_tuple_type>::type;
};
//---------------------------------------------------------//
// EndTemplate:{function_traits}
//---------------------------------------------------------//

//---------------------------------------------------------//
// Template:{is_callable, iCallable}
// Brief:{
//  Checks if a type is a callable.
//  iCallable is the concept equivalent.
// }
//---------------------------------------------------------//
template <typename T, typename U = void>
struct is_callable {
  static bool const constexpr value =
      std::conditional_t<std::is_class<std::remove_reference_t<T>>::value,
                         is_callable<std::remove_reference_t<T>, int>,
                         std::false_type>::value;
};

template <typename T, typename U, typename... Args>
struct is_callable<T(Args...), U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T (*)(Args...), U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T (&)(Args...), U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args......), U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T (*)(Args......), U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T (&)(Args......), U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args...) const, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args...) volatile, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args...) const volatile, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args......) const, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args......) volatile, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args......) const volatile, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args...)&, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args...) const&, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args...) volatile&, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args...) const volatile&, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args......)&, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args......) const&, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args......) volatile&, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args......) const volatile&, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args...)&&, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args...) const&&, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args...) volatile&&, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args...) const volatile&&, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args......)&&, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args......) const&&, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args......) volatile&&, U> : std::true_type {};
template <typename T, typename U, typename... Args>
struct is_callable<T(Args......) const volatile&&, U> : std::true_type {};

template <typename T>
struct is_callable<T, int> {
 private:
  using YesType = char (&)[1];
  using NoType = char (&)[2];

  struct Fallback {
    void operator()();
  };

  struct Derived : T, Fallback {};

  template <typename U, U>
  struct Check;

  template <typename>
  static YesType Test(...);

  template <typename C>
  static NoType Test(Check<void (Fallback::*)(), &C::operator()>*);

 public:
  static bool const constexpr value =
      sizeof(Test<Derived>(0)) == sizeof(YesType);
};
template <typename T>
concept iCallable = is_callable<T>::value;

//---------------------------------------------------------//
// EndTemplate:{is_callable, iCallable}
//---------------------------------------------------------//

//---------------------------------------------------------//
// Template:{is_template_for}
// Brief:{
//  Checks if a type is a template instantiation of a given template.
//  eg.
//      is_template_for_v<std::vector, std::vector<int>> == true
// }
// Template Parameters:{
//		1. :
//		2. :
// }
//---------------------------------------------------------//
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
//---------------------------------------------------------//
// EndTemplate:{is_template_for}
//---------------------------------------------------------//

//---------------------------------------------------------//
// Template:{is_unique_type_in_pack}
// Brief:{
//  Checks all the types in a pack are unique.
//  eg. See index_of_type_in_tuple implementation.
// }
//---------------------------------------------------------//
template <typename...>
inline constexpr auto is_unique_pack = std::true_type{};

template <typename T, typename... Rest>
inline constexpr auto is_unique_pack<T, Rest...> =
    std::bool_constant<(!std::is_same_v<T, Rest> && ...) &&
                       is_unique_pack<Rest...>>{};

static_assert(is_unique_pack<double, int, char>,
              "[is_unique_pack] Implementation Failure.");
//---------------------------------------------------------//
// EndTemplate:{is_unique_type_in_pack}
//---------------------------------------------------------//

//-----------------------------------//
// Method:{index_of_type_in_tuple}
// Brief:{
//  eg. index_of_type_in_tuple<int, std::tuple<int, float, double>> == 0
// }
//-----------------------------------//
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

template <typename T, typename U, typename... Us>
constexpr auto index_of_type_in_tuple(std::tuple<U, Us...>&&) {
  static_assert(is_unique_pack<U, Us...>,
                "[index_of_type_in_tuple] Should only be called on tuples with "
                "unique types.");
  return index_of_type_in_tuple_impl<T, U, Us...>();
}

static_assert(index_of_type_in_tuple<float>(std::tuple<int, float, double>()) ==
                  1,
              "[index_of_type_in_tuple] Implementation Failure.");
//-----------------------------------//
// EndMethod:{index_of_type_in_tuple}
//-----------------------------------//

//---------------------------------------------------------//
// Template:{tuple_to_variant}
// Brief:{
//  Converts a tuple to a variant at compile time.
// }
//---------------------------------------------------------//
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
//---------------------------------------------------------//
// EndTemplate:{tuple_to_variant}
//---------------------------------------------------------//

//---------------------------------------------------------------------------//
// EndSection:{Meta Methods}
//---------------------------------------------------------------------------//

//======================================================================================//
// Meta Structures:
// These structs typedefs and methods are to be treated as meta structures.
//======================================================================================//

//---------------------------------------------------------//
// Template:{compile_time_type_index_list}
// Brief:{
//  !!TODO: add brief
// }
//---------------------------------------------------------//
template <class... TypeTs>
struct compile_time_type_index_list {
  static constexpr inline size_t kNumTypes = sizeof...(TypeTs);
  using types_tuple = std::tuple<TypeTs...>;
  using types_variant = std::variant<TypeTs...>;
  //using types_variant = decltype(tuple_to_variant(types_tuple{}));
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
//---------------------------------------------------------//
// EndTemplate:{compile_time_type_index_list}
//---------------------------------------------------------//

//---------------------------------------------------------//
// Template:{string_constant , character_constant , null_string_constant}
// Brief:{
//  Constant evaluated string constants. Passable as template parameters.
//  Utilizes new C++20 lambda features to allow passing a string as a
//  template parameter to the string_constant struct. constexpr const char
//  * value is then used to access the string value.
// }
// Example: {
//  string_constant<[]() consteval { return "Hello, World!"; }>
//  hello_world; static_assert(hello_world.value == "Hello, World!");
//
// Define a STRING_CONSTANT macro to make it easier to define.
//  - Always #UNDEFINE AFTER!.
//  - Always provide description of the macro.
//  - Always use the macro in the following format:
/*

//-----------------------------------//
// Macro:{STRING_CONSTANT}
// Brief:{
//  Creates a string_constant from a string literal.
//  Access string value with string_constant<>::value.
//  Undef after use!
//      #undef STRING_CONSTANT
// }
//-----------------------------------//
#define STRING_CONSTANT(STR) mta::string_constant<[]() consteval { return STR;
}>
//-----------------------------------//
// EndMacro:{STRING_CONSTANT}
//-----------------------------------//

*/
//
//      STRING_CONSTANT("Hello, World!") hello_world;
// Undef after!
//      #undef STRING_CONSTANT
// }
//---------------------------------------------------------//
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
//---------------------------------------------------------//
// EndTemplate:{string_constant , character_constant , null_string_constant}
//---------------------------------------------------------//



// helper constant for the visitor #3
template <class>
inline constexpr bool always_false_v = false;

// helper type for the visitor #4
template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

template <class HostT, class... CallableTs>
constexpr inline decltype(auto) visit_overloaded(HostT&& host,
                                                 CallableTs&&... callables) {
  return std::visit(overloaded{std::forward<CallableTs>(callables)...},
                    std::forward<HostT>(host));
}



template <typename... Args>
consteval bool are_unique(Args&&... args) {
  std::array arr{std::forward<Args>(args)...};
  std::sort(arr.begin(), arr.end());
  return std::adjacent_find(arr.begin(), arr.end()) == arr.end();
}



//---------------------------------------------------------------------------//
};  // namespace mta
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
//
// Licensed under the Apache License, Version 2.0(the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: meta-template-archive
// File: mta.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_META_TEMPLATE_ARCHIVE_MTA_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
