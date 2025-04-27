///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
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
// clang-format off
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
#include "use_clmsg.hpp"
#include "cldata/tk.hpp"
#include "cldata/ast.hpp"
#include "token_cursor.hpp"
#include "token_scope.hpp"

#include "token_closure.hpp"
// clang-format on

namespace cnd {

/// Primary Expression Shift-Reduction Parser
///
/// Cursor begin and end must be the start and end of the expression.
/// Note this parser does not take the entire source as an argument.
/// Only pass the current scope to be parser.
/// The end of TkCursor may or may not be end of source.
/// This parser does not advance the cursor or return a new start point.
class ExprParser {
 public:
  static ExpectedAst Perform(TkCursor c) { return ExprParser().Parse(c); }
  ExpectedAst Parse(TkCursor c);

 private:
  /// Set is_first_operator_ to false if currently true, else do nothing.
  void FirstOperatorSwitch();
  inline void ResolvePrefix();
  inline void ResolvePostfix();
  inline void ResolveBinaryLeftAssoc();
  inline void ResolveBinaryRightAssoc();
  inline void ResolveBinary();
  /// Resolves the last closure based on type,unchecked.
  inline void ResolveLast();

  /// [Action Skip] Push OPERAND to closure output,
  ///    advance head by the number of tokens consumed.
  inline cxx::BoolError ActionSkip(TkCursor& c);

  /// [Action Store] Push OPERATOR to closure output,
  ///                Push back a new closure from OPERATOR,
  ///                advance head by the number of tokens consumed.
  inline cxx::BoolError ActionStore(TkCursor& c);

  /// [Action Check] Compares priority of head and last closure.
  ///    If head >= last closure, store head.
  ///    If head < last closure,
  ///        -> resolve all consecutive closures of lower priority.
  ///           Then store.
  ///    If head is at end, resolve all closures. Return.
  inline cxx::BoolError ActionCheck(TkCursor& c);

  /// [Choose Action] Decides which action to take.
  ///                 Main loop of the closure resolver.
  /// Action will be skip , store, or check.
  /// - On first occurence of an operator- a store will be performed.
  ///    All following operators will be checks.
  /// - A skip is performed when an operand is encountered.
  ///    Multi-token operands trigger repeated skips.
  /// - A check is performed when an operator is encountered.
  /// - At the end, always do a check.
  inline cxx::BoolError ChooseAction(TkCursor& c);

  cxx::Expected<std::vector<Tk>> CreateAndResolveClosures(TkCursor c);

  /// After the closures are resolved, perform an LL recursive descent parse.
  /// Expect the expression to be fully parenthesized.
  ExpectedAst ParseImpl(TkCursor c);

