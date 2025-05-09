///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
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

/// @addtogroup cnd_compiler_data
/// @{
#pragma once
#include "cnd_tk_scope.hpp"

// clang-format off
#include "cnd_implicit.hpp"
#include "cnd_constdef.hpp"
#include "cnd_clerr.hpp"
#include "cnd_tk.hpp"
#include "cnd_tk_cursor.hpp"
// clang-format on

#define CND_CPP_SRCLOC std::source_location::current()
#define CND_CPP_SRCLOC_FORMAT                                                                         \
  std::format("[{}][{}][{}][{}]", Path(CND_CPP_SRCLOC.file_name()).filename(), CND_CPP_SRCLOC.line(), \
              CND_CPP_SRCLOC.column(), __FUNCSIG__)

#define CND_ENABLE_COMPILER_DEBUG_DIAGNOSTICS 1
#ifdef CND_ENABLE_COMPILER_DEBUG_DIAGNOSTICS
#if CND_ENABLE_COMPILER_DEBUG_DIAGNOSTICS == 1
#define CND_CLDBG_SRCLOC CND_CPP_SRCLOC_FORMAT
#else
#define CND_CLDBG_SRCLOC Str("")
#endif
#endif  // CND_ENABLE_COMPILER_DEBUG_DIAGNOSTICS

