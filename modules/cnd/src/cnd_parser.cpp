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

/// @addtogroup cnd_compiler
/// @{

#include "cnd_parser.hpp"

// clang-format off
#include "cnd_implicit.hpp"
#include "cnd_constdef.hpp"
#include "cnd_clerr.hpp"
#include "cnd_tk.hpp"
#include "cnd_cst.hpp"
//#include "cnd_tk_closure.hpp"
#include "cnd_tk_cursor.hpp"
//#include "cnd_tk_scope.hpp"
// clang-format on

//#define CND_MACRO_QUOTE "
//#define CND_MACRO_STRIGIFY(x) #x
//#define CND_MACRO_EXPAND(x) x
//#define CND_MACRO_STRING(x) CND_MACRO_EXPAND(CND_MACRO_QUOTE)##x"
//
//#define CND_CPP_SRCLOC std::source_location::current()
//#define CND_CPP_SRCLOC_FORMAT std::format("[{}][{}][{}][{}]",\
//Path(CND_CPP_SRCLOC.file_name()).filename(),\
//CND_CPP_SRCLOC.line(),\
//                       CND_CPP_SRCLOC.column(),\
//                       __FUNCSIG__)
//
//#define CND_ENABLE_COMPILER_DEBUG_DIAGNOSTICS 1
//#ifdef CND_ENABLE_COMPILER_DEBUG_DIAGNOSTICS
//  #if CND_ENABLE_COMPILER_DEBUG_DIAGNOSTICS == 1
//    #define CND_CLDBG_SRCLOC CND_CPP_SRCLOC_FORMAT
//  #else
//    #define CND_CLDBG_SRCLOC Str("")
//  #endif
//#endif  // CND_ENABLE_COMPILER_DEBUG_DIAGNOSTICS
//

// #define COMPILER_ERROR(e, ...) caoco::caerr::CaErr::ErrDetail<caoco::caerr::CaErr::e>(__VA_ARGS__)

