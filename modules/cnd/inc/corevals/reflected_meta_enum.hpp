///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file Meta-Macros for Applied Enum Definitions
/// @ingroup cnd_corevals
/// Macros for enum generation and forward reflection of their enum entries.
///
/// Large enums which are contiguous may be generated using these macros. The forward reflection works by requiring the
/// enum to be defined in a specific way. In short, all enum entries must be defined in an 'applied macro list' which
/// we will call 'applied list'.
///
/// An applied list will have an optional prefix and apply a macro to each element in the list followed by a separator.
/// The last element will have no separator, ends in 'lst'. An example of an applied list:
/// @code
///   #define EXAMPLE_APPLIED_LIST(m, sep, pre, lst)\
///     pre m(Foo)\
///     sep m(Bar)\
///     sep m(Grok) lst
/// @endcode
///
/// @note Since there is currently no reflection support, this seems like the most sane method to associate identifiers
///     with enums and convert them to strings, or other symbols. I don't know the specific name of this pattern but
///     I'm sure wiser devs have already thought of it.
/// 
/// Implementation Limits:
///   - The applied list cannot have defined values, only identifiers.
///   - The enums are all contiguous.
///   - The enums all end in a COUNT enumeration entry.
/// 
/// Given this is a macro-based implementation, defining enums should follow the following instructions verbatim:
/// 
/// 1.Define an applied list of all the enum entries in the following format where [] is the substitute pattern.
/// 
///   @code
///     #define CND_APPLIED_ENUM_LIST_[name-of-enum](m, sep, pre, lst)\
///       pre m([first-enum-entry|'INVALID'])\
///       sep m([middle-enum-entry|'NONE'])\
///       sep m([last-enum-entry])\
///       lst
///   @endcode
/// 
///   [name-of-enum] : By C& compiler codebase coding guidelines define enums in CamelCase prefixed with an 'e'.
///   [first-enum-entry] : The first entry in the applied list. By convention the first entry is 'INVALID'.
///   [middle-enum-entry] : All entries except the first and last. By convention the second entry is 'NONE'.
///   [last-enum-entry] : The last entry in the applied list.
///   After defining a correct applied list most of the hard work is done. We can use macros to generate the rest.
///   @note Generative macros designed to be called are prefixed with 'CND_MM_'(meta-macro).
///
/// 2.Create an enum from an applied list by calling CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM(ln, en, et) or
///   CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(ln, en) - based on wether you need a typed of untyped enum.
///   This will generate an enum with the name 'en' and the entries from the applied list prefixed with 'k'.
///   A 'COUNT' entry is always added to the end of the list and is used to 'reflect' the entry count, consider it an
///   "out of bounds" entry.. !the 'COUNT' entry is not prefixed with a k.
///   @see CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM
///   @note C++ Core Guidelines suggest using untyped enums unless you have a good reason to use a typed enum.
/// 
/// 3.Create an enum name to c-string conversion method by calling CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(fn, ln, en).
///   @see CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST
/// 
/// 4.Static assert the enum to string method to ensure all entries are covered by calling CND_STATIC_ASSERT_ENUM_TO_CSTR.
///   Use the following format where [] is the substitute pattern:
/// 
///   @code
///     #define CND_STATIC_ASSERT_ENUM_TO_CSTR_[enum-name](x) CND_STATIC_ASSERT_ENUM_TO_CSTR(x, [enum-name], [fn-name])
///     CND_APPLIED_ENUM_[enum-name](CND_STATIC_ASSERT_ENUM_TO_CSTR_[enum-name], , , ); // test the entries
///     static_assert(cxx::StrEq([fn-name]([enum-name]::COUNT), "COUNT"));              // test the 'COUNT' entry
///     #undef CND_STATIC_ASSERT_ENUM_TO_CSTR_[enum-name]                               // undefine temporary macro
///   @endcode
/// 
/// BONUS: Applied lists may be nested within each other, this can be useful for defining related enums or categorizing
/// entries. The following example would make all the entries of 'eBar' part of 'eFoo' excluding the 'COUNT' entry:
/// 
/// @code
/// #define CND_APPLIED_ENUM_LIST_eFoo(m, sep, pre, lst)\
///     CND_APPLIED_ENUM_LIST_eBar(m, sep, pre, )\
///     sep m(Grok) lst
/// @endcode
/// 
/// To keep it clean and future-proof the codebase - define each enum in its own file. The header should only depend on
/// the 'ccapi' module. The header should only define the enum, and methods which query syntactical properties of the 
/// enum. DO NOT include logic related methods. Name the file in the format: 'cnd_corevals_[enum-name-lowercase].hpp'.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_corevals
/// @{
#pragma once

