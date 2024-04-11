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
// Directory: cand-official-compiler
// File: caoco_token_closure.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_TOKEN_CLOSURE_H
#define HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_TOKEN_CLOSURE_H
//---------------------------------------------------------------------------//
// Brief: Closure and ClosureBuffer classes for shift reduction parsing.
//---------------------------------------------------------------------------//
#include "cppsextended.h"
// Includes:
#include "caoco_enum.h"
#include "caoco_token.h"
//---------------------------------------------------------------------------//

namespace caoco {
class Closure;
using TkList = std::list<Tk>;
using TkListIter = TkList::iterator;
using CosureList = std::list<Closure>;
using ClosureListIter = CosureList::iterator;

//---------------------------------------------------------//
// Class:{Closure}
// Brief:{
//  A closure is a range of tokens representing a singular ast node.
//  All tokens from front to back are a single ast node with no children.
//  When a closure is a single token, the front and back are the same.
//  Example: 1 + 2 -> Closure #2 is +, •+ is the front and +• is the back.
//  Example: 1(abcd) + 2 -> Closure #2 is (abcd), •(abcd) is the front
//  and (abcd)• is the back.
// }
//---------------------------------------------------------//
class Closure {
 public:
  TkListIter Front() const { return front_; }
  TkListIter Back() const { return back_; }

  // Operation of the front token. ( [ { are postfix operators.
  eOperation Operation() const {
    // Special case for (, [, {, and postfix operators.
    if (front_->TypeIs(eTk::kOpenParen) || front_->TypeIs(eTk::kOpenBrace) ||
        front_->TypeIs(eTk::kOpenBracket)) {
      return eOperation::kPostfix;
    } else {
      return front_->Operation();
    }
  }

  // Priority of the front token. ( [ { are postfix priority.
  ePriority Priority() const {
    // Special case for (, [, {, are postfix priority.
    if (front_->TypeIs(eTk::kOpenParen) || front_->TypeIs(eTk::kOpenBrace) ||
        front_->TypeIs(eTk::kOpenBracket)) {
      return ePriority::kPostfix;
    } else {
      return front_->Priority();
    }
  }

  // Associativity of the front token.
  eAssoc Assoc() const { return front_->Assoc(); }

 public:
  Closure(TkListIter front) : front_(front), back_(front) {}
  Closure(TkListIter front, TkListIter back) : front_(front), back_(back) {}

 private:
  TkListIter front_;
  TkListIter back_;
};
//---------------------------------------------------------//
// EndClass:{Closure}
//---------------------------------------------------------//

//---------------------------------------------------------//
// Class:{ClosureBuffer}
// Brief:{
//  Container for a list of closures referencing a token stream.
// }
//---------------------------------------------------------//
class ClosureBuffer {
 public:  // Closure Operations
  void PushFrontClosure(TkListIter front) {
    closures_.push_front(Closure(front));
  }
  void PushFrontClosure(TkListIter front, TkListIter back) {
    closures_.push_front(Closure(front, back));
  }
  void PushBackClosure(TkListIter front) {
    closures_.push_back(Closure(front));
  }
  void PushBackClosure(TkListIter front, TkListIter back) {
    closures_.push_back(Closure(front, back));
  }
  void PopClosure() { closures_.pop_back(); }
  void PopClosure(ClosureListIter which) { closures_.erase(which); }
  void PopClosureIf(auto&& condition) {
    closures_.remove_if(std::move(condition));
  }

  // Returns the size of the internal closure list minus the sentinel begin.
  std::size_t ClosureCount() const { return closures_.size() - 1; }

  // Returns iter to the last closure.(One before the end).
  ClosureListIter LastClosure() { return std::prev(closures_.end()); }

  // Returns the token iterator at the front of the last closure.
  TkListIter LastClosureFront() { return closures_.back().Front(); }

  // Returns the token iterator at the back of the last closure.
  TkListIter LastClosureBack() { return closures_.back().Back(); }

  // Search from the end of the closure list until first occurence.
  ClosureListIter FindClosureReverse(std::function<bool(Closure&)>&& condition);