namespace cnd::parser {
//using TkVec = Vec<Tk>;
//using TkVecConstIter = TkVec::const_iterator;
//
//using std::move;
//// Static const members of ExprParser.
//const Tk parser::ExprParser::kOpenParenTk = Tk{eTk::kLParen, "("};
//const Tk ExprParser::kCloseParenTk = Tk{eTk::kRParen, ")"};
//
//ClRes<Cst> ExprParser::Parse(const TkCursor& c) {
//  ClRes<TkVec> resolved_closures = CreateAndResolveClosures(c);
//  if (!resolved_closures) return Fail(move(resolved_closures.error()));
//
//  // Recursive descent parse the reduced expression.
//  ClRes<Cst> parsed_expr = ParseImpl(TkCursor{resolved_closures.value().cbegin(), resolved_closures.value().cend()});
//  if (!parsed_expr) return Fail(move(parsed_expr.error()));
//
//  return parsed_expr;
//}
//
//void ExprParser::FirstOperatorSwitch() {
//  if (is_first_operator_) {
//    is_first_operator_ = false;
//  }
//}
//
//void ExprParser::ResolvePrefix() {
//  // Find consecutive prefix closures.
//  auto nclosures = closure_buffer_
//                       .FindClosureReverseConsecutive([](ClosureListIterT a, ClosureListIterT b) -> bool {
//                         return (a->Priority() == b->Priority()) && (a->Operation() == b->Operation());
//                       })
//                       .size();
//  auto closure_count = nclosures;
//  while (nclosures > 0) {
//    closure_buffer_.StreamInsertBeforeClosure(std::prev(closure_buffer_.LastClosure(), nclosures - 1), kOpenParenTk);
//    closure_buffer_.StreamPushBack(kCloseParenTk);
//    nclosures--;
//  }
//
//  for (std::size_t i = 0; i < closure_count; i++) {
//    closure_buffer_.PopClosure();
//  }
//}
//
//void ExprParser::ResolvePostfix() {
//  // Find concecutive postfix closures.
//  auto nclosures = closure_buffer_
//                       .FindClosureReverseConsecutive([](ClosureListIterT a, ClosureListIterT b) -> bool {
//                         return (a->Priority() == b->Priority()) && (a->Operation() == b->Operation());
//                       })
//                       .size();
//  auto closure_count = nclosures;
//
//  while (nclosures > 0) {
//    closure_buffer_.StreamInsertAfterClosure(std::prev(closure_buffer_.LastClosure(), closure_count), kOpenParenTk);
//    closure_buffer_.StreamInsertAfterClosure(std::prev(closure_buffer_.LastClosure(), nclosures - 1), kCloseParenTk);
//    nclosures--;
//  }
//
//  for (std::size_t i = 0; i < closure_count; i++) {
//    closure_buffer_.PopClosure();
//  }
//};
//
//void ExprParser::ResolveBinaryLeftAssoc() {
//  std::vector<ClosureListIterT> binary_closures = closure_buffer_.FindClosureReverseConsecutiveAndIgnore(
//      [](ClosureListIterT a, ClosureListIterT b) -> bool {
//        return (a->Priority() == b->Priority()) && (a->Operation() == b->Operation()) && (a->Assoc() == b->Assoc());
//      },
//      [](ClosureListIterT a) -> bool {
//        return ((a->Priority() == ePriority::Postfix) || (a->Priority() == ePriority::Prefix));
//      });
//
//  // Resolve first closure.(last in the list)
//  ClosureListIterT open_paren_location = std::prev(binary_closures.back());
//
//  // skip any postfix/prefix closures when looking for the open paren loc.
//  while (open_paren_location->Priority() == ePriority::Postfix ||
//         open_paren_location->Priority() == ePriority::Prefix) {
//    open_paren_location = std::prev(open_paren_location);
//  }
//
//  closure_buffer_.StreamInsertAfterClosure(open_paren_location, {eTk::kLParen, "("});
//  closure_buffer_.StreamPushBack({eTk::kRParen, ")"});
//
//  // Resolve the rest,skip last in loop.
//  std::vector<ClosureListIterT>::iterator close_paren_location_iter = binary_closures.begin();
//  while (close_paren_location_iter != std::prev(binary_closures.end())) {
//    closure_buffer_.StreamInsertAfterClosure(open_paren_location, {eTk::kLParen, "("});
//    closure_buffer_.StreamInsertBeforeClosure(*close_paren_location_iter, {eTk::kRParen, ")"});
//    close_paren_location_iter = std::next(close_paren_location_iter);
//  }
//
//  // Pop all collected closures.
//  for (ClosureListIterT closure_iter : binary_closures) {
//    closure_buffer_.PopClosure(closure_iter);
//  }
//};
//
//void ExprParser::ResolveBinaryRightAssoc() {
//  std::vector<ClosureListIterT> binary_closures = closure_buffer_.FindClosureReverseConsecutiveAndIgnore(
//      [](ClosureListIterT a, ClosureListIterT b) -> bool {
//        return (a->Priority() == b->Priority()) && (a->Operation() == b->Operation()) && (a->Assoc() == b->Assoc());
//      },
//      [](ClosureListIterT a) -> bool {
//        return ((a->Priority() == ePriority::Postfix) || (a->Priority() == ePriority::Prefix));
//      });
//
//  // Resolve first closure.(last in the list)
//  closure_buffer_.StreamInsertAfterClosure(std::prev(binary_closures.back()), {eTk::kLParen, "("});
//  closure_buffer_.StreamPushBack({eTk::kRParen, ")"});
//
//  // Resolve the rest.
//  std::vector<ClosureListIterT>::iterator open_paren_location_iter = std::next(binary_closures.begin());
//  while (open_paren_location_iter != binary_closures.end()) {
//    closure_buffer_.StreamInsertAfterClosure(*open_paren_location_iter, {eTk::kLParen, "("});
//    closure_buffer_.StreamPushBack({eTk::kRParen, ")"});
//    open_paren_location_iter = std::next(open_paren_location_iter);
//  }
//
//  // Pop all collected closures.
//  for (ClosureListIterT closure_iter : binary_closures) {
//    closure_buffer_.PopClosure(closure_iter);
//  }
//};
//
//void ExprParser::ResolveBinary() {
//  // Resolve associative binary if there are repeated equivalent priority
//  // closures before the last.
//  if (closure_buffer_.LastClosure()->Priority() == std::prev(closure_buffer_.LastClosure())->Priority()) {
//    if (closure_buffer_.LastClosure()->Assoc() == eAssoc::Left) {
//      ResolveBinaryLeftAssoc();
//    } else if (closure_buffer_.LastClosure()->Assoc() == eAssoc::Right) {
//      ResolveBinaryRightAssoc();
//    } else {
//      throw "ResolveBinary: Invalid binary token in closure, association "
//            "not left or right.\n";
//    }
//  } else {  // Resolve Singular Binary Closure
//    // Special Case: If the binary closure is higher priority than prefix/postfix
//    //               -> Then don't only skip the postfix.
//
//    ClosureListIterT open_paren_insertion_loc_iter = std::prev(closure_buffer_.LastClosure());
//
//    // Lower:
//    if (closure_buffer_.LastClosure()->Priority() < ePriority::Postfix) {
//      // If there are postfix/prefix closures-> skip them when
//      // looking for the open paren insertion location.
//      ClosureListIterT postfix_it = std::prev(closure_buffer_.LastClosure());
//      while (postfix_it->Priority() == ePriority::Postfix) {
//        postfix_it = std::prev(postfix_it);
//      }
//
//      ClosureListIterT prefix_it = postfix_it;
//      if (prefix_it->Priority() == ePriority::Prefix) {
//        while (prefix_it->Priority() == ePriority::Prefix) {
//          prefix_it = std::prev(prefix_it);
//        }
//      }
//
//      open_paren_insertion_loc_iter = prefix_it;
//    }
//    // Higher(will never be equal):
//    else {
//      // If there are postfix/prefix closures-> skip them when
//      // looking for the open paren insertion location.
//      ClosureListIterT postfix_it = std::prev(closure_buffer_.LastClosure());
//      while (postfix_it->Priority() == ePriority::Postfix) {
//        postfix_it = std::prev(postfix_it);
//      }
//
//      open_paren_insertion_loc_iter = postfix_it;
//    }
//
//    closure_buffer_.StreamInsertAfterClosure(open_paren_insertion_loc_iter, {eTk::kLParen, "("});
//    closure_buffer_.StreamPushBack({eTk::kRParen, ")"});
//    closure_buffer_.PopClosure();
//  }
//};
//
//void ExprParser::ResolveLast() {
//  if (closure_buffer_.LastClosure()->Operation() == eOperation::Prefix) {
//    ResolvePrefix();
//  } else if (closure_buffer_.LastClosure()->Operation() == eOperation::Postfix) {
//    ResolvePostfix();
//  } else if (closure_buffer_.LastClosure()->Operation() == eOperation::Binary) {
//    ResolveBinary();
//  } else {
//    throw "ActionResolveLast: Invalid token type in closure.Must be an "
//               "operator.\n";
//  }
//}
//
//ClRes<Bool> ExprParser::ActionSkip(TkCursor& c) {
//  // Check for an open paren-> Subexpression
//  // Resolve the subexpr first.
//  // Push resolved result to output.
//  if (c.TypeIs(eTk::kLParen)) {
//    TkScope scope = TkScope::FindParen(c);
//    if (not scope) {
//      return "Mismatched parentheses in operand.";
//    } else {
//      // Resolve but !do not parse! the inside of the parentheses.
//      ExprParser resolved_subexpr_parser;
//      ClRes<TkVec> resolved_subexpr_result =
//          ExprParser().CreateAndResolveClosures({scope.ContainedBegin(), scope.ContainedEnd()});
//      if (!resolved_subexpr_result) {
//        return resolved_subexpr_result.error().PushError(CND_CLDBG_SRCLOC + "Could not resolve subexpression.");
//      }
//      const TkVec& resolved_subexpr = resolved_subexpr_result.value();
//      for (const Tk& tk : resolved_subexpr) {
//        closure_buffer_.StreamPushBack(tk);
//      }
//      for (TkVecConstIter i = c.Iter(); i != scope.End(); i++) {
//        c.Advance();
//      }
//      return true;
//    }
//  }
//  // Check if this is a singular token OPERAND.
//  if (c.Operation() == eOperation::NONE || c.TypeIs(eTk::kSub)) {
//    // If unary minus - Emplace unary minus into operand.
//    if (c.TypeIs(eTk::kSub)) {
//      c.Advance();
//      if (c.TypeIs(eTk::kLitInt) || c.TypeIs(eTk::kLitReal)) {
//        Tk negative_number = c.Get();
//        negative_number.LiteralMutable() = "-" + negative_number.Literal();
//        closure_buffer_.StreamPushBack(negative_number);
//        c.Advance();
//        return true;
//      } else {
//        return "ExprParser::ActionSkip: Unary minus must be "
//               "followed by a numeric literal.\n";
//      }
//    }
//    // Parse single token operand.
//    closure_buffer_.StreamPushBack(c.Get());
//    c.Advance();
//    return true;
//  }
//
//  return "ExprParser::ActionSkip: Could not read operand token, "
//         "invalid or unimplemented.";
//}
//
//ClRes<Bool> ExprParser::ActionStore(TkCursor& c) {
//  // Check for an open paren-> Function Call
//  // Arguments are parsed and resolved in ParseImpl.
//  // Push a closure for the function call.
//  if (c.TypeIs(eTk::LParen)) {
//    TkScope scope = TkScope::FindParen(c);
//    if (not scope) {
//      return "Mismatched parentheses in function call.";
//    } else {
//      closure_buffer_.StreamPushBack(c.Get());
//      TkListIter fn_args_closure_begin = closure_buffer_.LastStreamed();
//      c.Advance();
//      // Store the rest of the function call.
//      for (TkVectorConstIter i = c.Iter(); i != scope.End(); i++) {
//        closure_buffer_.StreamPushBack(c.Get());
//        c.Advance();
//      }
//      // Make the closure.
//      closure_buffer_.PushBackClosure(fn_args_closure_begin, closure_buffer_.LastStreamed());
//      return true;
//    }
//  }
//  // Check for an open bracket-> Indexing Operator
//  if (c.TypeIs(eTk::LBracket)) {
//    TkScope scope = TkScope::FindBracket(c);
//    if (not scope) {
//      return "Mismatched bracket in indexing call.";
//    } else {
//      closure_buffer_.StreamPushBack(c.Get());
//      TkListIter fn_args_closure_begin = closure_buffer_.LastStreamed();
//      c.Advance();
//      for (TkVectorConstIter i = c.Iter(); i != scope.End(); i++) {
//        closure_buffer_.StreamPushBack(c.Get());
//        c.Advance();
//      }
//      closure_buffer_.PushBackClosure(fn_args_closure_begin, closure_buffer_.LastStreamed());
//      return true;
//    }
//  }
//  // Listing Operator
//  if (c.TypeIs(eTk::LBrace)) {
//    TkScope scope = TkScope::FindBrace(c);
//    if (not scope) {
//      return "Mismatched brace in listing call.";
//    } else {
//      closure_buffer_.StreamPushBack(c.Get());
//      TkListIter fn_args_closure_begin = closure_buffer_.LastStreamed();
//      c.Advance();
//      for (TkVectorConstIter i = c.Iter(); i != scope.End(); i++) {
//        closure_buffer_.StreamPushBack(c.Get());
//        c.Advance();
//      }
//      closure_buffer_.PushBackClosure(fn_args_closure_begin, closure_buffer_.LastStreamed());
//      return true;
//    }
//  }
//  // Single Token Operator.
//  if (c.Operation() == eOperation::Binary || c.Operation() == eOperation::Prefix ||
//      c.Operation() == eOperation::Postfix) {
//    closure_buffer_.StreamPushBack(c.Get());
//    closure_buffer_.PushBackClosure(closure_buffer_.LastStreamed());
//    c.Advance();
//    return true;
//  }
//
//  return "ExprParser::ActionStore: Could not read operator "
//         "token, "
//         "invalid or unimplemented.";
//}
//
//ClRes<Bool> ExprParser::ActionCheck(TkCursor& c) {
//  if (c.AtEnd()) {
//    while (closure_buffer_.ClosureCount() > 0) {
//      ResolveLast();
//    }
//    return true;
//  } else {
//    if (c.Priority() >= closure_buffer_.LastClosure()->Priority()) {
//      // If: (Head >= Last Closure) => Store
//      ActionStore(c);
//    } else {
//      // While:( Head < Last Closure ) => Resolve Next
//      while (c.Priority() < closure_buffer_.LastClosure()->Priority()) {
//        ResolveLast();
//      }
//      ActionStore(c);
//    }
//    return true;
//  }
//}
//
//ClRes<Bool> ExprParser::ChooseAction(TkCursor& c) {
//  if (c.AtEnd()) {
//    cxx::BoolError check_result = ActionCheck(c);
//    if (check_result) {
//      is_resolved_ = true;
//      return true;
//    } else {
//      return "ExprParser::ChooseAction: Could not resolve "
//             "closures.\n" +
//             check_result.Error();
//    }
//  }
//  // Choose action based on expected head token.
//  else {
//    // Operative-> Looking for an operand,prefix operator,open
//    // scope(subexpr)
//    // SPECIAL CASE[Unary Minus] -> If binary '-' is encountered.
//    //                           -> Bind tightly to the following operand.
//    //                           -> Assert Numeric Literal. Emplace - into
//    //                           operand.
//    //                           -> Skip Both.
//    //                         // If the following is non number, error.
//    if (next_expected_head_token_ == eNextExpectedHeadToken::kOperative) {
//      if (c.Operation() == eOperation::None || c.TypeIs(eTk::LParen) || c.TypeIs(eTk::Sub)) {
//        cxx::BoolError skip_result = ActionSkip(c);
//        if (skip_result) {
//          next_expected_head_token_ = eNextExpectedHeadToken::kOperator;
//          return true;
//        } else {
//          return "ExprParser::ChooseAction: Could not skip "
//                 "operand.\n" +
//                 skip_result.Error();
//        }
//      }
//      // Prefix Operator -> Check Single Prefix Operator.
//      else if (c.Operation() == eOperation::Prefix) {
//        if (is_first_operator_) {
//          cxx::BoolError store_result = ActionStore(c);
//          if (not store_result) {
//            return "ExprParser::ChooseAction: Could not store "
//                   "prefix operator.\n" +
//                   store_result.Error();
//          }
//        } else {
//          cxx::BoolError check_result = ActionCheck(c);
//          if (not check_result) {
//            return "ExprParser::ChooseAction: Could not check "
//                   "prefix operator.\n" +
//                   check_result.Error();
//          }
//        }
//        FirstOperatorSwitch();
//        next_expected_head_token_ = eNextExpectedHeadToken::kOperative;
//        return true;
//      }
//      // expected operand got operator -> user error
//      else if (c.Operation() == eOperation::Binary || c.Operation() == eOperation::Postfix) {
//        return "ExprParser::ChooseAction: Invalid token type "
//               "encountered, operator following operator.\n";
//      }
//      // Else user error -> this token does not belong in a value
//      // expression.
//      else {
//        return "ExprParser::ChooseAction: "
//               "Token type not valid for a primary expression.\n";
//      }
//    }
//    // Operator
//    // Looking for a binary operator, postfix operator, or open
//    // paren(function call), or open brace(index operator)
//    else if (next_expected_head_token_ == eNextExpectedHeadToken::kOperator) {
//      // Open Paren, Open Bracket, Postfix Operator
//      //    -> Check, next is Operator.
//      if (c.TypeIs(eTk::LParen) || c.TypeIs(eTk::LBracket) || c.TypeIs(eTk::LBrace) ||
//          c.Operation() == eOperation::Postfix) {
//        cxx::BoolError action_result;
//        if (is_first_operator_) {
//          action_result = ActionStore(c);
//          FirstOperatorSwitch();
//          next_expected_head_token_ = eNextExpectedHeadToken::kOperator;
//        } else {
//          action_result = ActionCheck(c);
//          FirstOperatorSwitch();
//          next_expected_head_token_ = eNextExpectedHeadToken::kOperator;
//        }
//      }
//      // Binary Operator -> Check, next is Operative.
//      else if (c.Operation() == eOperation::Binary) {
//        cxx::BoolError action_result;
//        if (is_first_operator_) {
//          action_result = ActionStore(c);
//        } else {
//          action_result = ActionCheck(c);
//        }
//        FirstOperatorSwitch();
//        next_expected_head_token_ = eNextExpectedHeadToken::kOperative;
//      }
//      // Prefix -> user Error, prefix following operand.
//      else if (c.Operation() == eOperation::Prefix) {
//        return "ExprParser::ChooseAction: Invalid token type "
//               "encountered, prefix following operand.\n";
//      }
//      // Operand -> user Error, operand following operand.
//      else if (c.Operation() == eOperation::None) {
//        return "ExprParser::ChooseAction: Invalid token type "
//               "encountered, operand following operand.\n";
//      }
//      // Else user error -> this token does not belong in a value
//      // expression.
//      else {
//        return "ExprParser::ChooseAction: "
//               "Token type not valid for a primary expression.\n";
//      }
//    }
//    // This should never happen -> critical logic error.
//    // eNextExpectedHeadToken enum set to invalid value.
//    else {
//      throw "[CRITICAL LOGIC ERROR] ExprParser::ChooseAction: "
//                "Invalid next expected head token enumeration type.\n";
//    }
//  }
//}
//
//ClRes<TkVec> ExprParser::CreateAndResolveClosures(TkCursor c) {
//  while (!is_resolved_) {
//    auto action_result = ChooseAction(c);
//    if (!action_result) return Fail(move(action_result.error()));   
//  }
//  return closure_buffer_.StreamToVector();
//}
//
//ClRes<Cst> ExprParser::ParseImpl(TkCursor c) {
//  using enum eTk;
//  Cst out_node;
//
//  if (c.TypeIs(kLParen)) {
//    auto scope = TkScope::FindParen(c);
//    if (not scope) {
//      return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//    } else {
//      // 1. Check for redundant paren-> Parse the inside of the paren instead.
//      if (scope.End() == c.End()) {
//        return ParseImpl({scope.ContainedBegin(), scope.ContainedEnd()});
//      }
//      // 2. Else scope is an operand contained in a subexpression.
//      else {
//        ExpectedAst lhs_result = ParseImpl({scope.ContainedBegin(), scope.ContainedEnd()});
//        if (not lhs_result) {
//          return lhs_result;
//        };
//        c.Advance(scope.End());
//
//        // 2.1. Postfix
//        if (c.Operation() == eOperation::Postfix) {
//          // 2.1.1. Postfix () -> Function Call
//          if (c.TypeIs(LParen)) {
//            auto scope = TkScope::FindParen(c);
//            if (not scope) {
//              return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//            }
//            // Parse the arguments
//            auto arguments_result = ParseArguments(c);
//            if (not arguments_result) {
//              return ExpectedAst::Failure(arguments_result.Error());
//            }
//            out_node = Cst(eAst::FunctionCall, "", lhs_result.Extract(), arguments_result.Extract());
//            c.Advance(scope.End());
//          }
//          // 2.1.2. Postfix [] -> Index Operator
//          else if (c.TypeIs(LBracket)) {
//            auto scope = TkScope::FindBracket(c);
//            if (not scope) {
//              return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//            }
//            auto arguments_result = ParseIndexingArguments(c);
//            if (not arguments_result) {
//              return ExpectedAst::Failure(arguments_result.Error());
//            }
//            out_node = Cst(eAst::IndexOperator, "", lhs_result.Extract(), arguments_result.Extract());
//            c.Advance(scope.End());
//          }
//          // 2.1.3. Postfix {} -> Listing Operator
//          else if (c.TypeIs(LBrace)) {
//            auto scope = TkScope::FindBrace(c);
//            if (not scope) {
//              return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//            }
//            auto arguments_result = ParseListingArguments(c);
//            if (not arguments_result) {
//              return ExpectedAst::Failure(arguments_result.Error());
//            }
//            out_node = Cst(eAst::ListingOperator, "", lhs_result.Extract(), arguments_result.Extract());
//            c.Advance(scope.End());
//          }
//          // 2.1.4. Postfix Single Token Operator.
//          else {
//            out_node = Cst(c.Get());
//            out_node.ExtractAndPush(lhs_result);
//            c.Advance();
//          }
//        }
//        // 2.2. Binary Operator
//        else if (c.Operation() == eOperation::Binary) {
//          out_node = Cst(c.Get());
//          c.Advance();
//          // Expecting an operand after a binary operator.
//          // 2.2.1. Open Paren -> Subexpr
//          if (c.TypeIs(LParen)) {
//            auto scope = TkScope::FindParen(c);
//            if (not scope) {
//              return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//            } else {
//              ExpectedAst rhs_result = ParseImpl({scope.ContainedBegin(), scope.ContainedEnd()});
//              if (not rhs_result) {
//                return rhs_result;
//              }
//              out_node.ExtractAndPush(lhs_result);
//              out_node.ExtractAndPush(rhs_result);
//              c.Advance(scope.End());
//            }
//          }
//          // 2.2.2. Else it has to be a singular token operand.
//          else {
//            OffsetParseResult rhs_result = ParseOperand(c);
//            if (not rhs_result) {
//              return ExpectedAst::Failure(rhs_result.Error());
//            }
//            out_node.ExtractAndPush(lhs_result);
//            out_node.ExtractAndPush(rhs_result);
//            c.Advance(rhs_result);
//          }
//        }
//        // 2.3 End of expr
//        else {
//          if (c.AtEnd()) {
//            out_node = lhs_result.Extract();
//          } else {
//            return ExpectedAst::Failure("Invalid token type encountered following binary operator.\n");
//          }
//        }
//      }
//    }
//  }
//  // - Operand
//  else if (c.Operation() == eOperation::None) {
//    auto lhs_result = ParseOperand(c);
//    if (not lhs_result) {
//      return ExpectedAst::Failure(lhs_result.Error());
//    }
//    c.Advance(lhs_result);
//
//    // 1. Postfix
//    if (c.Operation() == eOperation::Postfix) {
//      // 1.1. Postfix () -> Function Call
//      if (c.TypeIs(LParen)) {
//        auto scope = TkScope::FindParen(c);
//        if (not scope) {
//          return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//        }
//        // Parse the arguments
//        auto arguments_result = ParseArguments(c);
//        if (not arguments_result) {
//          return ExpectedAst::Failure(arguments_result.Error());
//        }
//        out_node = Cst(eAst::FunctionCall, "", lhs_result.Extract(), arguments_result.Extract());
//        c.Advance(scope.End());
//      }
//      // 1.2. Postfix [] -> Index Operator
//      else if (c.TypeIs(LBracket)) {
//        auto scope = TkScope::FindBracket(c);
//        if (not scope) {
//          return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//        }
//        auto arguments_result = ParseIndexingArguments(c);
//        if (not arguments_result) {
//          return ExpectedAst::Failure(arguments_result.Error());
//        }
//        out_node = Cst(eAst::IndexOperator, "", lhs_result.Extract(), arguments_result.Extract());
//        c.Advance(scope.End());
//      }
//      // 1.3. Postfix {} -> Listing Operator
//      else if (c.TypeIs(LBrace)) {
//        auto scope = TkScope::FindBrace(c);
//        if (not scope) {
//          return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//        }
//        auto arguments_result = ParseListingArguments(c);
//        if (not arguments_result) {
//          return ExpectedAst::Failure(arguments_result.Error());
//        }
//        out_node = Cst(eAst::ListingOperator, "", lhs_result.Extract(), arguments_result.Extract());
//        c.Advance(scope.End());
//      }
//      // 1.3. Postfix Single Token Operator.
//      else {
//        out_node = Cst(c.Get());
//        out_node.ExtractAndPush(lhs_result);
//        c.Advance();
//      }
//    }
//    // 2. Binary Operator after operand.
//    else if (c.Operation() == eOperation::Binary) {
//      out_node = Cst(c.Get());
//      c.Advance();
//      // Expecting an operand after a binary operator.
//      // 2.1. Open Paren -> Subexpr
//      if (c.TypeIs(LParen)) {
//        auto scope = TkScope::FindParen(c);
//        if (not scope) {
//          return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//        } else {
//          ExpectedAst rhs_result = ParseImpl({scope.ContainedBegin(), scope.ContainedEnd()});
//          if (not rhs_result) {
//            return rhs_result;
//          }
//          out_node.ExtractAndPush(lhs_result);
//          out_node.ExtractAndPush(rhs_result);
//          c.Advance(scope.End());
//        }
//      }
//      // 2.2. Else it has to be a singular token operand.
//      else {
//        auto rhs_result = ParseOperand(c);
//        if (not rhs_result) {
//          return ExpectedAst::Failure(rhs_result.Error());
//        }
//        out_node.ExtractAndPush(lhs_result);
//        out_node.ExtractAndPush(rhs_result);
//        c.Advance(rhs_result);
//      }
//    }
//    // 3. End of the expression.
//    else {
//      if (c.AtEnd()) {
//        out_node = lhs_result.Extract();
//      } else {
//        return ExpectedAst::Failure("Invalid token type encountered following binary operator.\n");
//      }
//    }
//  }
//  // - Prefix
//  else if (c.Operation() == eOperation::Prefix) {
//    // Prefix Operator, must be followed by an operand.
//    out_node = Cst(c.Get());
//    c.Advance();
//
//    if (c.AtEnd()) {
//      return ExpectedAst::Failure(
//          CaErr::ErrDetail<CaErr::UserSyntaxError>(c, CPP_LOC, "Prefix operator at end of expression."));
//    }
//
//    // 1. Open Paren -> Subexpr
//    if (c.TypeIs(LParen)) {
//      auto scope = TkScope::FindParen(c);
//      if (not scope) {
//        return ExpectedAst::Failure(CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//      } else {
//        ExpectedAst lhs_result = ParseImpl({scope.ContainedBegin(), scope.ContainedEnd()});
//        if (not lhs_result) {
//          return lhs_result;
//        }
//        out_node.ExtractAndPush(lhs_result);
//        c.Advance(scope.End());
//      }
//    }
//    // 2. Else it has to be a singular token operand.
//    else if (c.Operation() == eOperation::None) {
//      OffsetParseResult lhs_result = ParseOperand(c);
//      if (not lhs_result) {
//        return ExpectedAst::Failure(lhs_result.Error());
//      }
//      out_node.ExtractAndPush(lhs_result);
//      c.Advance(lhs_result);
//    }
//    // 3. Error -> Invalid token type encountered following prefix operator.
//    else {
//      return ExpectedAst::Failure("Invalid token type encountered following prefix operator.\n");
//    }
//
//  }
//  // Error -> Invalid token type at beginning of expression.
//  else {
//    return ExpectedAst::Failure("Invalid token type at beginning of primary expression.\n");
//  }
//
//  return ExpectedAst::Success(move(out_node));
//}
//









//
/////////////////////////////////////////////////////////////////////////////////
//// Internal parsing methods impl
/////////////////////////////////////////////////////////////////////////////////
//
// OffsetParseResult parser::ParseOperand(TkCursor c) {
//  using namespace caerr;
//  if (c.IsAnOperand())
//    return Success(c.Next(), c.Get());
//  else
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//};
//
// OffsetParseResult parser::ParseArguments(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//  if (c.TypeIs(LParen)) {
//    if (c.Next().TypeIs(RParen)) {  // Empty Arg.
//      return Success(c.Advance(2), Cst(eAst::Arguments));
//    }
//
//    TkScope::Vector arg_scopes = TkScope::FindSeperatedParen(c, Comma);
//
//    if (not arg_scopes.front()) {
//      return Fail(c, CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//    } else {
//      Cst arguments_node = eAst::Arguments;
//      for (const TkScope& arg_scope : arg_scopes) {
//        ExpectedAst arg_result = ParseExpr(arg_scope.Contained());
//        if (not arg_result) {
//          return Fail(c, arg_result.Error());
//        }
//        arguments_node.ExtractAndPush(arg_result);
//      }
//      return Success(c.Advance(arg_scopes.back().End()), move(arguments_node));
//    }
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//}
//
// OffsetParseResult parser::ParseIndexingArguments(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//  if (c.TypeIs(LBracket)) {
//    // Check for empty arguments.
//    if (c.Next().TypeIs(RBracket)) {
//      return Success(c.Advance(2), eAst::Arguments);
//    }
//
//    TkScope::Vector arg_scopes = TkScope::FindSeperatedBracket(c, Comma);
//    if (not arg_scopes.front()) {
//      return Fail(c, CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//    } else {
//      Cst arguments_node(eAst::Arguments);
//      for (const TkScope& arg_scope : arg_scopes) {
//        ExpectedAst arg_result = ParseExpr({arg_scope.ContainedBegin(), arg_scope.ContainedEnd()});
//        if (not arg_result) {
//          return Fail(c, arg_result.Error());
//        }
//        arguments_node.ExtractAndPush(arg_result);
//      }
//      return Success(c.Advance(arg_scopes.back().End()), move(arguments_node));
//    }
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//}
//
// OffsetParseResult parser::ParseListingArguments(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//  if (c.TypeIs(LBrace)) {
//    // Check for empty arguments.
//    if (c.Next().TypeIs(RBrace)) {
//      return Success(c.Advance(2), eAst::Arguments);
//    }
//
//    TkScope::Vector arg_scopes = TkScope::FindSeperatedBrace(c, Comma);
//    if (not arg_scopes.front()) {
//      return Fail(c, CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//    } else {
//      Cst arguments_node(eAst::Arguments);
//      for (const TkScope& arg_scope : arg_scopes) {
//        ExpectedAst arg_result = ParseExpr({arg_scope.ContainedBegin(), arg_scope.ContainedEnd()});
//        if (not arg_result) {
//          return Fail(c, arg_result.Error());
//        }
//        arguments_node.ExtractAndPush(arg_result);
//      }
//      return Success(c.Advance(arg_scopes.back().End()), move(arguments_node));
//    }
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//}
//
// OffsetParseResult parser::ParsePrimaryStatement(TkCursor c) {
//  using namespace caerr;
//  if (c.IsPrimary()) {
//    TkScope statement_scope = TkScope::FindProgramStatement(c);
//    if (statement_scope) {
//      ExpectedAst statement_result = ParseExpr({statement_scope.Begin(), statement_scope.ContainedEnd()});
//      if (not statement_result) {
//        return Fail(c, statement_result.Error());
//      }
//      return Success(c.Advance(statement_scope.End()), statement_result.Extract());
//    } else {
//      return Fail(c, CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//    }
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//}
//
// OffsetParseResult parser::ParseConditionalSubExpression(TkCursor c) {
//  using namespace caerr;
//  TkScope paren_scope = TkScope::FindParen(c);
//  if (paren_scope) {
//    if (paren_scope.ContainedBegin()->IsPrimary()) {
//      ExpectedAst subexpr_result = ParseExpr({paren_scope.ContainedBegin(), paren_scope.ContainedEnd()});
//      if (not subexpr_result) {
//        return Fail(c, subexpr_result.Error());
//      }
//      return Success(c.Advance(paren_scope.End()), subexpr_result.Extract());
//    } else {
//      return Fail(c,
//                  CaErr::ErrDetail<CaErr::UserSyntaxError>(c, CPP_LOC, "Invalid start of conditional
//                  sub-expression."));
//    }
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//  }
//}
//
// OffsetParseResult parser::ParsePrimaryPreIdentifier(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//  if (c.IsPrimary()) {
//    TkScope statement_scope = TkScope::FindOpenStatement(c.Get().Type(), {eTk::CommercialAt}, c.Iter(), c.End());
//    if (statement_scope) {
//      ExpectedAst statement_result = ParseExpr({statement_scope.Begin(), statement_scope.ContainedEnd()});
//      if (not statement_result) {
//        return Fail(c, statement_result.Error());
//      }
//      return Success(c.Advance(statement_scope.End()), statement_result.Extract());
//    } else {
//      return Fail(c, CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//    }
//  } else {
//    // Else the type is implicitly an any type.
//    // Expecting a @
//    if (c.TypeIs(eTk::CommercialAt)) {
//      return Success(c.Advance(), eAst::KwAny);
//    }
//
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//}
//
// OffsetParseResult parser::ParsePrimaryPostIdentifier(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//  if (c.IsAnOperand() || c.IsSingularPrefixOperator() || c.TypeIs(LParen)) {
//    TkScope statement_scope = TkScope::FindOpenStatement(c.Get().Type(), {Colon, Semicolon}, c.Iter(), c.End());
//    if (statement_scope) {
//      ExpectedAst statement_result = ExprParser::Perform({statement_scope.Begin(), statement_scope.ContainedEnd()});
//      if (not statement_result) {
//        return Fail(c, statement_result.Error());
//      }
//      // !SPECIAL CASE: if end of statement is a semicolon, go 1 back.
//      // This will allow the callee to determine if the statement is
//      // terminated or not.
//      if (statement_scope.ContainedEnd()->TypeIs(Semicolon)) {
//        return Success(c.Advance(statement_scope.ContainedEnd()), statement_result.Extract());
//      }
//
//      return Success(c.Advance(statement_scope.End()), statement_result.Extract());
//    } else {
//      return Fail(c, CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//    }
//  } else {
//    // Implicit any type.
//    if (c.TypeIs(Colon)) {
//      return Success(c.Advance(), eAst::KwAny);
//    }
//
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//}
//
// OffsetParseResult parser::ParseModifiers(TkCursor c) {
//  using namespace caerr;
//  if (c.IsModifierKeyword()) {
//    Cst mod_node(eAst::Modifiers);
//    while (c.IsModifierKeyword()) {
//      mod_node.PushBack(c.Get());
//      c.Advance();
//    }
//    return Success(c, move(mod_node));
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//}
//
// OffsetParseResult parser::ParseReturnStmt(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//  if (c.TypeIs(KwReturn)) {
//    c.Advance();
//
//    if (c.TypeIs(Semicolon)) {
//      Cst return_statement_node{eAst::KwReturn};
//      return Success(c.Advance(), move(return_statement_node));
//    }
//
//    auto value_expr_result = ParsePrimaryStatement(c);
//    if (not value_expr_result) {
//      return value_expr_result;
//    }
//    Cst return_statement_node(eAst::KwReturn);
//    return_statement_node.ExtractAndPush(value_expr_result);
//    return Success(c, move(return_statement_node));
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//}
//
// OffsetParseResult parser::ParsePragmaticStmt(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//
//  // Format: <modifiers?><decl keyword>
//  if (c.TypeIs(Semicolon)) {
//    std::cout << "[C&][Warning] Empty statement in code. Line: " << c.Line();
//    return Success(c.Advance(), eAst::NONE);
//  }
//
//  if (c.IsDeclarativeKeyword()) {
//    switch (c.Type()) {
//      case KwUse:
//        return ParseUsingDecl(c);
//      case KwMain:
//        return ParseMainDecl(c);
//      case KwImport:
//        return ParseImportDecl(c);
//      case KwEnum:
//        return ParseEnumDecl(c);
//      default:
//        break;
//    }
//  }
//
//  TkCursor start_of_decl = c;
//  while (c.IsModifierKeyword()) {
//    c.Advance();
//  }
//
//  if (c.IsDeclarativeKeyword()) {
//    switch (c.Type()) {
//      case KwDef:
//        return ParseVariableDecl(c);
//      case KwFn:
//        return ParseMethodDecl(c);
//      case KwClass:
//        return ParseClassDecl(c);
//      case KwLib:
//        return ParseLibDecl(c);
//      case KwUse:
//      case KwMain:
//        return Fail(c, CaErr::ErrDetail<CaErr::UserSyntaxError>(
//                           c, CPP_LOC, c.Literal() + " declaration keyword cannot be modified."));
//      default:
//        return Fail(c, CaErr::ErrDetail<CaErr::UserSyntaxError>(c, CPP_LOC,
//                                                                c.Literal() + " declaration keyword not permitted "
//                                                                              "inside pragmatic code block."));
//    }
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ExpectedPragmaticDeclaration>(c, CPP_LOC));
//  }
//}
//
// OffsetParseResult parser::ParseFunctionalStmt(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//
//  if (c.TypeIs(Semicolon)) {
//    std::cout << "[C&][Warning] Empty statement in code. Line: " << c.Line();
//    return Success(c.Advance(), eAst::NONE);
//  }
//
//  if (c.IsDeclarativeKeyword()) {
//    switch (c.Type()) {
//      case KwIf:
//        return ParseIfDecl(c);
//      case KwWhile:
//        return ParseWhileDecl(c);
//      case KwFor:
//        return ParseForDecl(c);
//      case KwReturn:
//        return ParseReturnStmt(c);
//      case KwUse:
//        return ParseUsingDecl(c);
//      case KwImport:
//        return ParseImportDecl(c);
//      default:
//        break;
//    }
//  }
//
//  TkCursor start_of_decl = c;
//  while (c.IsModifierKeyword()) {
//    c.Advance();
//  }
//
//  if (c.IsDeclarativeKeyword()) {
//    switch (c.Type()) {
//      case KwDef:
//        return ParseVariableDecl(c);
//      case KwFn:
//        return ParseMethodDecl(c);
//      case KwClass:
//        return ParseClassDecl(c);
//      case KwUse:
//        return Fail(c, CaErr::ErrDetail<CaErr::UserSyntaxError>(
//                           c, CPP_LOC, c.Literal() + " declaration keyword cannot be modified."));
//      default:
//        return Fail(c, CaErr::ErrDetail<CaErr::UserSyntaxError>(c, CPP_LOC,
//                                                                c.Literal() + " declaration keyword not permitted "
//                                                                              "inside functional code block."));
//    }
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ExpectedPragmaticDeclaration>(c, CPP_LOC));
//  }
//}
//
// OffsetParseResult parser::ParseConditionalStmt(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//
//  if (c.IsDeclarativeKeyword()) {
//    switch (c.Type()) {
//      case KwUse:
//        return ParseUsingDecl(c);
//      case KwImport:
//        return ParseImportDecl(c);
//      default:
//        break;
//    }
//  }
//
//  TkCursor start_of_decl = c;
//  while (c.IsModifierKeyword()) {
//    c.Advance();
//  }
//
//  if (c.IsDeclarativeKeyword()) {
//    switch (c.Type()) {
//      case KwDef:
//        return ParseVariableDecl(c);
//      case KwFn:
//        return ParseMethodDecl(c);
//      case KwClass:
//        return ParseClassDecl(c);
//      case KwUse:
//        return Fail(c, CaErr::ErrDetail<CaErr::UserSyntaxError>(
//                           c, CPP_LOC, c.Literal() + " declaration keyword cannot be modified."));
//      default:
//        return Fail(c, CaErr::ErrDetail<CaErr::UserSyntaxError>(c, CPP_LOC,
//                                                                c.Literal() + " declaration keyword not permitted "
//                                                                              "inside conditional code block."));
//    }
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ExpectedPragmaticDeclaration>(c, CPP_LOC));
//  }
//}
//
// OffsetParseResult parser::ParseIfDecl(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//
//  static const LAMBDA xParseIfElifElse = [&c](eAst ast_type) {
//    if (c.TypeIs(KwIf) or c.TypeIs(KwElif)) {
//      c.Advance();
//      auto condition_result = ParseConditionalSubExpression(c);
//      if (not condition_result) {
//        return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(LParen, c));
//      }
//      c.Advance(condition_result);
//      // Parse the body.
//      auto body_result = ParseMethodDef(c);
//      if (not body_result) {
//        return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(LBrace, c));
//      }
//      c.Advance(body_result);
//      return Success(c, Cst(ast_type, "", condition_result.Extract(), body_result.Extract()));
//    } else if (c.TypeIs(KwElse)) {
//      c.Advance();
//      auto body_result = ParseMethodDef(c);
//      if (not body_result) {
//        return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(LBrace, c));
//      }
//      c.Advance(body_result);
//      return Success(c, Cst(ast_type, "", body_result.Extract()));
//    } else {
//      return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC, "Expected if or elif or else."));
//    }
//  };
//
//  if (c.TypeIsnt(KwIf)) {
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//
//  Cst ifelifelse_statement = eAst::IfStatement;
//  auto if_statement = xParseIfElifElse(eAst::KwIf);
//  if (not if_statement) {
//    return if_statement;
//  }
//  ifelifelse_statement.ExtractAndPush(if_statement);
//
//  if (c.TypeIs(Semicolon)) {
//    return Success(c.Advance(), move(ifelifelse_statement));
//  }
//
//  while (c.TypeIs(KwElif)) {
//    auto elif_statement = xParseIfElifElse(eAst::KwElif);
//    if (not elif_statement) {
//      return elif_statement;
//    }
//    ifelifelse_statement.ExtractAndPush(elif_statement);
//  }
//
//  if (c.TypeIs(KwElse)) {
//    auto else_statement = xParseIfElifElse(eAst::KwElse);
//    if (not else_statement) {
//      return else_statement;
//    }
//    ifelifelse_statement.ExtractAndPush(else_statement);
//  }
//
//  return Success(c, move(ifelifelse_statement));
//}
//
// OffsetParseResult parser::ParseWhileDecl(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//  // While Keyword, Conditional subexpression, Functional Block, Semicolon.
//  if (c.TypeIsnt(KwWhile)) {
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//  c.Advance();
//
//  auto condition_result = ParseConditionalSubExpression(c);
//  if (not condition_result) {
//    return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(LParen, c));
//  }
//  c.Advance(condition_result);
//
//  auto body_result = ParseMethodDef(c);
//  if (not body_result) {
//    return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(LBrace, c));
//  }
//  c.Advance(body_result);
//
//  if (c.TypeIs(eTk::Semicolon)) {
//    return Success(c.Advance(), Cst(eAst::KwWhile, "", condition_result.Extract(), body_result.Extract()));
//  }
//
//  return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Semicolon, c));
//}
//
// OffsetParseResult parser::ParseForDecl(TkCursor c) {
//  using namespace caerr;
//
//  if (c.TypeIsnt(eTk::KwFor)) {
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//  c.Advance();
//
//  auto condition_scope = TkScope::FindParen(c);
//  if (not condition_scope) {
//    return Fail(c, CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//  }
//
//  TkScope::Vector condition_scopes = TkScope::FindSeperatedParen(c, eTk::Semicolon);
//  if (condition_scopes.size() > 3) {
//    return Fail(
//        c, CaErr::ErrDetail<CaErr::InvalidForLoopSyntax>(
//               c, CPP_LOC,
//               "For condition may have a maximum of 3 statements.Detected:" +
//               std::to_string(condition_scopes.size())));
//  }
//
//  auto init_var_result = ParseVariableDecl({condition_scopes[0].ContainedBegin(), condition_scopes[0].End()});
//  if (not init_var_result) {
//    return init_var_result;
//  }
//
//  auto condition_result = ParsePrimaryStatement({condition_scopes[1].ContainedBegin(), condition_scopes[1].End()});
//  if (not condition_result) {
//    return condition_result;
//  }
//
//  auto increment_result = ExprParser::Perform({condition_scopes[2].ContainedBegin(), condition_scope.ContainedEnd()});
//  if (not increment_result) {
//    return Fail(c, increment_result.Error());
//  }
//  c.Advance(condition_scope.End());
//
//  auto body_result = ParseMethodDef(c);
//  if (not body_result) {
//    return body_result;
//  }
//  c.Advance(body_result);
//
//  if (c.TypeIs(eTk::Semicolon)) {
//    return Success(c.Advance(), Cst(eAst::KwFor, "", init_var_result.Extract(), condition_result.Extract(),
//                                    increment_result.Extract(), body_result.Extract()));
//  }
//
//  return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(eTk::Semicolon, c));
//}
//
// OffsetParseResult parser::ParseUsingDecl(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//  if (c.TypeIs(KwUse)) {
//    c.Advance();
//    // Next may be:
//    // - @ commercial at -> Type Alias
//    // - @name: lib-> Library Type Alias.
//    if (c.TypeIs(CommercialAt)) {
//      c.Advance();
//      if (c.TypeIsnt(Ident)) {
//        return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Ident, c));
//      }
//      Cst typedef_identifier = Cst(c.Get());
//      c.Advance();
//
//      if (c.TypeIsnt(Colon)) {
//        return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Colon, c));
//      }
//      c.Advance();
//
//      // If next is lib, then it is a library type alias.
//      // Everything following lib must be a value expression closed by a
//      // semicolon. Wether it is reduced to a type is determined at a later
//      // stage. Builds a kLibraryTypeAlias else it is a type alias.
//      if (c.TypeIs(KwLib)) {
//        c.Advance();
//        auto value_expr_result = ParsePrimaryStatement(c);
//        if (not value_expr_result) {
//          return value_expr_result;
//        }
//        c.Advance(value_expr_result);
//        return Success(c, Cst(eAst::LibraryTypeAlias, "", typedef_identifier, value_expr_result.Extract()));
//
//      } else {
//        auto value_expr_result = ParsePrimaryStatement(c);
//        if (not value_expr_result) {
//          return value_expr_result;
//        }
//        c.Advance(value_expr_result);
//        return Success(c, Cst(eAst::TypeAlias, "", typedef_identifier, value_expr_result.Extract()));
//      }
//    }
//    // - lib-> Library Namespace Inclusion.
//    else if (c.TypeIs(KwLib)) {
//      c.Advance();
//      auto value_expr_result = ParsePrimaryStatement(c);
//      if (not value_expr_result) {
//        return value_expr_result;
//      }
//      c.Advance(value_expr_result.Always().Iter());
//      return Success(c, Cst(eAst::LibraryNamespaceInclusion, "", value_expr_result.Extract()));
//
//    }
//    // - namespace-> Namespace Inclusion.
//    else if (c.TypeIs(KwNamespace)) {
//      c.Advance();
//      auto value_expr_result = ParsePrimaryStatement(c);
//      if (not value_expr_result) {
//        return value_expr_result;
//      }
//      c.Advance(value_expr_result);
//
//      return Success(c, Cst(eAst::NamespaceInclusion, "", value_expr_result.Extract()));
//    } else if (c.IsPrimary()) {
//      auto primary_result = ParsePrimaryStatement(c);
//      if (not primary_result) {
//        return primary_result;
//      }
//      c.Advance(primary_result);
//      return Success(c, Cst(eAst::NamespaceObjectInclusion, "", primary_result.Extract()));
//
//    } else {
//      return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Colon, c,
//                                                                  "Expected commercial at or lib or namespace "
//                                                                  "or type expression."));
//    }
//
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//};
//
// OffsetParseResult parser::ParseVariableDecl(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//  // Format:
//  // <modifiers?><def><what*?><commercial_at?><name?><colon?><definition?>
//  // <semicolon>
//  Cst mod_node;  // Check if there are any modifiers.
//  if (c.IsModifierKeyword()) {
//    auto mod_result = ParseModifiers(c);
//    if (not mod_result) {
//      return mod_result;
//    }
//    mod_node = mod_result.Extract();
//    c.Advance(mod_result);
//  } else {
//    // No modifiers found.
//    mod_node = Cst(eAst::Modifiers);
//  }
//
//  // Expect a def keyword
//  if (c.TypeIs(KwDef)) {
//    c.Advance();
//    // 2. Expect a what token(s).
//    // Everything following def until kCommericalAt is a type constraint
//    // describing what is being created.
//    auto what_result = ParsePrimaryPreIdentifier(c);
//    if (not what_result) {
//      return what_result;
//    }
//    Cst what_node = what_result.Extract();
//    c.Advance(what_result);
//
//    // Expecting an identifier.
//    if (c.TypeIsnt(Ident)) {
//      return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Ident, c));
//    }
//    Cst ident_node = Cst(c.Get());
//    c.Advance();
//
//    // If there is a colon, this is a Definition.
//    // If there is a semicolon, this is a Declaration.
//    if (c.TypeIs(Colon)) {
//      c.Advance();
//      auto def_result = ParsePrimaryStatement(c);
//      if (not def_result) {
//        return def_result;
//      }
//      Cst def_node = Cst(eAst::VariableDefinition, "", def_result.Extract());
//      c.Advance(def_result);
//      return Success(c, Cst(eAst::VariableDeclaration, "", mod_node, what_node, ident_node, def_node));
//    } else if (c.TypeIs(Semicolon)) {
//      c.Advance();
//      return Success(c, Cst(eAst::VariableDeclaration, "", mod_node, what_node, ident_node));
//    } else {
//      return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Colon, c, "Expected colon or semicolon."));
//    }
//
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//}
//
// OffsetParseResult parser::ParseMethodDecl(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//  // Format:
//  // <modifiers?><fn><commercial_at?><name?><signature?><colon?>
//  // <definition?> <semicolon>
//  Cst mod_node;  // Check if there are any modifiers.
//  if (c.IsModifierKeyword()) {
//    auto mod_result = ParseModifiers(c);
//    if (not mod_result) {
//      return mod_result;
//    }
//    mod_node = mod_result.Extract();
//    c.Advance(mod_result);
//  } else {
//    // No modifiers found.
//    mod_node = Cst(eAst::Modifiers);
//  }
//
//  // Expect a fn keyword
//  if (c.TypeIs(KwFn)) {
//    c.Advance();
//    // Expecting a commercial @
//    if (c.TypeIsnt(CommercialAt)) {
//      return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(CommercialAt, c));
//    }
//    c.Advance();
//
//    // Expecting an identifier.
//    if (c.TypeIsnt(Ident)) {
//      return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Ident, c));
//    }
//    Cst ident_node = Cst(c.Get());
//    c.Advance();
//
//    Cst sig_node;
//    // Expecting a signature or colon/semicolon.
//    if (c.TypeIsnt(Colon) && c.TypeIsnt(Semicolon)) {
//      auto sig_result = ParseMethodSignature(c);
//      if (not sig_result) {
//        return sig_result;
//      }
//      sig_node = sig_result.Extract();
//      c.Advance(sig_result);
//      if (c.Peek(-1).TypeIs(Colon)) c.Advance(-1);  // !!SPECIAL CASE:
//      // Signature will consume and advance past the colon or semicolon.
//      // This is to allow checking if the signature is a decl or
//      // definition within the signature parsing method.
//      // So after getting signature, advance backwards 1 token
//      // but only if that token is a colon!!
//    } else {
//      sig_node = Cst(eAst::MethodSignature);
//    }
//
//    // If there is a colon, this is a Definition.
//    // If there is a semicolon, this is a Declaration.
//    if (c.TypeIs(Colon)) {
//      c.Advance();
//      auto def_result = ParseMethodDef(c);
//      if (not def_result) {
//        return def_result;
//      }
//      Cst def_node = def_result.Extract();
//      c.Advance(def_result);
//      // Expect semicolon
//      if (c.TypeIsnt(Semicolon)) {
//        return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Semicolon, c));
//      }
//      c.Advance();
//      return Success(c, Cst(eAst::MethodDeclaration, "", mod_node, ident_node, sig_node, def_node));
//
//    } else if (c.TypeIs(Semicolon)) {
//      c.Advance();
//      return Success(c, Cst(eAst::MethodDeclaration, "", mod_node, ident_node, sig_node));
//
//    } else {
//      return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Colon, c, "Expected colon or semicolon."));
//    }
//
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//}
//
// OffsetParseResult parser::ParseClassDecl(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//  // Format:
//  // <modifiers?><class><commercial_at?><name?><colon?>
//  // <class_definition?> <semicolon>
//  Cst mod_node;
//  if (c.IsModifierKeyword()) {
//    auto mod_result = ParseModifiers(c);
//    if (not mod_result) {
//      return mod_result;
//    }
//    mod_node = mod_result.Extract();
//    c.Advance(mod_result);
//  } else {
//    // No modifiers found.
//    mod_node = Cst(eAst::Modifiers);
//  }
//
//  if (c.TypeIs(KwClass)) {
//    c.Advance();
//
//    if (c.TypeIsnt(CommercialAt)) {
//      return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(CommercialAt, c));
//    }
//    c.Advance();
//
//    if (c.TypeIsnt(Ident)) {
//      return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Ident, c));
//    }
//    Cst ident_node = Cst(c.Get());
//    c.Advance();
//
//    // If there is a colon, this is a Definition.
//    // If there is a semicolon, this is a Declaration.
//    if (c.TypeIs(Colon)) {
//      c.Advance();
//      auto def_result = ParseClassDef(c);
//      if (not def_result) {
//        return def_result;
//      }
//      Cst def_node = def_result.Extract();
//      c.Advance(def_result);
//      return Success(c, Cst(eAst::ClassDeclaration, "", mod_node, ident_node, def_node));
//    } else if (c.TypeIs(Semicolon)) {
//      c.Advance();
//      return Success(c, Cst(eAst::ClassDeclaration, "", mod_node, ident_node));
//    } else {
//      return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Colon, c, "Expected colon or semicolon."));
//    }
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//}
//
// OffsetParseResult parser::ParseImportDecl(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//  // Format: <import><name><semicolon>
//  if (c.TypeIs(KwImport)) {
//    c.Advance();
//    // Expecting an identifier.
//    if (not c.TypeIs(Ident)) {
//      return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Ident, c));
//    }
//    Cst ident_node = Cst(c.Get());
//    c.Advance();
//
//    // Expecting a semicolon.
//    if (c.TypeIs(Semicolon)) {
//      c.Advance();
//      return Success(c, Cst(eAst::ImportDeclaration, "", ident_node));
//    } else {
//      return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Semicolon, c));
//    }
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//}
//
// OffsetParseResult parser::ParseLibDecl(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//  // Format:
//  // <modifiers?><lib><commercial_at?><name?><colon?>
//  // <lib_definition?> <semicolon>
//  Cst mod_node;  // Check if there are any modifiers.
//  if (c.IsModifierKeyword()) {
//    auto mod_result = ParseModifiers(c);
//    if (not mod_result) {
//      return mod_result;
//    }
//    mod_node = mod_result.Extract();
//    c.Advance(mod_result);
//  } else {
//    // No modifiers found.
//    mod_node = Cst(eAst::Modifiers);
//  }
//
//  // Expect a lib keyword
//  if (c.TypeIs(KwLib)) {
//    c.Advance();
//    // If there is a colon, this is an unnamed library.
//    // Expect a definition ending in a semicolon.
//    if (c.TypeIs(Colon)) {
//      c.Advance();
//      auto def_result = ParseLibDef(c);
//      if (not def_result) {
//        return def_result;
//      }
//      auto def_node = def_result.Extract();
//      c.Advance(def_result);
//      return Success(c, Cst(eAst::LibraryDeclaration, "", mod_node, def_node));
//    }
//
//    // If there is a @ following the lib keyword, this is a named library.
//    // Expecting a commercial @ followed by an identifier.
//    if (c.TypeIsnt(CommercialAt)) {
//      return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(CommercialAt, c));
//    }
//    c.Advance();
//
//    // Expecting an identifier.
//    if (c.TypeIsnt(Ident)) {
//      return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Ident, c));
//    }
//    Cst ident_node{c.Get()};
//    c.Advance();
//
//    if (c.TypeIs(Semicolon)) {
//      c.Advance();
//      return Success(c, Cst(eAst::LibraryDeclaration, "", mod_node, ident_node));
//    }
//
//    // Expecting a colon and a definition.
//    if (c.TypeIsnt(Colon)) {
//      return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Colon, c));
//    }
//    c.Advance();
//
//    auto def_result = ParseLibDef(c);
//    if (not def_result) {
//      return def_result;
//    }
//    Cst def_node = def_result.Extract();
//    c.Advance(def_result);
//
//    return Success(c, Cst(eAst::LibraryDeclaration, "", mod_node, ident_node, def_node));
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//}
//
// OffsetParseResult parser::ParseMainDecl(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//  // Format:
//  // <main><commerical_at?><name?><function_signature><colon?><definition?>
//  // <semicolon>
//
//  // Expect a main keyword
//  if (c.TypeIs(KwMain)) {
//    c.Advance();
//
//    // If there is a commercial at following the main keyword, this is a named
//    // main. Expecting a commercial @ followed by an identifier.
//    if (c.TypeIs(CommercialAt)) {
//      return Fail(c, CaErr::ErrDetail<CaErr::NotImplemented>(c, CPP_LOC, "Named main not implemented."));
//    } else {
//      // This is an unnamed main.
//      // Expecting a function signature followed by a colon and a definition.
//      // Expecting a colon.
//      Cst sig_node;
//      if (c.TypeIs(Colon)) {
//        sig_node = Cst(eAst::MethodSignature);
//        c.Advance();
//      } else {
//        auto sig_result = ParseMethodSignature(c);
//        if (not sig_result) {
//          return sig_result;
//        }
//        sig_node = sig_result.Extract();
//        c.Advance(sig_result);
//      }
//
//      // Expecting a definition.
//      auto def_result = ParseMainDef(c);
//      if (not def_result) {
//        return def_result;
//      }
//      Cst def_node = def_result.Extract();
//      c.Advance(def_result);
//
//      // Expect semicolon
//      if (c.TypeIsnt(Semicolon)) {
//        return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Semicolon, c));
//      }
//      c.Advance();
//
//      return Success(c, Cst(eAst::MainDeclaration, "", sig_node, def_node));
//    }
//
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//}
//
// OffsetParseResult parser::ParseMethodParameters(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//
//  static constexpr LAMBDA xFindCommercialAt = [](TkVectorConstIter beg, TkVectorConstIter end) -> bool {
//    TkCursor c(beg, end);
//    while (c.TypeIsnt(CommercialAt)) {
//      if (not c.AtEnd())
//        c.Advance();
//      else
//        return false;
//    }
//    return true;
//  };
//
//  if (c.TypeIsnt(LParen)) {
//    return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(LParen, c));
//  }
//
//  // Check for empty arguments.
//  if (c.Next().TypeIs(RParen)) {
//    return Success(c.Advance(2),
//                   Cst(eAst::MethodParameterList, "", Cst(eAst::MethodParameter, "", Cst(eAst::MethodVoid))));
//  }
//
//  TkScope::Vector arg_scopes = TkScope::FindSeperatedParen(c, Comma);
//  if (not arg_scopes.front()) {
//    return Fail(c, CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//  }
//
//  // For each arg:
//  // - If there is no @ then it is an identifier of type any -> Expecting an
//  // identifier.
//  // - If there is an @ then it is a typed argument -> Expect possible mods,
//  // followed a primaryPreIdentifier.
//  Cst method_parameters_list_node(eAst::MethodParameterList);
//  for (const auto& scope : arg_scopes) {
//    Cst method_parameter_node(eAst::MethodParameter);
//    c.Advance(scope.ContainedBegin());
//    if (xFindCommercialAt(scope.ContainedBegin(), scope.ContainedEnd())) {
//      // Modifiers
//      if (c.IsModifierKeyword()) {
//        auto method_mods_result = ParseModifiers(c);
//        if (not method_mods_result) {
//          return method_mods_result;
//        }
//        c.Advance(method_mods_result);
//        method_parameter_node.ExtractAndPush(method_mods_result);
//      } else {
//        method_parameter_node.PushBack(Cst(eAst::Modifiers, "", Cst(eAst::KwNone)));
//      }
//
//      // Type
//      if (c.IsPrimary()) {
//        auto method_parameter_type_result = ParsePrimaryPreIdentifier(c);
//        if (not method_parameter_type_result) {
//          return method_parameter_type_result.ChainFailure("Error parsing method parameter type.");
//        }
//        c.Advance(method_parameter_type_result.Always().Iter());
//        method_parameter_node.ExtractAndPush(method_parameter_type_result);
//      } else if (c.TypeIs(CommercialAt)) {
//        c.Advance();
//        // Implied any type.
//        method_parameter_node.PushBack(eAst::KwAny);
//      } else {
//        return Fail(c, CaErr::ErrDetail<CaErr::ExpectedPrimaryExpression>(c, CPP_LOC));
//      }
//
//      // Identity
//      if (c.TypeIs(Ident)) {
//        method_parameter_node.PushBack(c.Get());
//      } else {
//        return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Ident, c));
//      }
//
//      // Add to list
//      method_parameters_list_node.PushBack(move(method_parameter_node));
//    } else {
//      if (scope.ContainedBegin()->TypeIs(Ident)) {
//        method_parameters_list_node.PushBack(Cst(eAst::MethodParameter, "", Cst(eAst::Modifiers, "", Cst(eAst::NONE)),
//                                                 Cst(eAst::KwAny), Cst(*scope.ContainedBegin())));
//      } else {
//        c.Advance(scope.ContainedBegin());
//        return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Ident, c));
//      }
//    }
//  }
//  return Success(c.Advance(arg_scopes.back().End()), move(method_parameters_list_node));
//}
//
// OffsetParseResult parser::ParseMethodReturnParameters(TkCursor c) {
//  using namespace caerr;
//  Cst method_return_type_node{eAst::MethodReturnType};
//  // Expect any number of modifiers.
//  if (c.IsModifierKeyword()) {
//    auto method_mods_result = ParseModifiers(c);
//    if (not method_mods_result) {
//      return method_mods_result;
//    }
//    c.Advance(method_mods_result);
//    method_return_type_node.ExtractAndPush(method_mods_result);
//  }
//
//  // Expecting a primary expression ending in a colon.
//  if (c.IsPrimary()) {
//    auto ret_type_result = ParsePrimaryPostIdentifier(c);
//    if (not ret_type_result) {
//      return ret_type_result;
//    }
//    c.Advance(ret_type_result);
//    method_return_type_node.ExtractAndPush(ret_type_result);
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ExpectedPrimaryExpression>(c, CPP_LOC));
//  }
//
//  return Success(c, move(method_return_type_node));
//}
//
// OffsetParseResult parser::ParseMethodSignature(TkCursor c) {
//  using namespace caerr;
//  using enum eAst;
//  static const LAMBDA xMake1ParamSigAst = [](Cst ret, Cst param) {
//    return Cst(MethodSignature, "", Cst(MethodParameterList, "", Cst(MethodParameter, "", param)),
//               Cst(MethodReturnType, "", ret));
//  };
//
//  static const LAMBDA xMakeSigAst = [](Cst ret_type_ast, Cst param_list_ast) {
//    return Cst(MethodSignature, "", param_list_ast, ret_type_ast);
//  };
//
//  ///////////////////////////////////////////////////////////////////////////////
//  // Colon after identifier -> Implicit void arg, no return.
//  ///////////////////////////////////////////////////////////////////////////////
//  if (c.TypeIs(eTk::Colon)) {
//    return Success(c, xMake1ParamSigAst(MethodVoid, MethodVoid));
//  }
//
//  ///////////////////////////////////////////////////////////////////////////////
//  // GreaterThan after identifier -> Some sort of void arg with a return.
//  ///////////////////////////////////////////////////////////////////////////////
//  else if (c.TypeIs(eTk::Gt)) {
//    c.Advance();
//    if (c.TypeIs(eTk::Colon)) {  // Implicit any return void method.
//      c.Advance();
//      return Success(c, xMake1ParamSigAst(KwAny, MethodVoid));
//    }
//
//    if (c.TypeIs(eTk::Semicolon)) {
//      return Success(c, xMake1ParamSigAst(KwAny, MethodVoid));
//    }
//
//    // Parse the return type.
//    auto return_type_result = ParseMethodReturnParameters(c);
//    if (not return_type_result) {
//      return return_type_result;
//    }
//
//    // Return type must end in a colon. If it doesn't, error above.
//    c.Advance(return_type_result);
//    return Success(c, xMakeSigAst(return_type_result.Extract(),
//                                  Cst{MethodParameterList, "", Cst{MethodParameter, "", Cst{MethodVoid}}}));
//  }
//
//  ///////////////////////////////////////////////////////////////////////////////
//  // Open Paren After Identifier -> Method with arguments.
//  ///////////////////////////////////////////////////////////////////////////////
//  else if (c.TypeIs(eTk::LParen)) {
//    auto method_params_result = ParseMethodParameters(c);
//    if (not method_params_result) {
//      return method_params_result;
//    }
//    c.Advance(method_params_result);
//    if (c.TypeIs(eTk::Semicolon)) {
//      return Success(c, xMakeSigAst(Cst(MethodReturnType, "", Cst(MethodVoid)), method_params_result.Extract()));
//    }
//
//    // Expecting a colon or a greater than.
//    if (c.TypeIs(eTk::Colon)) {
//      c.Advance();
//      return Success(c, xMakeSigAst(Cst(MethodReturnType, "", Cst(MethodVoid)), method_params_result.Extract()));
//    } else if (c.TypeIs(eTk::Gt)) {
//      c.Advance();
//      // if the next token is a colon, then the return type is any.
//      if (c.TypeIs(eTk::Colon)) {
//        c.Advance();
//        return Success(c, xMakeSigAst(Cst(MethodReturnType, "", Cst(KwAny)), method_params_result.Extract()));
//      }
//
//      if (c.TypeIs(eTk::Semicolon)) {
//        return Success(c, xMakeSigAst(Cst(MethodReturnType, "", Cst(KwAny)), method_params_result.Extract()));
//      }
//
//      auto return_type_result = ParseMethodReturnParameters(c);
//      if (not return_type_result) {
//        return return_type_result;
//      }
//      c.Advance(return_type_result);
//      return Success(c, xMakeSigAst(return_type_result.Extract(), method_params_result.Extract()));
//    } else {
//      return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(eTk::Colon, c));
//    }
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//}
//
// OffsetParseResult parser::ParseMethodDef(TkCursor c) {
//  using namespace caerr;
//
//  Cst node{eAst::MethodDefinition};
//  auto statement_scope = TkScope::FindBrace(c);
//  if (not statement_scope) {
//    return Fail(c, CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//  }
//  c.Advance();
//
//  while (c.Iter() != statement_scope.ContainedEnd()) {
//    if (c.IsPragmatic()) {
//      auto decl_result = ParseFunctionalStmt(c);
//      if (not decl_result) {
//        return decl_result;
//      }
//      node.ExtractAndPush(decl_result);
//      c.Advance(decl_result);
//    } else if (c.IsPrimary()) {
//      auto primary_result = ParsePrimaryStatement(c);
//      if (not primary_result) {
//        return primary_result;
//      }
//      node.ExtractAndPush(primary_result);
//      c.Advance(primary_result);
//    } else {
//      return Fail(c, CaErr::ErrDetail<CaErr::ExpectedPragmaticDeclaration>(c, CPP_LOC));
//    }
//  }
//  c.Advance();
//
//  return Success(c, move(node));
//}
//
// OffsetParseResult parser::ParseMainDef(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//
//  Cst node{eAst::MainDefinition};
//  auto statement_scope = TkScope::FindBrace(c);
//  if (not statement_scope) {
//    return Fail(c, CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//  }
//  c.Advance();
//
//  while (c.Iter() != statement_scope.ContainedEnd()) {
//    if (c.IsPragmatic()) {
//      auto decl_result = ParseFunctionalStmt(c);
//      if (not decl_result) {
//        return decl_result;
//      }
//      node.ExtractAndPush(decl_result);
//      c.Advance(decl_result);
//    } else if (c.IsPrimary()) {
//      auto primary_result = ParsePrimaryStatement(c);
//      if (not primary_result) {
//        return primary_result;
//      }
//      node.ExtractAndPush(primary_result);
//      c.Advance(primary_result);
//    } else {
//      return Fail(c, CaErr::ErrDetail<CaErr::ExpectedPragmaticDeclaration>(c, CPP_LOC));
//    }
//  }
//  c.Advance();
//
//  return Success(c, move(node));
//}
//
// OffsetParseResult parser::ParseClassDef(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//
//  Cst node{eAst::ClassDefinition};
//  auto statement_scope = TkScope::FindBrace(c);
//  if (not statement_scope) {
//    return Fail(c, CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//  }
//  c.Advance();
//
//  while (c.Iter() != statement_scope.ContainedEnd()) {
//    if (c.IsPragmatic()) {
//      auto decl_result = ParsePragmaticStmt(c);
//      if (not decl_result) {
//        return decl_result;
//      }
//      node.ExtractAndPush(decl_result);
//      c.Advance(decl_result);
//    } else {
//      return Fail(c, CaErr::ErrDetail<CaErr::ExpectedPragmaticDeclaration>(c, CPP_LOC));
//    }
//  }
//  c.Advance();
//
//  if (c.TypeIs(Semicolon)) {
//    c.Advance();
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Semicolon, c));
//  }
//  return Success(c, move(node));
//}
//
// OffsetParseResult parser::ParseLibDef(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//
//  Cst node{eAst::LibraryDefinition};
//  auto statement_scope = TkScope::FindBrace(c);
//  if (not statement_scope) {
//    return Fail(c, CaErr::ErrDetail<CaErr::MismatchedScope>(c, CPP_LOC));
//  }
//  c.Advance();
//
//  while (c.Iter() != statement_scope.ContainedEnd()) {
//    if (c.IsPragmatic()) {
//      auto decl_result = ParsePragmaticStmt(c);
//      if (not decl_result) {
//        return decl_result;
//      }
//      node.ExtractAndPush(decl_result);
//      c.Advance(decl_result);
//    } else {
//      return Fail(c, CaErr::ErrDetail<CaErr::ExpectedPragmaticDeclaration>(c, CPP_LOC));
//    }
//  }
//  c.Advance();
//
//  if (c.TypeIs(Semicolon)) {
//    c.Advance();
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Semicolon, c));
//  }
//  return Success(c, move(node));
//}
//
// OffsetParseResult parser::ParseProgram(TkCursor c) {
//  using namespace caerr;
//  Cst program_node{eAst::Program};
//  while (not c.AtEnd()) {
//    if (c.IsPragmatic()) {
//      OffsetParseResult decl_result = ParsePragmaticStmt(c);
//      if (not decl_result) {
//        return decl_result;
//      }
//      program_node.ExtractAndPush(decl_result);
//      c.Advance(decl_result);
//    } else {
//      return Fail(c, CaErr::ErrDetail<CaErr::ExpectedPragmaticDeclaration>(c, CPP_LOC));
//    }
//  }
//  return Success(c, move(program_node));
//}
//
// OffsetParseResult parser::ParseEnumDecl(TkCursor c) {
//  using namespace caerr;
//  using enum eTk;
//  // Format:
//  // <modifiers?> <enum> <commercial_at?><name?><colon?>
//  // <<association_typename><<commercial_at><association_identifier>?><semicolon>?>
//  // <<commercial_at><enum_entry_name?><semicolon>
//  Cst mod_node;
//  if (c.IsModifierKeyword()) {
//    auto mod_result = ParseModifiers(c);
//    if (not mod_result) {
//      return mod_result;
//    }
//    mod_node = mod_result.Extract();
//    c.Advance(mod_result);
//  } else {
//    // No modifiers found.
//    mod_node = Cst(eAst::Modifiers);
//  }
//
//  if (c.TypeIs(KwEnum)) {
//    c.Advance();
//    if (c.TypeIsnt(CommercialAt)) {
//      return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(CommercialAt, c));
//    }
//    c.Advance();
//    if (c.TypeIsnt(Ident)) {
//      return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Ident, c));
//    }
//    Cst ident_node = Cst(c.Get());
//    c.Advance();
//
//    // If there is a colon, this is a Definition.
//    // If there is a semicolon, this is a Declaration.
//    if (c.TypeIs(Colon)) {
//      c.Advance();
//      auto def_result = ParseEnumDef(c);
//      if (not def_result) {
//        return def_result;
//      }
//      Cst def_node = def_result.Extract();
//      c.Advance(def_result);
//      return Success(c, Cst(eAst::EnumDeclaration, "", mod_node, ident_node, def_node));
//    } else if (c.TypeIs(Semicolon)) {
//      c.Advance();
//      return Success(c, Cst(eAst::EnumDeclaration, "", mod_node, ident_node));
//    } else {
//      return Fail(c, CaErr::ErrDetail<CaErr::ParserExpectedToken>(Colon, c, "Expected colon or semicolon."));
//    }
//  } else {
//    return Fail(c, CaErr::ErrDetail<CaErr::ImplExpectedToken>(c, CPP_LOC));
//  }
//};
//
// OffsetParseResult parser::ParseEnumDef(TkCursor c) {
//  // If followed by an open brace, positional_enum.
//  // If followed by anything else,associative_enum.
//  // associative_enum -> <type_expr><commercial_at><enum_entry_name?><colon>
//  // After each named association, may be an open brace or another association.
//  using namespace caerr;
//  using enum eTk;
//  Cst node{eAst::EnumDefinition};
//  // Parse enum header.
//  while (c.TypeIsnt(LBrace)) {
//    Cst association_node{eAst::EnumAssociation};
//    auto type_expr_result = ParsePrimaryPreIdentifier(c);
//    if (!type_expr_result) {
//      return type_expr_result;
//    }
//    c.Advance(type_expr_result);
//    association_node.ExtractAndPush(type_expr_result);
//    if (c.TypeIsnt(Ident)) return Fail(c, COMPILER_ERROR(ParserExpectedToken, Ident, c));
//    association_node.PushBack(c.Get());
//    c.Advance();
//    if (c.TypeIs(Colon)) return Fail(c, COMPILER_ERROR(ParserExpectedToken, Colon, c));
//    c.Advance();
//  }
//
//  // Parse enum definition block.
//  auto block_result = ParseEnumBlock(c);
//  if (!block_result) return block_result;
//  node.ExtractAndPush(block_result);
//  c.Advance(block_result);
//
//  return Success(c, move(node));
//};
//
// OffsetParseResult parser::ParseEnumBlock(TkCursor c) {
//  // Parse inside an enum definition block.
//  // Cursor should begin at the open brace of the block.
//  using namespace caerr;
//  using enum eTk;
//  static constexpr LAMBDA xParseAssociationInitializers = [](TkCursor& crsr, caoco::Cst& entry) {
//    while (crsr.TypeIsnt(Semicolon)) {
//      if (crsr.TypeIsnt(Colon)) return Fail(crsr, COMPILER_ERROR(ParserExpectedToken, Ident, crsr));
//      crsr.Advance();
//      auto next_assoc_res = ParsePrimaryPostIdentifier(crsr);
//      if (!next_assoc_res) return next_assoc_res;
//      crsr.Advance(next_assoc_res);
//      entry.ExtractAndPush(next_assoc_res);
//    }
//    crsr.Advance();
//  };
//
//  Cst node{eAst::EnumBlock};
//  if (c.TypeIsnt(LBrace))  // Require opening brace.
//    return Fail(c, COMPILER_ERROR(ParserExpectedToken, LBrace, c));
//  c.Advance();
//  while (c.TypeIsnt(RBrace)) {
//    // EnumEntry
//    if (c.TypeIs(CommercialAt)) {
//      c.Advance();
//      if (c.TypeIsnt(Ident)) return Fail(c, COMPILER_ERROR(ParserExpectedToken, Ident, c));
//      node.PushBack({eAst::EnumEntry, c.Literal()});
//      auto& this_entry = node.Back();
//      c.Advance();
//      // Check for association initializers. Expect a colon or semicolon.
//      xParseAssociationInitializers(c, this_entry);
//    }
//    // EnumCategory
//    else if (c.TypeIs(KwUse)) {
//      c.Advance();
//      TkVector this_category{};
//      // Consume identifiers until a semicolon is reached.
//      while (c.TypeIsnt(Colon)) {
//        if (c.TypeIsnt(CommercialAt)) return Fail(c, COMPILER_ERROR(ParserExpectedToken, CommercialAt, c));
//        c.Advance();
//        if (c.TypeIsnt(Ident)) return Fail(c, COMPILER_ERROR(ParserExpectedToken, Ident, c));
//        this_category.push_back(c.Get());
//        c.Advance();
//      }
//      c.Advance();  // pass colon
//
//      // expect the enum entry for a singular entry or a block for multi entry.
//      if (c.TypeIs(CommercialAt)) {
//        c.Advance();
//        if (c.TypeIsnt(Ident)) return Fail(c, COMPILER_ERROR(ParserExpectedToken, Ident, c));
//        node.PushBack({eAst::EnumEntry, c.Literal()});
//        auto& this_entry = node.Back();
//
//        // Attribute this category to this entry.
//        this_entry.PushBack(eAst::EnumCategory);
//        auto& added_category = this_entry.Back();
//        for (auto& tk : this_category) added_category.PushBack(tk);
//
//        // Check for association initializers. Expect a colon or semicolon.
//        xParseAssociationInitializers(c, this_entry);
//      }
//      // a block
//      else if (c.TypeIs(LBrace)) {
//        auto recursed_block_res = ParseEnumBlock(c);
//        if (!recursed_block_res) return recursed_block_res;
//
//        // Add this category to all categories of the sub-block
//        // as a parent (push to the front), then add the entry
//        // to this block.
//        auto recursed_block = recursed_block_res.Extract();
//        for (auto& entry : recursed_block.ChildrenUnsafe()) {
//          for (auto& entry_data : entry.ChildrenUnsafe()) {
//            if (entry_data.TypeIs(eAst::EnumCategory))
//              for (auto& category_name_fragment : this_category) entry_data.PushFront(category_name_fragment);
//          }
//          node.PushBack(entry);
//        }
//      } else
//        return Fail(c, COMPILER_ERROR(MismatchedScope, c, CPP_LOC));
//    }
//    // Invalid enum description format.
//    else
//      return Fail(c, COMPILER_ERROR(MismatchedScope, c, CPP_LOC));
//  }
//  c.Advance();
//  ``
//
//      return Success(c, move(node));
//};
//
/////////////////////////////////////////////////////////////////////////////////
//// end Internal parsing methods impl
/////////////////////////////////////////////////////////////////////////////////
//
///// Main parsing method.
// static ExpectedAst ParseTokens(const TkVector& c) {
//   auto parsed = parser::ParseProgram({c.cbegin(), c.cend()});
//   if (parsed) {
//     return ExpectedAst::Success(parsed.Extract());
//   } else {
//     return ExpectedAst::Failure(parsed.Error());
//   }
// }

}  // namespace cnd::parser

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