// clang-format off
#include "use_ccapi.hpp"

// Some local utility macros to perform all the dark magic.
#define CND_MM_EXPAND(x) x          ///> @warning Internal macro, do not use.
#define CND_MM_COMMA ,              ///> @warning Internal macro, do not use.
#define CND_MM_IDTOSTR(x) #x        ///> @warning Internal macro, do not use.

/// Used to expand an enum name to its enum entry name, format: k##x
/// @warning Internal macro, do not use.
#define CND_MM_CONST_ID_TAG(x) k##x 

/// Creates a typed enum from an applied list. Adds a COUNT entry at the end.
/// @pre 'COUNT' must NOT be an identifier in the applied list.
/// @param ln Applied list macro name.
/// @param en Enum name. Fully qualified or call macro inside the namespace.
/// @param et Enum type. Underlying type of the enum. Must be provided.
#define CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM(ln, en, et) \
  enum class en : et { CND_APPLIED_ENUM_##ln(CND_MM_CONST_ID_TAG, CND_MM_COMMA, , CND_MM_COMMA COUNT) }

/// Creates an enum from an applied list. Adds a COUNT entry at the end.
/// @pre 'COUNT' must NOT be an identifier in the applied list.
/// @param ln Applied list macro name.
/// @param en Enum name. Fully qualified or call macro inside the namespace.
#define CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(ln, en) \
  enum class en { CND_APPLIED_ENUM_##ln(CND_MM_CONST_ID_TAG, CND_MM_COMMA, , CND_MM_COMMA COUNT) }

/// Creates a switch case for a macro identifier to string conversion.
/// @warning Temporary macro definition.
/// @warning Defined ONLY for use in the CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST macro.
#define CND_MM_IDENT_TO_CSTR_SWITCH_CASE(x) \
  case CND_MM_CONST_ID_TAG(x):              \
    return CND_MM_IDTOSTR(k##x);

/// Creates a method to convert an enum to a string given an applied list. Adds a case for 'COUNT'. Default case returns
/// an "<invalid>" string (which should never happen, but can be used to detect implementation errors).
/// @pre Enum must be defined using CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM
/// @param fn : Function name to be defined.
/// @param ln : Applied list name, make sure to include namespace in the name.
/// @param en : Enum name with full namespace from the global ns, or call macro
///             inside the ns.
#define CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(fn, ln, en)                  \
  constexpr const char* fn(en e) noexcept {                                  \
    using enum en;                                                           \
    switch (e) {                                                             \
      CND_APPLIED_ENUM_##ln(CND_MM_IDENT_TO_CSTR_SWITCH_CASE, , , case COUNT \
                            : return "COUNT";) default : return "<invalid>"; \
    }                                                                        \
  }

/// Applied Enum Meta Macro : Asserts that all entries are covered by the conversion method.
/// @pre Enum must be defined using CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM
/// 
/// Use the following format where [] is the substitute pattern:
/// @code
///   #define CND_STATIC_ASSERT_ENUM_TO_CSTR_[enum-name](x) \
///     CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x,[enum-name],[fn-name])
///
///   CND_APPLIED_ENUM_[enum-name](CND_STATIC_ASSERT_ENUM_TO_CSTR_[enum-name], , , );
///   static_assert(cxx::StrEq([fn-name]([enum-name]::COUNT), "COUNT"));
///   #undef CND_STATIC_ASSERT_ENUM_TO_CSTR_[enum-name]
/// @endcode
#define CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x,en,fn)\
    static_assert(cxx::StrEq(fn(en::k##x), CND_MM_IDTOSTR(k##x)));

// clang-format on

/// @} // end of cnd_corevals

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// This program is free software : you can redistribute it and / or modify it
// under the terms of the GNU Affero General Public License as published by the
// Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////