 private:
  enum class eNextExpectedHeadToken { kOperative, kOperator, kNone };
  static const Tk kOpenParenTk;
  static const Tk kCloseParenTk;
  /// Expected token type on next iteration.
  eNextExpectedHeadToken next_expected_head_token_ = {eNextExpectedHeadToken::kOperative};
  ClosureBuffer closure_buffer_;
  bool is_first_operator_ = {true};
  bool is_resolved_ = {false};
};

// Static const members of ExprParser.
const Tk parser::ExprParser::kOpenParenTk = Tk{eTk::LParen, "("};
const Tk ExprParser::kCloseParenTk = Tk{eTk::RParen, ")"};

ExpectedAst ExprParser::Parse(TkCursor c) {
  cxx::Expected<vector<Tk>> resolved_closures_result = CreateAndResolveClosures(c);
  if (not resolved_closures_result) {
    return ExpectedAst::Failure(resolved_closures_result.Error());
  }

  const vector<Tk>& resolved_closures = resolved_closures_result.Value();

  TkCursor intermediate_expr(resolved_closures.cbegin(), resolved_closures.cend());
  ExpectedAst expected_parse_result = ParseImpl(intermediate_expr);

  if (not expected_parse_result) {
    return expected_parse_result;
  }
  return ExpectedAst::Success(expected_parse_result.Extract());
}

void ExprParser::FirstOperatorSwitch() {
  if (is_first_operator_) {
    is_first_operator_ = false;
  }
}

inline void ExprParser::ResolvePrefix() {
  // Find concecutive prefix closures.
  auto nclosures = closure_buffer_
                       .FindClosureReverseConsecutive([](ClosureListIter a, ClosureListIter b) -> bool {
                         return (a->Priority() == b->Priority()) && (a->Operation() == b->Operation());
                       })
                       .size();
  auto closure_count = nclosures;
  while (nclosures > 0) {
    closure_buffer_.StreamInsertBeforeClosure(std::prev(closure_buffer_.LastClosure(), nclosures - 1), kOpenParenTk);
    closure_buffer_.StreamPushBack(kCloseParenTk);
    nclosures--;
  }

  for (std::size_t i = 0; i < closure_count; i++) {
    closure_buffer_.PopClosure();
  }
}

inline void ExprParser::ResolvePostfix() {
  // Find concecutive postfix closures.
  auto nclosures = closure_buffer_
                       .FindClosureReverseConsecutive([](ClosureListIter a, ClosureListIter b) -> bool {
                         return (a->Priority() == b->Priority()) && (a->Operation() == b->Operation());
                       })
                       .size();
  auto closure_count = nclosures;

  while (nclosures > 0) {
    closure_buffer_.StreamInsertAfterClosure(std::prev(closure_buffer_.LastClosure(), closure_count), kOpenParenTk);

    closure_buffer_.StreamInsertAfterClosure(std::prev(closure_buffer_.LastClosure(), nclosures - 1), kCloseParenTk);

    nclosures--;
  }

  for (std::size_t i = 0; i < closure_count; i++) {
    closure_buffer_.PopClosure();
  }
};

inline void ExprParser::ResolveBinaryLeftAssoc() {
  std::vector<ClosureListIter> binary_closures = closure_buffer_.FindClosureReverseConsecutiveAndIgnore(
      [](ClosureListIter a, ClosureListIter b) -> bool {
        return (a->Priority() == b->Priority()) && (a->Operation() == b->Operation()) && (a->Assoc() == b->Assoc());
      },
      [](ClosureListIter a) -> bool {
        return ((a->Priority() == ePriority::Postfix) || (a->Priority() == ePriority::Prefix));
      });

  // Resolve first closure.(last in the list)
  ClosureListIter open_paren_location = std::prev(binary_closures.back());
  // skip any postfix/prefix closures when looking for the open paren loc.
  while (open_paren_location->Priority() == ePriority::Postfix ||
         open_paren_location->Priority() == ePriority::Prefix) {
    open_paren_location = std::prev(open_paren_location);
  }

  closure_buffer_.StreamInsertAfterClosure(open_paren_location, {eTk::LParen, "("});
  closure_buffer_.StreamPushBack({eTk::RParen, ")"});

  // Resolve the rest,skip last in loop.
  std::vector<ClosureListIter>::iterator close_paren_location_iter = binary_closures.begin();
  while (close_paren_location_iter != std::prev(binary_closures.end())) {
    closure_buffer_.StreamInsertAfterClosure(open_paren_location, {eTk::LParen, "("});

    closure_buffer_.StreamInsertBeforeClosure(*close_paren_location_iter, {eTk::RParen, ")"});
    close_paren_location_iter = std::next(close_paren_location_iter);
  }

  // Pop all collected closures.
  for (ClosureListIter closure_iter : binary_closures) {
    closure_buffer_.PopClosure(closure_iter);
  }
};

inline void ExprParser::ResolveBinaryRightAssoc() {
  std::vector<ClosureListIter> binary_closures = closure_buffer_.FindClosureReverseConsecutiveAndIgnore(
      [](ClosureListIter a, ClosureListIter b) -> bool {
        return (a->Priority() == b->Priority()) && (a->Operation() == b->Operation()) && (a->Assoc() == b->Assoc());
      },
      [](ClosureListIter a) -> bool {
        return ((a->Priority() == ePriority::Postfix) || (a->Priority() == ePriority::Prefix));
      });

  // Resolve first closure.(last in the list)
  closure_buffer_.StreamInsertAfterClosure(std::prev(binary_closures.back()), {eTk::LParen, "("});
  closure_buffer_.StreamPushBack({eTk::RParen, ")"});
  // Resolve the rest.
  std::vector<ClosureListIter>::iterator open_paren_location_iter = std::next(binary_closures.begin());
  while (open_paren_location_iter != binary_closures.end()) {
    closure_buffer_.StreamInsertAfterClosure(*open_paren_location_iter, {eTk::LParen, "("});
    closure_buffer_.StreamPushBack({eTk::RParen, ")"});
    open_paren_location_iter = std::next(open_paren_location_iter);
  }
  // Pop all collected closures.
  for (ClosureListIter closure_iter : binary_closures) {
    closure_buffer_.PopClosure(closure_iter);
  }
};

inline void ExprParser::ResolveBinary() {
  // Resolve associative binary if there are repeated equivalent priority
  // closures before the last.
  if (closure_buffer_.LastClosure()->Priority() == std::prev(closure_buffer_.LastClosure())->Priority()) {
    if (closure_buffer_.LastClosure()->Assoc() == eAssoc::Left) {
      ResolveBinaryLeftAssoc();
    } else if (closure_buffer_.LastClosure()->Assoc() == eAssoc::Right) {
      ResolveBinaryRightAssoc();
    } else {
      throw "ResolveBinary: Invalid binary token in closure, association "
            "not left or right.\n";
    }
  } else {  // Resolve Singular Binary Closure
    // Special Case: If the binary closure is higher priority than
    // prefix/postfix
    //               Then don't only skip the postfix.

    ClosureListIter open_paren_insertion_loc_iter = std::prev(closure_buffer_.LastClosure());

    // Lower:
    if (closure_buffer_.LastClosure()->Priority() < ePriority::Postfix) {
      // If there are postfix/prefix closures-> skip them when
      // looking for the open paren insertion location.
      ClosureListIter postfix_it = std::prev(closure_buffer_.LastClosure());
      while (postfix_it->Priority() == ePriority::Postfix) {
        postfix_it = std::prev(postfix_it);
      }

      ClosureListIter prefix_it = postfix_it;
      if (prefix_it->Priority() == ePriority::Prefix) {
        while (prefix_it->Priority() == ePriority::Prefix) {
          prefix_it = std::prev(prefix_it);
        }
      }

      open_paren_insertion_loc_iter = prefix_it;
    }
    // Higher(will never be equal):
    else {
      // If there are postfix/prefix closures-> skip them when
      // looking for the open paren insertion location.
      ClosureListIter postfix_it = std::prev(closure_buffer_.LastClosure());
      while (postfix_it->Priority() == ePriority::Postfix) {
        postfix_it = std::prev(postfix_it);
      }

      open_paren_insertion_loc_iter = postfix_it;
    }

    closure_buffer_.StreamInsertAfterClosure(open_paren_insertion_loc_iter, {eTk::LParen, "("});
    closure_buffer_.StreamPushBack({eTk::RParen, ")"});
    closure_buffer_.PopClosure();
  }
};

inline void ExprParser::ResolveLast() {
  if (closure_buffer_.LastClosure()->Operation() == eOperation::Prefix) {
    ResolvePrefix();
  } else if (closure_buffer_.LastClosure()->Operation() == eOperation::Postfix) {
    ResolvePostfix();
  } else if (closure_buffer_.LastClosure()->Operation() == eOperation::Binary) {
    ResolveBinary();
  } else {
    throw "ActionResolveLast: Invalid token type in closure.Must be an "
               "operator.\n";
  }
}

inline cxx::BoolError ExprParser::ActionSkip(TkCursor& c) {
  // Check for an open paren-> Subexpression
  // Resolve the subexpr first.
  // Push resolved result to output.
  if (c.TypeIs(eTk::LParen)) {
    TkScope scope = TkScope::FindParen(c);
    if (not scope) {
      return "Mismatched parentheses in operand.";
    } else {
      // Resolve but do not parse!the inside of the parentheses.
      ExprParser resolved_subexpr_parser;
      cxx::Expected<TkVector> resolved_subexpr_result =
          ExprParser().CreateAndResolveClosures({scope.ContainedBegin(), scope.ContainedEnd()});
      if (not resolved_subexpr_result) {
        return "ExprParser::ActionSkip: Could not resolve "
               "subexpression.\n" +
               resolved_subexpr_result.Error();
      }
      const TkVector& resolved_subexpr = resolved_subexpr_result.Value();
      for (const Tk& tk : resolved_subexpr) {
        closure_buffer_.StreamPushBack(tk);
      }
      for (TkVectorConstIter i = c.Iter(); i != scope.End(); i++) {
        c.Advance();
      }
      return true;
    }
  }
  // Check if this is a singular token OPERAND.
  if (c.Operation() == eOperation::None || c.TypeIs(eTk::Sub)) {
    // If unary minus - Emplace unary minus into operand.
    if (c.TypeIs(eTk::Sub)) {
      c.Advance();
      if (c.TypeIs(eTk::LitInt) || c.TypeIs(eTk::LitReal)) {
        Tk negative_number = c.Get();
        negative_number.LiteralMutable() = "-" + negative_number.Literal();
        closure_buffer_.StreamPushBack(negative_number);
        c.Advance();
        return true;
      } else {
        return "ExprParser::ActionSkip: Unary minus must be "
               "followed by a numeric literal.\n";
      }
    }
    // Parse single token operand.
    closure_buffer_.StreamPushBack(c.Get());
    c.Advance();
    return true;
  }

  return "ExprParser::ActionSkip: Could not read operand token, "
         "invalid or unimplemented.";
}

inline cxx::BoolError ExprParser::ActionStore(TkCursor& c) {
  // Check for an open paren-> Function Call
  // Arguments are parsed and resolved in ParseImpl.
  // Push a closure for the function call.
  if (c.TypeIs(eTk::LParen)) {
    TkScope scope = TkScope::FindParen(c);
    if (not scope) {
      return "Mismatched parentheses in function call.";
    } else {
      closure_buffer_.StreamPushBack(c.Get());
      TkListIter fn_args_closure_begin = closure_buffer_.LastStreamed();
      c.Advance();
      // Store the rest of the function call.
      for (TkVectorConstIter i = c.Iter(); i != scope.End(); i++) {
        closure_buffer_.StreamPushBack(c.Get());
        c.Advance();
      }
      // Make the closure.
      closure_buffer_.PushBackClosure(fn_args_closure_begin, closure_buffer_.LastStreamed());
      return true;
    }
  }
  // Check for an open bracket-> Indexing Operator
  if (c.TypeIs(eTk::LBracket)) {
    TkScope scope = TkScope::FindBracket(c);
    if (not scope) {
      return "Mismatched bracket in indexing call.";
    } else {
      closure_buffer_.StreamPushBack(c.Get());
      TkListIter fn_args_closure_begin = closure_buffer_.LastStreamed();
      c.Advance();
      for (TkVectorConstIter i = c.Iter(); i != scope.End(); i++) {
        closure_buffer_.StreamPushBack(c.Get());
        c.Advance();
      }
      closure_buffer_.PushBackClosure(fn_args_closure_begin, closure_buffer_.LastStreamed());
      return true;
    }
  }
  // Listing Operator
  if (c.TypeIs(eTk::LBrace)) {
    TkScope scope = TkScope::FindBrace(c);
    if (not scope) {
      return "Mismatched brace in listing call.";
    } else {
      closure_buffer_.StreamPushBack(c.Get());
      TkListIter fn_args_closure_begin = closure_buffer_.LastStreamed();
      c.Advance();
      for (TkVectorConstIter i = c.Iter(); i != scope.End(); i++) {
        closure_buffer_.StreamPushBack(c.Get());
        c.Advance();
      }
      closure_buffer_.PushBackClosure(fn_args_closure_begin, closure_buffer_.LastStreamed());
      return true;
    }
  }
  // Single Token Operator.
  if (c.Operation() == eOperation::Binary || c.Operation() == eOperation::Prefix ||
      c.Operation() == eOperation::Postfix) {
    closure_buffer_.StreamPushBack(c.Get());
    closure_buffer_.PushBackClosure(closure_buffer_.LastStreamed());
    c.Advance();
    return true;
  }

  return "ExprParser::ActionStore: Could not read operator "
         "token, "
         "invalid or unimplemented.";
}

inline cxx::BoolError ExprParser::ActionCheck(TkCursor& c) {
  if (c.AtEnd()) {
    while (closure_buffer_.ClosureCount() > 0) {
      ResolveLast();
    }
    return true;
  } else {
    if (c.Priority() >= closure_buffer_.LastClosure()->Priority()) {
      // If: (Head >= Last Closure) => Store
      ActionStore(c);
    } else {
      // While:( Head < Last Closure ) => Resolve Next
      while (c.Priority() < closure_buffer_.LastClosure()->Priority()) {
        ResolveLast();
      }
      ActionStore(c);
    }
    return true;
  }
}

inline cxx::BoolError ExprParser::ChooseAction(TkCursor& c) {
  if (c.AtEnd()) {
    cxx::BoolError check_result = ActionCheck(c);
    if (check_result) {
      is_resolved_ = true;
      return true;
    } else {
      return "ExprParser::ChooseAction: Could not resolve "
             "closures.\n" +
             check_result.Error();
    }
  }
  // Choose action based on expected head token.
  else {
    // Operative-> Looking for an operand,prefix operator,open
    // scope(subexpr)
    // SPECIAL CASE[Unary Minus] -> If binary '-' is encountered.
    //                           -> Bind tightly to the following operand.
    //                           -> Assert Numeric Literal. Emplace - into
    //                           operand.
    //                           -> Skip Both.
    //                         // If the following is non number, error.
    if (next_expected_head_token_ == eNextExpectedHeadToken::kOperative) {
      if (c.Operation() == eOperation::None || c.TypeIs(eTk::LParen) || c.TypeIs(eTk::Sub)) {
        cxx::BoolError skip_result = ActionSkip(c);
        if (skip_result) {
          next_expected_head_token_ = eNextExpectedHeadToken::kOperator;
          return true;
        } else {
          return "ExprParser::ChooseAction: Could not skip "
                 "operand.\n" +
                 skip_result.Error();
        }
      }
      // Prefix Operator -> Check Single Prefix Operator.
      else if (c.Operation() == eOperation::Prefix) {
        if (is_first_operator_) {
          cxx::BoolError store_result = ActionStore(c);
          if (not store_result) {
            return "ExprParser::ChooseAction: Could not store "
                   "prefix operator.\n" +
                   store_result.Error();
          }
        } else {
          cxx::BoolError check_result = ActionCheck(c);
          if (not check_result) {
            return "ExprParser::ChooseAction: Could not check "
                   "prefix operator.\n" +
                   check_result.Error();
          }
        }
        FirstOperatorSwitch();
        next_expected_head_token_ = eNextExpectedHeadToken::kOperative;
        return true;
      }
      // expected operand got operator -> user error
      else if (c.Operation() == eOperation::Binary || c.Operation() == eOperation::Postfix) {
        return "ExprParser::ChooseAction: Invalid token type "
               "encountered, operator following operator.\n";
      }
      // Else user error -> this token does not belong in a value
      // expression.
      else {
        return "ExprParser::ChooseAction: "
               "Token type not valid for a primary expression.\n";
      }
    }
    // Operator
    // Looking for a binary operator, postfix operator, or open
    // paren(function call), or open brace(index operator)
    else if (next_expected_head_token_ == eNextExpectedHeadToken::kOperator) {
      // Open Paren, Open Bracket, Postfix Operator
      //    -> Check, next is Operator.
      if (c.TypeIs(eTk::LParen) || c.TypeIs(eTk::LBracket) || c.TypeIs(eTk::LBrace) ||
          c.Operation() == eOperation::Postfix) {
        cxx::BoolError action_result;
        if (is_first_operator_) {
          action_result = ActionStore(c);
          FirstOperatorSwitch();
          next_expected_head_token_ = eNextExpectedHeadToken::kOperator;
        } else {
          action_result = ActionCheck(c);
          FirstOperatorSwitch();
          next_expected_head_token_ = eNextExpectedHeadToken::kOperator;
        }
      }
      // Binary Operator -> Check, next is Operative.
      else if (c.Operation() == eOperation::Binary) {
        cxx::BoolError action_result;
        if (is_first_operator_) {
          action_result = ActionStore(c);
        } else {
          action_result = ActionCheck(c);
        }
        FirstOperatorSwitch();
        next_expected_head_token_ = eNextExpectedHeadToken::kOperative;
      }
      // Prefix -> user Error, prefix following operand.
      else if (c.Operation() == eOperation::Prefix) {
        return "ExprParser::ChooseAction: Invalid token type "
               "encountered, prefix following operand.\n";
      }
      // Operand -> user Error, operand following operand.
      else if (c.Operation() == eOperation::None) {
        return "ExprParser::ChooseAction: Invalid token type "
               "encountered, operand following operand.\n";
      }
      // Else user error -> this token does not belong in a value
      // expression.
      else {
        return "ExprParser::ChooseAction: "
               "Token type not valid for a primary expression.\n";
      }
    }
    // This should never happen -> critical logic error.
    // eNextExpectedHeadToken enum set to invalid value.
    else {
      throw "[CRITICAL LOGIC ERROR] ExprParser::ChooseAction: "
                "Invalid next expected head token enumeration type.\n";
    }
  }
}

cxx::Expected<std::vector<Tk>> ExprParser::CreateAndResolveClosures(TkCursor c) {
  while (not is_resolved_) {
    auto action_result = ChooseAction(c);
    if (not action_result) {
      return cxx::Expected<std::vector<Tk>>::Failure(action_result.Error());
    }
  }
  return cxx::Expected<std::vector<Tk>>::Success(closure_buffer_.StreamToVector());
}

ExpectedAst ExprParser::ParseImpl(TkCursor c) {
  using enum eTk;
  using namespace caerr;

  Ast out_node;

  if (c.TypeIs(LParen)) {
    auto scope = TkScope::FindParen(c);
    if (not scope) {
      return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
    } else {
      // 1. Check for redundant paren-> Parse the inside of the paren instead.
      if (scope.End() == c.End()) {
        return ParseImpl({scope.ContainedBegin(), scope.ContainedEnd()});
      }
      // 2. Else scope is an operand contained in a subexpression.
      else {
        ExpectedAst lhs_result = ParseImpl({scope.ContainedBegin(), scope.ContainedEnd()});
        if (not lhs_result) {
          return lhs_result;
        };
        c.Advance(scope.End());

        // 2.1. Postfix
        if (c.Operation() == eOperation::Postfix) {
          // 2.1.1. Postfix () -> Function Call
          if (c.TypeIs(LParen)) {
            auto scope = TkScope::FindParen(c);
            if (not scope) {
              return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
            }
            // Parse the arguments
            auto arguments_result = ParseArguments(c);
            if (not arguments_result) {
              return ExpectedAst::Failure(arguments_result.Error());
            }
            out_node = Ast(eAst::FunctionCall, "", lhs_result.Extract(), arguments_result.Extract());
            c.Advance(scope.End());
          }
          // 2.1.2. Postfix [] -> Index Operator
          else if (c.TypeIs(LBracket)) {
            auto scope = TkScope::FindBracket(c);
            if (not scope) {
              return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
            }
            auto arguments_result = ParseIndexingArguments(c);
            if (not arguments_result) {
              return ExpectedAst::Failure(arguments_result.Error());
            }
            out_node = Ast(eAst::IndexOperator, "", lhs_result.Extract(), arguments_result.Extract());
            c.Advance(scope.End());
          }
          // 2.1.3. Postfix {} -> Listing Operator
          else if (c.TypeIs(LBrace)) {
            auto scope = TkScope::FindBrace(c);
            if (not scope) {
              return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
            }
            auto arguments_result = ParseListingArguments(c);
            if (not arguments_result) {
              return ExpectedAst::Failure(arguments_result.Error());
            }
            out_node = Ast(eAst::ListingOperator, "", lhs_result.Extract(), arguments_result.Extract());
            c.Advance(scope.End());
          }
          // 2.1.4. Postfix Single Token Operator.
          else {
            out_node = Ast(c.Get());
            out_node.ExtractAndPush(lhs_result);
            c.Advance();
          }
        }
        // 2.2. Binary Operator
        else if (c.Operation() == eOperation::Binary) {
          out_node = Ast(c.Get());
          c.Advance();
          // Expecting an operand after a binary operator.
          // 2.2.1. Open Paren -> Subexpr
          if (c.TypeIs(LParen)) {
            auto scope = TkScope::FindParen(c);
            if (not scope) {
              return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
            } else {
              ExpectedAst rhs_result = ParseImpl({scope.ContainedBegin(), scope.ContainedEnd()});
              if (not rhs_result) {
                return rhs_result;
              }
              out_node.ExtractAndPush(lhs_result);
              out_node.ExtractAndPush(rhs_result);
              c.Advance(scope.End());
            }
          }
          // 2.2.2. Else it has to be a singular token operand.
          else {
            OffsetParseResult rhs_result = ParseOperand(c);
            if (not rhs_result) {
              return ExpectedAst::Failure(rhs_result.Error());
            }
            out_node.ExtractAndPush(lhs_result);
            out_node.ExtractAndPush(rhs_result);
            c.Advance(rhs_result);
          }
        }
        // 2.3 End of expr
        else {
          if (c.AtEnd()) {
            out_node = lhs_result.Extract();
          } else {
            return ExpectedAst::Failure("Invalid token type encountered following binary operator.\n");
          }
        }
      }
    }
  }
  // - Operand
  else if (c.Operation() == eOperation::None) {
    auto lhs_result = ParseOperand(c);
    if (not lhs_result) {
      return ExpectedAst::Failure(lhs_result.Error());
    }
    c.Advance(lhs_result);

    // 1. Postfix
    if (c.Operation() == eOperation::Postfix) {
      // 1.1. Postfix () -> Function Call
      if (c.TypeIs(LParen)) {
        auto scope = TkScope::FindParen(c);
        if (not scope) {
          return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
        }
        // Parse the arguments
        auto arguments_result = ParseArguments(c);
        if (not arguments_result) {
          return ExpectedAst::Failure(arguments_result.Error());
        }
        out_node = Ast(eAst::FunctionCall, "", lhs_result.Extract(), arguments_result.Extract());
        c.Advance(scope.End());
      }
      // 1.2. Postfix [] -> Index Operator
      else if (c.TypeIs(LBracket)) {
        auto scope = TkScope::FindBracket(c);
        if (not scope) {
          return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
        }
        auto arguments_result = ParseIndexingArguments(c);
        if (not arguments_result) {
          return ExpectedAst::Failure(arguments_result.Error());
        }
        out_node = Ast(eAst::IndexOperator, "", lhs_result.Extract(), arguments_result.Extract());
        c.Advance(scope.End());
      }
      // 1.3. Postfix {} -> Listing Operator
      else if (c.TypeIs(LBrace)) {
        auto scope = TkScope::FindBrace(c);
        if (not scope) {
          return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
        }
        auto arguments_result = ParseListingArguments(c);
        if (not arguments_result) {
          return ExpectedAst::Failure(arguments_result.Error());
        }
        out_node = Ast(eAst::ListingOperator, "", lhs_result.Extract(), arguments_result.Extract());
        c.Advance(scope.End());
      }
      // 1.3. Postfix Single Token Operator.
      else {
        out_node = Ast(c.Get());
        out_node.ExtractAndPush(lhs_result);
        c.Advance();
      }
    }
    // 2. Binary Operator after operand.
    else if (c.Operation() == eOperation::Binary) {
      out_node = Ast(c.Get());
      c.Advance();
      // Expecting an operand after a binary operator.
      // 2.1. Open Paren -> Subexpr
      if (c.TypeIs(LParen)) {
        auto scope = TkScope::FindParen(c);
        if (not scope) {
          return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
        } else {
          ExpectedAst rhs_result = ParseImpl({scope.ContainedBegin(), scope.ContainedEnd()});
          if (not rhs_result) {
            return rhs_result;
          }
          out_node.ExtractAndPush(lhs_result);
          out_node.ExtractAndPush(rhs_result);
          c.Advance(scope.End());
        }
      }
      // 2.2. Else it has to be a singular token operand.
      else {
        auto rhs_result = ParseOperand(c);
        if (not rhs_result) {
          return ExpectedAst::Failure(rhs_result.Error());
        }
        out_node.ExtractAndPush(lhs_result);
        out_node.ExtractAndPush(rhs_result);
        c.Advance(rhs_result);
      }
    }
    // 3. End of the expression.
    else {
      if (c.AtEnd()) {
        out_node = lhs_result.Extract();
      } else {
        return ExpectedAst::Failure("Invalid token type encountered following binary operator.\n");
      }
    }
  }
  // - Prefix
  else if (c.Operation() == eOperation::Prefix) {
    // Prefix Operator, must be followed by an operand.
    out_node = Ast(c.Get());
    c.Advance();

    if (c.AtEnd()) {
      return ExpectedAst::Failure(
          CaErr::ErrDetail<CaErr::UserSyntaxError>(c, CPP_LOC, "Prefix operator at end of expression."));
    }

    // 1. Open Paren -> Subexpr
    if (c.TypeIs(LParen)) {
      auto scope = TkScope::FindParen(c);
      if (not scope) {
        return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
      } else {
        ExpectedAst lhs_result = ParseImpl({scope.ContainedBegin(), scope.ContainedEnd()});
        if (not lhs_result) {
          return lhs_result;
        }
        out_node.ExtractAndPush(lhs_result);
        c.Advance(scope.End());
      }
    }
    // 2. Else it has to be a singular token operand.
    else if (c.Operation() == eOperation::None) {
      OffsetParseResult lhs_result = ParseOperand(c);
      if (not lhs_result) {
        return ExpectedAst::Failure(lhs_result.Error());
      }
      out_node.ExtractAndPush(lhs_result);
      c.Advance(lhs_result);
    }
    // 3. Error -> Invalid token type encountered following prefix operator.
    else {
      return ExpectedAst::Failure("Invalid token type encountered following prefix operator.\n");
    }

  }
  // Error -> Invalid token type at beginning of expression.
  else {
    return ExpectedAst::Failure("Invalid token type at beginning of primary expression.\n");
  }

  return ExpectedAst::Success(move(out_node));
}


} // namespace cnd

/// @} // end of cnd_compiler_data

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// Licensed under the Apache License, Version 2.0(the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////