///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_compiler_data
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cand_compiler_data_closure
/// @{
#include "cnd_tk_closure.hpp"
// clang-format off
#include "cnd_implicit.hpp"
#include "cnd_constdef.hpp"
#include "cnd_clerr.hpp"
#include "cnd_tk.hpp"
// clang-format on

namespace cnd {
// Sentinel beginning of closure buffer.
/// HAS to be set or else begin is the end before and after first insertion.
const Tk ClosureBuffer::kSentinelBegin = {eTk::kNONE, "ClosureBuffer sentinel begin."};

Closure::TkListIterT Closure::Front() const { return front_; }

Closure::TkListIterT Closure::Back() const { return back_; }

/// Operation of the front token. ( [ { are postfix operators.
eOperation Closure::Operation() const {
  // Special case for (, [, {, and postfix operators.
  if (front_->TypeIs(eTk::kLParen) || front_->TypeIs(eTk::kLBrace) || front_->TypeIs(eTk::kLBracket)) {
    return eOperation::Postfix;
  } else {
    return front_->Operation();
  }
}

/// Priority of the front token. ( [ { are postfix priority.
ePriority Closure::Priority() const {
  // Special case for (, [, {, are postfix priority.
  if (front_->TypeIs(eTk::kLParen) || front_->TypeIs(eTk::kLBrace) || front_->TypeIs(eTk::kLBracket)) {
    return ePriority::Postfix;
  } else {
    return front_->Priority();
  }
}

/// Associativity of the front token.
eAssoc Closure::Assoc() const { return front_->Assoc(); }

Closure::Closure(TkListIterT front) : front_(front), back_(front) {}

Closure::Closure(TkListIterT front, TkListIterT back) : front_(front), back_(back) {}

void ClosureBuffer::PushFrontClosure(TkListIterT front) { closures_.push_front(Closure(front)); }

void ClosureBuffer::PushFrontClosure(TkListIterT front, TkListIterT back) {
  closures_.push_front(Closure(front, back));
}

void ClosureBuffer::PushBackClosure(TkListIterT front) { closures_.push_back(Closure(front)); }

void ClosureBuffer::PushBackClosure(TkListIterT front, TkListIterT back) { closures_.push_back(Closure(front, back)); }

void ClosureBuffer::PopClosure() { closures_.pop_back(); }

void ClosureBuffer::PopClosure(ClosureListIterT which) { closures_.erase(which); }

void ClosureBuffer::PopClosureIf(auto&& condition) { closures_.remove_if(std::move(condition)); }

Size ClosureBuffer::ClosureCount() const { return closures_.size() - 1; }

ClosureBuffer::ClosureListIterT ClosureBuffer::LastClosure() { return std::prev(closures_.end()); }

ClosureBuffer::TkListIterT ClosureBuffer::LastClosureFront() { return closures_.back().Front(); }

ClosureBuffer::TkListIterT ClosureBuffer::LastClosureBack() { return closures_.back().Back(); }

/// Search from the end of the closure list until first occurrence.
ClosureBuffer::ClosureListIterT ClosureBuffer::FindClosureReverse(std::function<bool(Closure&)>&& condition) {
  return std::find_if(closures_.rbegin(), closures_.rend(), condition).base();
}

Vec<ClosureBuffer::ClosureListIterT> ClosureBuffer::FindClosureReverseConsecutive(
    std::function<bool(ClosureListIterT, ClosureListIterT)>&& condition) {
  auto it = closures_.rbegin();
  auto end = closures_.rend();
  std::vector<ClosureListIterT> accumulated_iterators;
  while (it != end) {
    auto next = std::next(it);
    if (condition(std::prev(it.base()), std::prev(next.base()))) {
      accumulated_iterators.push_back(std::prev(it.base()));
    } else {
      accumulated_iterators.push_back(std::prev(it.base()));
      break;
    }
    ++it;
  }
  return accumulated_iterators;
}

Vec<ClosureBuffer::ClosureListIterT> ClosureBuffer::FindClosureReverseConsecutiveAndIgnore(
    std::function<bool(ClosureListIterT, ClosureListIterT)>&& condition,
    std::function<bool(ClosureListIterT)>&& ignore_condition) {
  auto it = closures_.rbegin();
  auto end = closures_.rend();
  std::vector<ClosureListIterT> accumulated_iterators;
  while (it != end) {
    auto next = std::next(it);
    if (ignore_condition(next.base())) {
      it++;
      continue;
    } else {
      if (condition(std::prev(it.base()), std::prev(next.base()))) {
        accumulated_iterators.push_back(std::prev(it.base()));
        it++;
      } else {
        accumulated_iterators.push_back(std::prev(it.base()));
        break;
      }
    }
  }
  return accumulated_iterators;
}

void ClosureBuffer::StreamInsertBeforeClosure(ClosureListIterT closure_it, Tk token) {
  if (closure_it == sentinel_begin_closure_) {
    token_stream_.insert(std::next(token_stream_.begin()), token);
    return;
  }
  auto front = closure_it->Front();
  token_stream_.insert(front, token);
}

void ClosureBuffer::StreamInsertAfterClosure(ClosureListIterT closure_it, Tk token) {
  auto back = closure_it->Back();
  auto next = std::next(back);
  if (next == token_stream_.end()) {
    token_stream_.push_back(token);
  } else {
    token_stream_.insert(next, token);
  }
}

/// Get the last pushed token in the token stream.
ClosureBuffer::TkListIterT ClosureBuffer::LastStreamed() { return std::prev(token_stream_.end()); }

void ClosureBuffer::StreamPushBack(Tk token) { token_stream_.push_back(token); }

Vec<Tk> ClosureBuffer::StreamToVector() {
  // Temporarily remove sentinel begin token.
  token_stream_.pop_front();
  Vec<Tk> vec;
  vec.reserve(token_stream_.size());
  for (auto& tk : token_stream_) {
    vec.push_back(tk);
  }
  // Re-add sentinel begin token.
  token_stream_.push_front(kSentinelBegin);
  return vec;
}
}  // namespace cnd

/// @} // end of cand_compiler_data_closure

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////