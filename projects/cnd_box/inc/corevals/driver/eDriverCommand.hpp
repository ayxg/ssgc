///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_compiler_corevals
/// @brief eClCliFlag enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "corevals/reflected_meta_enum.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::driver {
#define CND_MM_AENUM_ENTRY(s, p, m) p m(s)

/// Define applied enum cnd::corevals::diagnostic::eDriverCommand
#define CND_APPLIED_ENUM_eDriverCommand(m, s, p, l)  \
  CND_MM_AENUM_ENTRY(INVALID, p, m)                  \
  CND_MM_AENUM_ENTRY(NONE, s, m)                     \
  CND_MM_AENUM_ENTRY(Build, s, m)                 \
  CND_MM_AENUM_ENTRY(Dev, s, m)                  \
  CND_MM_AENUM_ENTRY(Help, s, m)                 \
  CND_MM_AENUM_ENTRY(Version, s, m)              \
  l

// Define the enum.
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eDriverCommand, eDriverCommand);

// Define the enum to cstr conversion.
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eDriverCommandToCStr, eDriverCommand, eDriverCommand);

// Assert enum to cstr conversion.
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eDriverCommand(x) \
  CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eDriverCommand, eDriverCommandToCStr)

CND_APPLIED_ENUM_eDriverCommand(CND_STATIC_ASSERT_ENUM_TO_CSTR_eDriverCommand, , , );
static_assert(cxx::StrEq(eDriverCommandToCStr(eDriverCommand::COUNT), "COUNT"));

#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eDriverCommand
}  // namespace cnd::corevals::driver

/// @} // end of cnd_compiler_corevals

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////