  // Search from the end of the closure list for repeated occurences.
  std::vector<ClosureListIter> FindClosureReverseConsecutive(
      std::function<bool(ClosureListIter, ClosureListIter)>&& condition);

  // Search from the end of the closure list for repeated occurences
  // ignoring certain elements based on ignore_condition.
  std::vector<ClosureListIter> FindClosureReverseConsecutiveAndIgnore(
      std::function<bool(ClosureListIter, ClosureListIter)>&& condition,
      std::function<bool(ClosureListIter)>&& ignore_condition);

 public:  // Token stream operations
  // Insert token before the front of the specified closure in the token stream.
  // Closure must be retrieved from this ClosureBuffer instance.
  void StreamInsertBeforeClosure(ClosureListIter closure_it, Tk token);

  // Insert token after the back of the specified closure in the token stream.
  // Closure must be retrieved from this ClosureBuffer instance.
  void StreamInsertAfterClosure(ClosureListIter closure_it, Tk token);

  // Get the last pushed token in the token stream.
  TkListIter LastStreamed() { return std::prev(token_stream_.end()); }

  void StreamPushBack(Tk token) { token_stream_.push_back(token); }

  TkVector StreamToVector();

 public:
  ClosureBuffer() {
    // Create sentinel begin closure.
    // HAS to be set or else begin is the end before and after first insertion.
    token_stream_.push_back(kSentinelBegin);
    closures_.push_back(Closure(token_stream_.begin()));
    sentinel_begin_closure_ = closures_.begin();
  }

 private:
  static const Tk kSentinelBegin;
  TkList token_stream_;
  CosureList closures_;
  ClosureListIter sentinel_begin_closure_;
};
//---------------------------------------------------------//
// EndClass:{ClosureBuffer}
//---------------------------------------------------------//

const Tk ClosureBuffer::kSentinelBegin = {eTk::kNone,
                                          "ClosureBuffer sentinel begin."};

// Search from the end of the closure list until first occurence.
ClosureListIter ClosureBuffer::FindClosureReverse(
    std::function<bool(Closure&)>&& condition) {
  return std::find_if(closures_.rbegin(), closures_.rend(), condition).base();
}

std::vector<ClosureListIter> ClosureBuffer::FindClosureReverseConsecutive(
    std::function<bool(ClosureListIter, ClosureListIter)>&& condition) {
  auto it = closures_.rbegin();
  auto end = closures_.rend();
  std::vector<ClosureListIter> accumulated_iterators;
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

std::vector<ClosureListIter>
ClosureBuffer::FindClosureReverseConsecutiveAndIgnore(
    std::function<bool(ClosureListIter, ClosureListIter)>&& condition,
    std::function<bool(ClosureListIter)>&& ignore_condition) {
  auto it = closures_.rbegin();
  auto end = closures_.rend();
  std::vector<ClosureListIter> accumulated_iterators;
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

void ClosureBuffer::StreamInsertBeforeClosure(ClosureListIter closure_it,
                                              Tk token) {
  if (closure_it == sentinel_begin_closure_) {
    token_stream_.insert(std::next(token_stream_.begin()), token);
    return;
  }
  auto front = closure_it->Front();
  token_stream_.insert(front, token);
}

void ClosureBuffer::StreamInsertAfterClosure(ClosureListIter closure_it,
                                             Tk token) {
  auto back = closure_it->Back();
  auto next = std::next(back);
  if (next == token_stream_.end()) {
    token_stream_.push_back(token);
  } else {
    token_stream_.insert(next, token);
  }
}

TkVector ClosureBuffer::StreamToVector() {
  // Temporarily remove sentinel begin token.
  token_stream_.pop_front();
  TkVector vec;
  vec.reserve(token_stream_.size());
  for (auto& tk : token_stream_) {
    vec.push_back(tk);
  }
  // Re-add sentinel begin token.
  token_stream_.push_front(kSentinelBegin);
  return vec;
}

}  // namespace caoco

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
// Directory: cand-official-compiler
// File: caoco_token_closure.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_TOKEN_CLOSURE_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//