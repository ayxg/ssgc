///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_compiler_data
/// @brief Closure and ClosureBuffer classes for shift reduction parsing.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @defgroup cand_compiler_data_closure Token Closure Buffer
/// @ingroup cnd_compiler_data
/// Closure and ClosureBuffer classes for shift reduction parsing.
/// Used in the primary expression parsing method.
/// @see cnd::parser::ExprParser
/// @{

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
#include "use_clmsg.hpp"
#include "cldata/tk.hpp"
#include "cldata/ast.hpp"
#include "trtools/token_cursor.hpp"
// clang-format on

//namespace cnd {
/////  A closure is a range of tokens representing a singular ast node.
/////
/////  All tokens from front to back are a single ast node with no children.
/////  When a closure is a single token, the front and back are the same.
/////  Example: 1 + 2 -> Closure #2 is +, •+ is the front and +• is the back.
/////  Example: 1(abcd) + 2 -> Closure #2 is (abcd), •(abcd) is the front
/////  and (abcd)• is the back.
//class Closure {
// public:
//  CND_CX Vec<Span<const Tk>::iterator>::iterator Front() const;
//  CND_CX Vec<Span<const Tk>::iterator>::iterator Back() const;
//  CND_CX eOperation Operation() const;
//  CND_CX ePriority Priority() const;
//  CND_CX eAssoc Assoc() const;
//  CND_CX Closure(Vec<Span<const Tk>::iterator>::iterator front);
//  CND_CX Closure(Vec<Span<const Tk>::iterator>::iterator front, Vec<Tk>::iterator back);
//
// private:
//  Vec<Tk>::iterator front_;
//  Vec<Tk>::iterator back_;
//};
//
///// Container for a list of closures referencing a token stream.
//class ClosureBuffer {
// public:
//  using TkListT = Vec<Tk>;
//  using CosureListT = Vec<Closure>;
//  using ClosureListIterT = CosureListT::iterator;
//
//  // Closure Operations
//  CND_CX void PushFrontClosure(Vec<Span<const Tk>::iterator>::iterator front);
//  CND_CX void PushFrontClosure(Vec<Span<const Tk>::iterator>::iterator front,
//                               Vec<Span<const Tk>::iterator>::iterator back);
//  CND_CX void PushBackClosure(Vec<Span<const Tk>::iterator>::iterator front);
//  CND_CX void PushBackClosure(Vec<Span<const Tk>::iterator>::iterator front,
//                              Vec<Span<const Tk>::iterator>::iterator back);
//  CND_CX void PopClosure();
//  CND_CX void PopClosure(Vec<Closure>::iterator which);
//  CND_CX void PopClosureIf(auto&& condition);
//  CND_CX Size ClosureCount() const;
//  CND_CX Vec<Closure>::iterator LastClosure();
//  CND_CX Vec<Span<const Tk>::iterator>::iterator LastClosureFront();
//  CND_CX Vec<Span<const Tk>::iterator>::iterator LastClosureBack();
//  CND_CX Vec<Closure>::iterator FindClosureReverse(std::function<Bool(Closure&)>&& condition);
//  CND_CX Vec<Vec<Closure>::iterator> FindClosureReverseConsecutive(
//      std::function<Bool(Vec<Closure>::iterator, Vec<Closure>::iterator)>&& condition);
//  CND_CX Vec<Vec<Closure>::iterator> FindClosureReverseConsecutiveAndIgnore(
//      std::function<Bool(Vec<Closure>::iterator, Vec<Closure>::iterator)>&& cond,
//      std::function<Bool(Vec<Closure>::iterator)>&& ignore_cond);
//  CND_CX void StreamInsertBeforeClosure(Vec<Closure>::iterator closure_it, Span<const Tk>::iterator token);
//  CND_CX void StreamInsertAfterClosure(Vec<Closure>::iterator closure_it, Span<const Tk>::iterator token);
//  CND_CX Vec<Span<const Tk>::iterator>::iterator LastStreamed();
//  CND_CX void StreamPushBack(Span<const Tk>::iterator token);
//  CND_CX Vec<Span<const Tk>::iterator> StreamToVector();
//  CND_CX ClosureBuffer();
//
// private:
//  /// Sentinel beginning of closure buffer.
//  /// HAS to be set or else begin is the end before and after first insertion.
//  static constexpr Arr<Tk, 3> kGeneratedTokens{{{eTk::kNONE, "ClosureBuffer sentinel begin."},
//                                              {eTk::kLParen, "("},
//                                              {eTk::kRParen, ")"}}};
//  static constexpr Span<const Tk> kGeneratedTokensView{kGeneratedTokens};
//  static constexpr Span<const Tk>::iterator kSentinelBegin{kGeneratedTokensView.begin()};
//  static constexpr Span<const Tk>::iterator kOpenScope{kGeneratedTokensView.begin()};
//  static constexpr Span<const Tk>::iterator kCloseScope{kGeneratedTokensView.begin()};
//
//  Vec<Span<const Tk>::iterator> token_stream_;
//  Vec<Closure> closures_;
//  Vec<Closure>::iterator sentinel_begin_closure_;
//};
//}  // namespace cnd
//
//namespace cnd {
//// Sentinel beginning of closure buffer.
///// HAS to be set or else begin is the end before and after first insertion.
//// const Tk ClosureBuffer::kSentinelBegin = {eTk::kNONE, "ClosureBuffer sentinel begin."};
//
//CND_CX Vec<Span<const Tk>::iterator>::iterator Closure::Front() const { return front_; }
//
//CND_CX Vec<Span<const Tk>::iterator>::iterator Closure::Back() const { return back_; }
//
///// Operation of the front token. ( [ { are postfix operators.
//CND_CX eOperation Closure::Operation() const {
//  // Special case for (, [, {, and postfix operators.
//  if ((front_)->TypeIs(eTk::kLParen) || (front_)->TypeIs(eTk::kLBrace) || (front_)->TypeIs(eTk::kLBracket)) {
//    return eOperation::Postfix;
//  } else {
//    return (front_)->Operation();
//  }
//}
//
///// Priority of the front token. ( [ { are postfix priority.
//CND_CX ePriority Closure::Priority() const {
//  // Special case for (, [, {, are postfix priority.
//  if ((front_)->TypeIs(eTk::kLParen) || (front_)->TypeIs(eTk::kLBrace) || (front_)->TypeIs(eTk::kLBracket)) {
//    return ePriority::Postfix;
//  } else {
//    return (front_)->Priority();
//  }
//}
//
///// Associativity of the front token.
//CND_CX eAssoc Closure::Assoc() const { return (front_)->Assoc(); }
//
//CND_CX Closure::Closure(Vec<Tk>::iterator front) : front_(front), back_(front) {}
//
//CND_CX Closure::Closure(Vec<Tk>::iterator front, Vec<Tk>::iterator back)
//    : front_(front), back_(back) {}
//
//CND_CX void ClosureBuffer::PushFrontClosure(Vec<Tk>::iterator front) {
//  // closures_.push_front(Closure(front));
//  closures_.insert(closures_.begin(), Closure(front));
//}
//
//CND_CX void ClosureBuffer::PushFrontClosure(Vec<Tk>::iterator front,
//                                            Vec<Tk>::iterator back) {
//  closures_.insert(closures_.begin(), Closure(front, back));
//  // closures_.push_front(Closure(front, back));
//}
//
//CND_CX void ClosureBuffer::PushBackClosure(Vec<Tk>::iterator front) {
//  closures_.push_back(Closure(front));
//}
//
//CND_CX void ClosureBuffer::PushBackClosure(Vec<Tk>::iterator front,
//                                           Vec<Tk>::iterator back) {
//  closures_.push_back(Closure(front, back));
//}
//
//CND_CX void ClosureBuffer::PopClosure() { closures_.pop_back(); }
//
//CND_CX void ClosureBuffer::PopClosure(ClosureListIterT which) { closures_.erase(which); }
//
//CND_CX void ClosureBuffer::PopClosureIf(auto&& condition) {
//  std::remove_if(closures_.begin(), closures_.end(), std::forward<decltype(condition)>(condition));
//  // closures_.remove_if(std::move(condition));
//}
//
//CND_CX Size ClosureBuffer::ClosureCount() const { return closures_.size() - 1; }
//
//CND_CX ClosureBuffer::ClosureListIterT ClosureBuffer::LastClosure() { return std::prev(closures_.end()); }
//
//CND_CX Vec<Span<const Tk>::iterator>::iterator ClosureBuffer::LastClosureFront() { return closures_.back().Front(); }
//
//CND_CX Vec<Span<const Tk>::iterator>::iterator ClosureBuffer::LastClosureBack() { return closures_.back().Back(); }
//
///// Search from the end of the closure list until first occurrence.
//CND_CX ClosureBuffer::ClosureListIterT ClosureBuffer::FindClosureReverse(std::function<bool(Closure&)>&& condition) {
//  return std::find_if(closures_.rbegin(), closures_.rend(), condition).base();
//}
//
//CND_CX Vec<ClosureBuffer::ClosureListIterT> ClosureBuffer::FindClosureReverseConsecutive(
//    std::function<bool(ClosureListIterT, ClosureListIterT)>&& condition) {
//  auto it = closures_.rbegin();
//  auto end = closures_.rend();
//  std::vector<ClosureListIterT> accumulated_iterators;
//  while (it != end) {
//    auto next = std::next(it);
//    if (condition(std::prev(it.base()), std::prev(next.base()))) {
//      accumulated_iterators.push_back(std::prev(it.base()));
//    } else {
//      accumulated_iterators.push_back(std::prev(it.base()));
//      break;
//    }
//    ++it;
//  }
//  return accumulated_iterators;
//}
//
//CND_CX Vec<ClosureBuffer::ClosureListIterT> ClosureBuffer::FindClosureReverseConsecutiveAndIgnore(
//    std::function<bool(ClosureListIterT, ClosureListIterT)>&& condition,
//    std::function<bool(ClosureListIterT)>&& ignore_condition) {
//  auto it = closures_.rbegin();
//  auto end = closures_.rend();
//  std::vector<ClosureListIterT> accumulated_iterators;
//  while (it != end) {
//    auto next = std::next(it);
//    if (ignore_condition(next.base())) {
//      it++;
//      continue;
//    } else {
//      if (condition(std::prev(it.base()), std::prev(next.base()))) {
//        accumulated_iterators.push_back(std::prev(it.base()));
//        it++;
//      } else {
//        accumulated_iterators.push_back(std::prev(it.base()));
//        break;
//      }
//    }
//  }
//  return accumulated_iterators;
//}
//
//CND_CX void ClosureBuffer::StreamInsertBeforeClosure(ClosureListIterT closure_it, Span<const Tk>::iterator token) {
//  if (closure_it == sentinel_begin_closure_) {
//    token_stream_.insert(std::next(token_stream_.begin()), token);
//    return;
//  }
//  auto front = closure_it->Front();
//  token_stream_.insert(front, token);
//}
//
//CND_CX void ClosureBuffer::StreamInsertAfterClosure(ClosureListIterT closure_it, Span<const Tk>::iterator token) {
//  auto back = closure_it->Back();
//  auto next = std::next(back);
//  if (next == token_stream_.end()) {
//    token_stream_.push_back(token);
//  } else {
//    token_stream_.insert(next, token);
//  }
//}
//
///// Get the last pushed token in the token stream.
//CND_CX Vec<Span<const Tk>::iterator>::iterator ClosureBuffer::LastStreamed() { return std::prev(token_stream_.end()); }
//
//CND_CX void ClosureBuffer::StreamPushBack(Span<const Tk>::iterator token) { token_stream_.push_back(token); }
//
//CND_CX Vec<Span<const Tk>::iterator> ClosureBuffer::StreamToVector() {
//  Vec<Span<const Tk>::iterator> vec;
//  // Dont append the sentinel begin token.
//  vec.reserve(token_stream_.size() - 1);
//  for (auto& tk : std::ranges::subrange(token_stream_.begin() + 1, token_stream_.end())) {
//    vec.push_back(tk);
//  }
//  return vec;
//}
//
//constexpr ClosureBuffer::ClosureBuffer() {
//  // Create sentinel begin closure.
//  // HAS to be set or else begin is the end before and after first insertion.
//  token_stream_.push_back(kSentinelBegin);
//  closures_.push_back(Closure(token_stream_.begin()));
//  sentinel_begin_closure_ = closures_.begin();
//}
//
//}  // namespace cnd

/// @} // end of cand_compiler_parser_closure

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
