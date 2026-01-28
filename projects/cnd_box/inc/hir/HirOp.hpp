///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2026 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler
/// @{
#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "hir/AnyValue.hpp"
// clang-format on

namespace cnd::hir {

enum class eContextState {
  kExecute,
  kPause,
  kExit
};

enum class eHirOp : int { 
  kAdd,   // LHS<slot> RHS<slot> OUT<slot>
  kJump  // TO<label>
};

struct HirOp;
struct Environment;
struct Context;
struct Slot;
struct Label;

struct Environment {
  Environment* root{nullptr};
  std::list<Environment> branches{};
  std::vector<AV> data;
  std::vector<HirOp> ops;
};

struct Context {
  eContextState state{eContextState::kPause};
  Environment root{};
  Environment* curr_env{nullptr};
  Environment* prev_env{nullptr};
  std::vector<HirOp>::iterator curr_line;
  std::vector<HirOp>::iterator prev_line;
  
  void ExecuteNext();
};

struct Slot {
  Environment* env;
  std::size_t index;

  std::vector<AV>::iterator Get() { return env->data.begin() + index; }
};

struct Label {
  Environment* env;
  std::size_t index;
  std::vector<HirOp>::iterator Get() { return env->ops.begin() + index; }
};

struct HirOp {
  eHirOp op;
  std::vector<std::variant<Slot, Label>> args;

  void Execute(Context& ctx) {
    switch (op) {
      case eHirOp::kAdd:
        *std::get<Slot>(args[2]).Get() = AV::Add(*std::get<Slot>(args[0]).Get(), *std::get<Slot>(args[1]).Get());
      case eHirOp::kJump:
        ctx.curr_line = std::get<Label>(args[0]).Get();
        
    }
  }
};

void Context::ExecuteNext() { 
  prev_line = curr_line;
  curr_line++;
  curr_line->Execute(*this);
}

}  // namespace cnd::hir