namespace cnd {

clerr::ClRes<TkCursor> FindScope(const TkCursor& crsr) {
  int scope_depth = 0;
  std::stack<eTk> scope_history;
  for (auto c = crsr.Next(); c.Iter() != crsr.End(); c.Advance())
    if (c.IsOpeningScope()) {
      scope_depth++;
      scope_history.push(c.Type());
    } else if (c.IsClosingScope()) {
      if (scope_history.empty())
        if (c.IsClosingScopeOf(crsr.Type()))
          return crsr;  // Good end of scope
        else
          ;  // Fail

      if (!crsr.IsClosingScopeOf(scope_history.top())) return crsr;  // fail

      scope_history.pop();
      scope_depth--;
    }
}

clerr::ClRes<TkCursor> FindParen(const TkCursor& crsr) {
  using namespace clerr;
  using enum eClErr;

  // TkCursor last_open = {begin, end};
  auto last_closed = begin;
  auto error_last_closed = begin;

  // Out of bounds begin passed to method...
  if (crsr.AtEnd()) return Fail(MakeClErr<kParserExpectedOpeningScope>(crsr.Get()));
  // No open list token to start with.
  if (!crsr.TypeIs(eTk::kLParen)) return Fail(MakeClErr<kParserExpectedOpeningScope>(crsr.Get()));

  // First token after the opening scope token.
  auto contained_begin = crsr.Next();

  // End right after open, cannot be closed.
  if (contained_begin.AtEnd()) return Fail(MakeClErr<kParserOpeningScopeAtEof>(crsr.Get()));
  // Empty paren scope '()'
  if (contained_begin.TypeIs(eTk::kRParen)) return TkCursor{crsr.Iter(), contained_begin.Next().Iter()};

  // Find the last matching close token that is not within a () [] or {} scope.
  // Start from the token after the open token. All scope depths should be 0
  // at the end of the loop on a valid scope.
  Size paren_scope_depth = 0;
  Size frame_scope_depth = 0;
  Size list_scope_depth = 0;
  std::stack<eTk> scope_type_history;
  for (auto tk_it = contained_begin.Begin(); tk_it != crsr.End(); tk_it++) {
    // Open Paren
    if (tk_it->TypeIs(eTk::kLParen)) {
      paren_scope_depth++;
      scope_type_history.push(eTk::kLParen);
    }
    // Close Paren
    else if (tk_it->TypeIs(eTk::kRParen)) {
      // No open scopes left to close, this is the final right paren. End of scope -> break
      if (scope_type_history.empty()) {
        last_closed = tk_it;
        break;
      }
      if (scope_type_history.top() != eTk::kLParen)
        return Fail(MakeClErr<kParserClosingScopeBeforeOpen>(crsr.Get(), eTk::kLParen));
      scope_type_history.pop();
      paren_scope_depth--;
    }
    // Open Bracket
    else if (tk_it->TypeIs(eTk::kLBracket)) {
      frame_scope_depth++;
      scope_type_history.push(eTk::kLBracket);
    }
    // Close Bracket
    else if (tk_it->TypeIs(eTk::kRBracket)) {
      if (scope_type_history.empty()) return Fail(MakeClErr<kParserClosingScopeBeforeOpen>(crsr.Get(), eTk::kLBracket));
      if (scope_type_history.top() != eTk::kLBracket)
        return Fail(MakeClErr<kParserClosingScopeBeforeOpen>(crsr.Get(), eTk::kLBracket));
      scope_type_history.pop();
      frame_scope_depth--;
    }
    // Open Brace
    else if (tk_it->TypeIs(eTk::kLBrace)) {
      list_scope_depth++;
      scope_type_history.push(eTk::kLBrace);
    }
    // Close Brace
    else if (tk_it->TypeIs(eTk::kRBrace)) {
      if (scope_type_history.empty()) return Fail(MakeClErr<kParserClosingScopeBeforeOpen>(crsr.Get(), eTk::kLBrace));

      if (scope_type_history.top() != eTk::kLBrace)
        return Fail(MakeClErr<kParserClosingScopeBeforeOpen>(crsr.Get(), eTk::kLBrace));
      scope_type_history.pop();
      list_scope_depth--;
    }  // end if
    error_last_closed = tk_it;
  }  // end for

  if (paren_scope_depth != 0) return Fail(MakeClErr<kParserMismatchedParenScope>(crsr.Get(), *last_closed));

  if (frame_scope_depth != 0) return Fail(MakeClErr<kParserMismatchedBracketScope>(crsr.Get(), *last_closed));

  if (list_scope_depth != 0) return Fail(MakeClErr<kParserMismatchedBraceScope>(crsr.Get(), *last_closed));

  if (last_closed == begin) return Fail(MakeClErr<kParserMismatchedParenScope>(crsr.Get(), *error_last_closed));

  return TkCursor{crsr.Iter(), last_closed + 1};
}  // end find_paren

CND_CX TkScope TkScope::FindParen(TkCursor crsr) { return FindParen(crsr.Iter(), crsr.End()); }

CND_CX TkScope TkScope::FindBrace(TkVecConstIterT begin, TkVecConstIterT end) {
  auto paren_scope_depth = 0;
  auto frame_scope_depth = 0;
  auto list_scope_depth = 0;
  std::stack<eTk> scope_type_history;
  TkCursor last_open = {begin, end};
  TkVecConstIterT last_closed = begin;
  TkVecConstIterT error_last_closed = begin;
  if (last_open.AtEnd()) {  // Open is at end therefore cannot be closed.
    auto failure = TkScope{false, begin, end};
    failure.error_message_ = "find_brace: Open token is at end of token Vec.";
    return failure;
  }

  if (!last_open.TypeIs(eTk::kLBrace)) {  // No open list token to start with.
    auto failure = TkScope{false, begin, end};
    failure.error_message_ = "find_brace: Open token is not an open list token.";
    return failure;
  }

  if (last_open.Next().AtEnd()) {  // End right after open, cannot be closed.
    auto failure = TkScope{false, begin, end};
    failure.error_message_ = "find_brace: Open token is at end of token Vec.";
    return failure;
  }

  if (last_open.Next().TypeIs(eTk::kRBrace)) {  // Empty list
    return TkScope{true, begin, last_open.Next(2).Iter()};
  }

  // find the last matching close token that is not within a () [] or {}
  // scope, if there is no matching close token, return false
  for (auto tk_it = (begin + 1); tk_it < end; tk_it++) {
    if (tk_it->TypeIs(eTk::kLParen)) {
      paren_scope_depth++;
      scope_type_history.push(eTk::kLParen);
    } else if (tk_it->TypeIs(eTk::kRParen)) {
      if (scope_type_history.empty()) {
        auto failure = TkScope{false, begin, end};
        failure.error_message_ = "find_brace: Close token ')' without open token '('.";
        return failure;
      }

      if (scope_type_history.top() != eTk::kLParen) {
        auto failure = TkScope{false, begin, end};
        failure.error_message_ = "find_brace: Close token ')' without open token '('.";
        return failure;
      }
      scope_type_history.pop();
      paren_scope_depth--;
    } else if (tk_it->TypeIs(eTk::kLBracket)) {
      frame_scope_depth++;
      scope_type_history.push(eTk::kLBracket);
    } else if (tk_it->TypeIs(eTk::kRBracket)) {
      if (scope_type_history.empty()) {
        auto failure = TkScope{false, begin, end};
        failure.error_message_ = "find_brace: Close token ']' without open token '['.";
        return failure;
      }

      if (scope_type_history.top() != eTk::kLBracket) {
        auto failure = TkScope{false, begin, end};
        failure.error_message_ = "find_brace: Close token ']' without open token '['.";
        return failure;
      }
      scope_type_history.pop();
      frame_scope_depth--;
    } else if (tk_it->TypeIs(eTk::kLBrace)) {
      list_scope_depth++;
      scope_type_history.push(eTk::kLBrace);
    } else if (tk_it->TypeIs(eTk::kRBrace)) {
      if (scope_type_history.empty()) {
        last_closed = tk_it;
        break;  // This is the end of the scope
      }

      if (scope_type_history.top() != eTk::kLBrace) {
        auto failure = TkScope{false, begin, end};
        failure.error_message_ = "find_brace: Close token '}' without open token '{'.";
        return failure;
      }
      scope_type_history.pop();
      list_scope_depth--;
    }

    error_last_closed = tk_it;
  }

  if (paren_scope_depth != 0) {
    auto failure = TkScope{false, begin, last_closed + 1};
    failure.error_message_ = "find_brace: List scope contains mismatched parentheses.";
    return failure;
  }
  if (frame_scope_depth != 0) {
    auto failure = TkScope{false, begin, last_closed + 1};
    failure.error_message_ = "find_brace: List scope contains mismatched frames.";
    return failure;
  }
  if (list_scope_depth != 0) {
    auto failure = TkScope{false, begin, last_closed + 1};
    failure.error_message_ = "find_brace: List scope contains mismatched list scopes.";
    return failure;
  }
  if (last_closed == begin) {
    auto failure = TkScope{false, begin, end};
    failure.error_message_ = "find_brace: List scope at Line:" + std::to_string(last_open.Line()) +
                             " column:" + std::to_string(last_open.Col()) + " was never closed.\n" +
                             "Expected a close token ']' at Line:" + std::to_string(error_last_closed->BegLine()) +
                             " column:" + std::to_string(error_last_closed->BegCol());
    return failure;
  }

  return TkScope{true, begin, last_closed + 1};
}

CND_CX TkScope TkScope::FindBrace(TkCursor crsr) { return FindBrace(crsr.Iter(), crsr.End()); }

CND_CX TkScope TkScope::FindBracket(TkVecConstIterT begin, TkVecConstIterT end) {
  auto paren_scope_depth = 0;
  auto frame_scope_depth = 0;
  auto list_scope_depth = 0;
  std::stack<eTk> scope_type_history;
  TkCursor last_open = {begin, end};
  TkVecConstIterT last_closed = begin;
  TkVecConstIterT error_last_closed = begin;

  if (last_open.AtEnd()) {  // Open is at end therefore cannot be closed.
    auto failure = TkScope{false, begin, end};
    failure.error_message_ = "find_bracket: Open token is at end of token Vec.";
  }

  if (!last_open.TypeIs(eTk::kLBracket)) {  // No open list token to start with.
    auto failure = TkScope{false, begin, end};
    failure.error_message_ = "find_bracket: Open token is not an open frame token.";
    return failure;
  }

  if (last_open.Next().AtEnd()) {  // End right after open, cannot be closed.
    auto failure = TkScope{false, begin, end};
    failure.error_message_ = "find_bracket: Open token is at end of token Vec.";
    return failure;
  }

  if (last_open.Next().TypeIs(eTk::kRBracket)) {  // Empty list
    return TkScope{true, begin, last_open.Next(2).Iter()};
  }

  // find the last matching close token that is not within a () [] or {}
  // scope, if there is no matching close token, return false find the last
  // matching close token that is not within a () [] or {} scope, if there is
  // no matching close token, return false
  for (auto tk_it = (begin + 1); tk_it != end; tk_it++) {
    if (tk_it->TypeIs(eTk::kLBracket)) {
      frame_scope_depth++;
      scope_type_history.push(eTk::kLBracket);
    } else if (tk_it->TypeIs(eTk::kRBracket)) {
      if (scope_type_history.empty()) {
        // This is the end of the scope
        last_closed = tk_it;
        break;
      }

      if (scope_type_history.top() != eTk::kLBracket) {
        auto failure = TkScope{false, begin, end};
        failure.error_message_ = "find_bracket: Close token ']' without open token '['.";
        return failure;
      }
      scope_type_history.pop();
      frame_scope_depth--;
    } else if (tk_it->TypeIs(eTk::kLParen)) {
      paren_scope_depth++;
      scope_type_history.push(eTk::kLParen);
    } else if (tk_it->TypeIs(eTk::kRParen)) {
      if (scope_type_history.empty()) {
        auto failure = TkScope{false, begin, end};
        failure.error_message_ = "find_bracket: Close token ')' without open token '('.";
        return failure;
      }

      if (scope_type_history.top() != eTk::kLParen) {
        auto failure = TkScope{false, begin, end};
        failure.error_message_ = "find_bracket: Close token ')' without open token '('.";
        return failure;
      }
      scope_type_history.pop();
      paren_scope_depth--;
    } else if (tk_it->TypeIs(eTk::kLBrace)) {
      list_scope_depth++;
      scope_type_history.push(eTk::kLBrace);
    } else if (tk_it->TypeIs(eTk::kRBrace)) {
      if (scope_type_history.empty()) {
        auto failure = TkScope{false, begin, end};
        failure.error_message_ = "find_bracket: Close token '}' without open token '{'.";
        return failure;
      }

      if (scope_type_history.top() != eTk::kLBrace) {
        auto failure = TkScope{false, begin, end};
        failure.error_message_ = "find_bracket: Close token '}' without open token '{'.";
        return failure;
      }
      scope_type_history.pop();
      list_scope_depth--;
    }  // end switch

    error_last_closed = tk_it;
  }

  if (paren_scope_depth != 0) {
    auto failure = TkScope{false, begin, last_closed + 1};
    failure.error_message_ = "find_bracket: Frame scope contains mismatched parentheses.";
    return failure;
  }
  if (frame_scope_depth != 0) {
    auto failure = TkScope{false, begin, last_closed + 1};
    failure.error_message_ = "find_bracket: Frame scope contains mismatched frames.";
    return failure;
  }
  if (list_scope_depth != 0) {
    auto failure = TkScope{false, begin, last_closed + 1};
    failure.error_message_ = "find_bracket: Frame scope contains mismatched list scopes.";
    return failure;
  }
  if (last_closed == begin) {
    auto failure = TkScope{false, begin, end};
    failure.error_message_ = "find_bracket: Frame scope at Line:" + std::to_string(last_open.Line()) +
                             " column:" + std::to_string(last_open.Col()) + " was never closed.\n" +
                             "Expected a close token ']' at Line:" + std::to_string(error_last_closed->BegLine()) +
                             " column:" + std::to_string(error_last_closed->BegCol());
    return failure;
  }

  return TkScope{true, begin, last_closed + 1};
}

CND_CX TkScope TkScope::FindBracket(TkCursor crsr) { return FindBracket(crsr.Iter(), crsr.End()); }

CND_CX Vec<TkScope> TkScope::FindSeperatedParen(TkVecConstIterT begin, TkVecConstIterT end, eTk separator) {
  Vec<TkScope> scopes;
  if (begin->Type() != eTk::kLParen) {
    scopes.push_back(TkScope{false, begin, end});
    return scopes;
  }
  std::stack<eTk> scope_type_history;
  TkVecConstIterT last_closed = begin;
  begin++;  // Skip the open list token
  for (TkVecConstIterT i = begin; i < end;) {
    if (i->TypeIs(separator) && scope_type_history.empty()) {
      scopes.push_back(TkScope{true, last_closed, i + 1});
      last_closed = i;
    } else if (i->IsLScope()) {
      scope_type_history.push(i->Type());
    } else if (i->IsRScope() && !scope_type_history.empty()) {
      if (i->IsRScopeOf(scope_type_history.top())) {
        scope_type_history.pop();
      } else {
        scopes.push_back(TkScope{false, i, end});  // Wrong closing scope
        return scopes;
      }
    } else if (i->TypeIs(eTk::kRParen) && scope_type_history.empty()) {
      // end of list
      scopes.push_back(TkScope{true, last_closed, i + 1});
      return scopes;
    } else if (i->TypeIs(eTk::kEofile)) {
      scopes.push_back(TkScope{false, i, end});  // End of file
      return scopes;
    }
    std::advance(i, 1);
  }
}

CND_CX Vec<TkScope> TkScope::FindSeperatedParen(TkCursor crsr, eTk separator) {
  return FindSeperatedParen(crsr.Iter(), crsr.End(), separator);
}

CND_CX Vec<TkScope> TkScope::FindSeperatedParen(TkScope ls, eTk separator) {
  return FindSeperatedParen(ls.Begin(), ls.End(), separator);
}

CND_CX Vec<TkScope> TkScope::FindSeperatedBrace(TkVecConstIterT begin, TkVecConstIterT end, eTk separator) {
  Vec<TkScope> scopes;
  if (begin->Type() != eTk::kLBrace) {
    scopes.push_back(TkScope{false, begin, end});
    return scopes;
  }
  std::stack<eTk> scope_type_history;
  TkVecConstIterT last_closed = begin;
  begin++;  // Skip the open list token
  for (TkVecConstIterT i = begin; i < end;) {
    if (i->TypeIs(separator) && scope_type_history.empty()) {
      scopes.push_back(TkScope{true, last_closed, i + 1});
      last_closed = i;
    } else if (i->IsLScope()) {
      scope_type_history.push(i->Type());
    } else if (i->IsRScope() && !scope_type_history.empty()) {
      if (i->IsRScopeOf(scope_type_history.top())) {
        scope_type_history.pop();
      } else {
        scopes.push_back(TkScope{false, i, end});  // Wrong closing scope
        return scopes;
      }
    } else if (i->TypeIs(eTk::kRBrace) && scope_type_history.empty()) {
      // end of list
      scopes.push_back(TkScope{true, last_closed, i + 1});
      return scopes;
    } else if (i->TypeIs(eTk::kEofile)) {
      scopes.push_back(TkScope{false, i, end});  // End of file
      return scopes;
    }
    std::advance(i, 1);
  }
}

CND_CX Vec<TkScope> TkScope::FindSeperatedBrace(TkCursor crsr, eTk separator) {
  return FindSeperatedBrace(crsr.Iter(), crsr.End(), separator);
}

CND_CX Vec<TkScope> TkScope::FindSeperatedBrace(TkScope ls, eTk separator) {
  return FindSeperatedBrace(ls.Begin(), ls.End(), separator);
}

CND_CX Vec<TkScope> TkScope::FindSeperatedBracket(TkVecConstIterT begin, TkVecConstIterT end, eTk separator) {
  Vec<TkScope> scopes;
  if (begin->Type() != eTk::kLBracket) {
    scopes.push_back(TkScope{false, begin, end});
    return scopes;
  }
  std::stack<eTk> scope_type_history;
  TkVecConstIterT last_closed = begin;
  begin++;  // Skip the open list token
  for (TkVecConstIterT i = begin; i < end;) {
    if (i->TypeIs(separator) && scope_type_history.empty()) {
      scopes.push_back(TkScope{true, last_closed, i + 1});
      last_closed = i;
    } else if (i->IsLScope()) {
      scope_type_history.push(i->Type());
    } else if (i->IsRScope() && !scope_type_history.empty()) {
      if (i->IsRScopeOf(scope_type_history.top())) {
        scope_type_history.pop();
      } else {
        scopes.push_back(TkScope{false, i, end});  // Wrong closing scope
        return scopes;
      }
    } else if (i->TypeIs(eTk::kRBracket) && scope_type_history.empty()) {
      // end of list
      scopes.push_back(TkScope{true, last_closed, i + 1});
      return scopes;
    } else if (i->TypeIs(eTk::kEofile)) {
      scopes.push_back(TkScope{false, i, end});  // End of file
      return scopes;
    }
    std::advance(i, 1);
  }
}

CND_CX Vec<TkScope> TkScope::FindSeperatedBracket(TkCursor crsr, eTk separator) {
  return FindSeperatedBracket(crsr.Iter(), crsr.End(), separator);
}

CND_CX Vec<TkScope> TkScope::FindSeperatedBracket(const TkScope& ls, eTk separator) {
  return FindSeperatedBracket(ls.Begin(), ls.End(), separator);
}

CND_CX TkScope TkScope::FindStatement(eTk open, eTk close, TkVecConstIterT begin, TkVecConstIterT end) {
  auto paren_scope_depth = 0;
  auto frame_scope_depth = 0;
  auto list_scope_depth = 0;
  std::stack<eTk> scope_type_history;
  TkVecConstIterT last_open = begin;
  TkVecConstIterT last_closed = begin;

  if (begin->Type() != open) {
    throw std::runtime_error("find_statement: begin iterator not on an open token.");
  }
  if (std::next(begin)->Type() == close) return TkScope{true, begin, begin + 2};  // Empty statement

  // find the last matching close token that is not within a () [] or {}
  // scope, if there is no matching close token, return false
  for (auto it = begin + 1; it < end; it++) {
    if (it->Type() == eTk::kLParen) {
      paren_scope_depth++;
      scope_type_history.push(eTk::kLParen);
      // currrent_scope_type = eTk::kLParen;
    } else if (it->Type() == eTk::kRParen) {
      if (scope_type_history.empty() || scope_type_history.top() != eTk::kLParen) {
        // Has to be a close or error
        if (it->Type() == close) {
          last_closed = it;
          break;
        } else
          throw std::runtime_error("find_statement: Found a close_scope before an open_scope.");
      }
      scope_type_history.pop();
      paren_scope_depth--;
    } else if (it->Type() == eTk::kLBracket) {
      frame_scope_depth++;
      // currrent_scope_type = eTk::kLBracket;
      scope_type_history.push(eTk::kLBracket);
    } else if (it->Type() == eTk::kRBracket) {
      if (scope_type_history.empty() || scope_type_history.top() != eTk::kLBracket) {
        // Has to be a close or error
        if (it->Type() == close) {
          last_closed = it;
          break;
        } else
          throw std::runtime_error(
              "find_statement: Found a close frame token before an open "
              "frame token.");
      }
      scope_type_history.pop();
      frame_scope_depth--;
    } else if (it->Type() == eTk::kLBrace) {
      list_scope_depth++;
      // currrent_scope_type = eTk::kLBrace;
      scope_type_history.push(eTk::kLBrace);
    } else if (it->Type() == eTk::kRBrace) {
      if (scope_type_history.empty() || scope_type_history.top() != eTk::kLBrace) {
        // Has to be a close or error
        if (it->Type() == close) {
          last_closed = it;
          break;
        } else
          throw std::runtime_error(
              "find_statement: Found a close list token before an open list "
              "token.");
      }
      scope_type_history.pop();
      list_scope_depth--;
    }

    if (paren_scope_depth == 0 && frame_scope_depth == 0 && list_scope_depth == 0) {
      if (it->Type() == close) {
        last_closed = it;
        break;
      } else if (it->Type() == open)
        throw std::runtime_error(
            "find_statement: Found a repeated open token before a close "
            "token.");
    }
  }

  // if(last_closed == end)
  //	return TkScope{ false, begin, last_closed + 1 }; // No matching close
  // token

  // NOTE: We are adding 1 to last closed because the end is 1 past the last
  // token.
  if (paren_scope_depth != 0) {
    return TkScope{false, begin, last_closed + 1};  // No matching close token for parenthesis
  } else if (frame_scope_depth != 0) {
    return TkScope{false, begin, last_closed + 1};  // No matching close token for frame
  } else if (list_scope_depth != 0) {
    return TkScope{false, begin, last_closed + 1};  // No matching close token for list
  } else {
    return TkScope{true, begin, last_closed + 1};
  }

}  // end find_scope

/// Open token may be repeated.
/// TODO: refactor to use TkCursor instead of TkVecConstIterT next time you
/// use this.
CND_CX TkScope TkScope::FindOpenStatement(eTk open, eTk close, TkVecConstIterT begin, TkVecConstIterT end) {
  auto paren_scope_depth = 0;
  auto frame_scope_depth = 0;
  auto list_scope_depth = 0;
  // eTk currrent_scope_type = eTk::knone_;
  std::stack<eTk> scope_type_history;
  TkVecConstIterT last_open = begin;
  TkVecConstIterT last_closed = begin;

  if (begin + 1 == end) return TkScope{false, begin, end + 1};  // Empty statement

  if (begin->Type() != open) {
    throw std::runtime_error("find_statement: begin iterator not on an open token.");
  }

  if (std::next(begin)->Type() == close) return TkScope{true, begin, begin + 2};  // Empty statement

  // SPECIAL CASE: if the open token is a list,paren or frame-> set
  // scope_type_history and scope depth of the list,paren or frame.
  if (begin->Type() == eTk::kLParen) {
    paren_scope_depth++;
    scope_type_history.push(eTk::kLParen);
  } else if (begin->Type() == eTk::kLBracket) {
    frame_scope_depth++;
    scope_type_history.push(eTk::kLBracket);
  } else if (begin->Type() == eTk::kLBrace) {
    list_scope_depth++;
    scope_type_history.push(eTk::kLBrace);
  }

  // find the last matching close token that is not within a () [] or {}
  // scope, if there is no matching close token, return false
  for (auto it = begin + 1; it < end; it++) {
    if (it->Type() == eTk::kLParen) {
      paren_scope_depth++;
      scope_type_history.push(eTk::kLParen);
      // currrent_scope_type = eTk::kLParen;
    } else if (it->Type() == eTk::kRParen) {
      if (scope_type_history.empty()) {
        return TkScope{false, begin, end};  // Close token without open token
      }

      if (scope_type_history.top() != eTk::kLParen) {
        // Has to be a close or error
        if (it->Type() == close) {
          last_closed = it;
          break;
        } else
          throw std::runtime_error("find_statement: Found a close_scope before an open_scope.");
      }
      scope_type_history.pop();
      paren_scope_depth--;
    } else if (it->Type() == eTk::kLBracket) {
      frame_scope_depth++;
      // currrent_scope_type = eTk::kLBracket;
      scope_type_history.push(eTk::kLBracket);
    } else if (it->Type() == eTk::kRBracket) {
      if (scope_type_history.top() != eTk::kLBracket) {
        // Has to be a close or error
        if (it->Type() == close) {
          last_closed = it;
          break;
        } else
          throw std::runtime_error(
              "find_statement: Found a close frame token before an open "
              "frame token.");
      }
      scope_type_history.pop();
      frame_scope_depth--;
    } else if (it->Type() == eTk::kLBrace) {
      list_scope_depth++;
      // currrent_scope_type = eTk::kLBrace;
      scope_type_history.push(eTk::kLBrace);
    } else if (it->Type() == eTk::kRBrace) {
      if (scope_type_history.top() != eTk::kLBrace) {
        // Has to be a close or error
        if (it->Type() == close) {
          last_closed = it;
          break;
        } else
          throw std::runtime_error(
              "find_statement: Found a close list token before an open list "
              "token.");
      }
      scope_type_history.pop();
      list_scope_depth--;
    }

    if (paren_scope_depth == 0 && frame_scope_depth == 0 && list_scope_depth == 0) {
      if (it->Type() == close) {
        last_closed = it;
        break;
      }
    }
  }

  // if(last_closed == end)
  //	return TkScope{ false, begin, last_closed + 1 }; // No matching close
  // token

  // NOTE: We are adding 1 to last closed because the end is 1 past the last
  // token.
  if (paren_scope_depth != 0) {
    return TkScope{false, begin, last_closed + 1};
  } else if (frame_scope_depth != 0) {
    return TkScope{false, begin, last_closed + 1};
  } else if (list_scope_depth != 0) {
    return TkScope{false, begin, last_closed + 1};
  } else {
    if (last_closed == begin)  // Expression was never closed
      return TkScope{false, begin, last_closed + 1};
    else
      return TkScope{true, begin, last_closed + 1};
  }

}  // end find_scope

/// Open token may be repeated.
/// TODO: refactor to use TkCursor instead of TkVecConstIterT next time you
/// use this.
CND_CX TkScope TkScope::FindOpenStatement(eTk open, Vec<eTk> close, TkVecConstIterT begin, TkVecConstIterT end) {
  auto paren_scope_depth = 0;
  auto frame_scope_depth = 0;
  auto list_scope_depth = 0;
  // eTk currrent_scope_type = eTk::knone_;
  std::stack<eTk> scope_type_history;
  TkVecConstIterT last_open = begin;
  TkVecConstIterT last_closed = begin;

  if (begin + 1 == end) return TkScope{false, begin, end + 1};  // Empty statement

  if (begin->Type() != open) {
    throw std::runtime_error("find_statement: begin iterator not on an open token.");
  }

  if (std::any_of(close.begin(), close.end(), [begin](eTk tk) { return std::next(begin)->Type() == tk; }))
    return TkScope{true, begin, begin + 2};  // Empty statement

  // SPECIAL CASE: if the open token is a list,paren or frame-> set
  // scope_type_history and scope depth of the list,paren or frame.
  if (begin->Type() == eTk::kLParen) {
    paren_scope_depth++;
    scope_type_history.push(eTk::kLParen);
  } else if (begin->Type() == eTk::kLBracket) {
    frame_scope_depth++;
    scope_type_history.push(eTk::kLBracket);
  } else if (begin->Type() == eTk::kLBrace) {
    list_scope_depth++;
    scope_type_history.push(eTk::kLBrace);
  }

  // find the last matching close token that is not within a () [] or {}
  // scope, if there is no matching close token, return false
  for (auto it = begin + 1; it < end; it++) {
    if (it->Type() == eTk::kLParen) {
      paren_scope_depth++;
      scope_type_history.push(eTk::kLParen);
      // currrent_scope_type = eTk::kLParen;
    } else if (it->Type() == eTk::kRParen) {
      if (scope_type_history.empty()) {
        return TkScope{false, begin, end};  // Close token without open token
      }

      if (scope_type_history.top() != eTk::kLParen) {
        // Has to be a close or error
        if (std::any_of(close.begin(), close.end(), [it](eTk tk) { return it->Type() == tk; })) {
          last_closed = it;
          break;
        } else
          throw std::runtime_error("find_statement: Found a close_scope before an open_scope.");
      }
      scope_type_history.pop();
      paren_scope_depth--;
    } else if (it->Type() == eTk::kLBracket) {
      frame_scope_depth++;
      // currrent_scope_type = eTk::kLBracket;
      scope_type_history.push(eTk::kLBracket);
    } else if (it->Type() == eTk::kRBracket) {
      if (scope_type_history.top() != eTk::kLBracket) {
        // Has to be a close or error
        if (std::any_of(close.begin(), close.end(), [it](eTk tk) { return it->Type() == tk; })) {
          last_closed = it;
          break;
        } else
          throw std::runtime_error(
              "find_statement: Found a close frame token before an open "
              "frame token.");
      }
      scope_type_history.pop();
      frame_scope_depth--;
    } else if (it->Type() == eTk::kLBrace) {
      list_scope_depth++;
      // currrent_scope_type = eTk::kLBrace;
      scope_type_history.push(eTk::kLBrace);
    } else if (it->Type() == eTk::kRBrace) {
      if (scope_type_history.top() != eTk::kLBrace) {
        // Has to be a close or error
        if (std::any_of(close.begin(), close.end(), [it](eTk tk) { return it->Type() == tk; })) {
          last_closed = it;
          break;
        } else
          throw std::runtime_error(
              "find_statement: Found a close list token before an open list "
              "token.");
      }
      scope_type_history.pop();
      list_scope_depth--;
    }

    if (paren_scope_depth == 0 && frame_scope_depth == 0 && list_scope_depth == 0) {
      if (std::any_of(close.begin(), close.end(), [it](eTk tk) { return it->Type() == tk; })) {
        last_closed = it;
        break;
      }
    }
  }

  // if(last_closed == end)
  //	return TkScope{ false, begin, last_closed + 1 }; // No matching close
  // token

  // NOTE: We are adding 1 to last closed because the end is 1 past the last
  // token.
  if (paren_scope_depth != 0) {
    return TkScope{false, begin, last_closed + 1};
  } else if (frame_scope_depth != 0) {
    return TkScope{false, begin, last_closed + 1};
  } else if (list_scope_depth != 0) {
    return TkScope{false, begin, last_closed + 1};
  } else {
    if (last_closed == begin)  // Expression was never closed
      return TkScope{false, begin, last_closed + 1};
    else
      return TkScope{true, begin, last_closed + 1};
  }

}  // end find_scope

/// Starts with the begin token which may be repeated, ends with a semicolon_
/// ';'
CND_CX TkScope TkScope::FindProgramStatement(TkVecConstIterT begin, TkVecConstIterT end) {
  return FindOpenStatement(begin->Type(), eTk::kSemicolon, begin, end);
}

CND_CX TkScope TkScope::FindProgramStatement(TkCursor cursor) {
  return FindOpenStatement(cursor.Get().Type(), eTk::kSemicolon, cursor.Iter(), cursor.End());
}

CND_CX const Str& TkScope::Error() const { return error_message_; }

CND_CX Bool TkScope::Valid() const { return valid_; }

/// Returns the end of the scope, not including the close token.
CND_CX TkScope::TkVecConstIterT TkScope::ContainedEnd() const { return end_ - 1; }

/// Returns the beginning of the scope, not including the open token.
CND_CX TkScope::TkVecConstIterT TkScope::ContainedBegin() const { return begin_ + 1; }

/// Returns true if the scope is empty.
CND_CX Bool TkScope::IsEmpty() const { return ContainedBegin() == ContainedEnd(); }

/// Returns the end of the scope, including the close token.
CND_CX TkScope::TkVecConstIterT TkScope::End() const { return end_; }

/// Returns the beginning of the scope, including the open token.
CND_CX TkScope::TkVecConstIterT TkScope::Begin() const { return begin_; }

CND_CX TkCursor TkScope::Contained() const { return TkCursor{ContainedBegin(), ContainedEnd()}; }

CND_CX TkScope::operator Bool() const { return valid_; }

CND_CX TkScope::TkScope(Bool valid, TkVecConstIterT begin, TkVecConstIterT end)
    : valid_(valid), begin_(begin), end_(end) {}

}  // namespace cnd

/// @} // end of cnd_compiler_data

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// Licensed under the GNU Affero General Public License, Version 3.
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////