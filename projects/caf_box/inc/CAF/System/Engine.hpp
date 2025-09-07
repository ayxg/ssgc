///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup core_app_framework
/// @brief [HEADER]
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup core_app_framework
/// @{

// clang-format off
#pragma once
#include "cxxx.hpp"
#include "CAF/PreConfig.hpp"
#include "CAF/System/EngineMeta.hpp"
#include "CAF/System/Entity.hpp"
// clang-format on

namespace caf {
class Engine;

using OSEventType = sf::Event;

/// Access CoreApp Framework global engine singleton.
static constexpr Engine& TheEngine() {
  static Engine engine;
  return engine;
}

class Engine {
  std::vector<Ent*> ents_;              // Detached entity node graphs.
  std::unordered_set<Ent*> ents_live_;  // Allows user to query invalidated/deleted windows.

  // Entities registered as OS event generators paired with their recievers.
  std::vector<std::pair<Ent*, std::vector<Ent*>>> os_event_generators_;  
  OSEventType os_event_cache_;
  EventBufferType event_buffer_;

  Ent* ent_last_created_;
  Ent* curr_ent_;

 public:
  eProcRes ProcessOSEvents() {
    // All OS events are processed first regardless of the entity node location. It is recommended to forward any needed
    // OS events to the ProcEvents method of an entity. This way you can maintain the order of reactions to events with
    // the CAF event system. This method will be called frequently on average ~hundreds of times per frame, heavy
    // processing may result in an unresponsive program.
    for (auto& generator : os_event_generators_) {
      while (generator.first->PollEvent(os_event_cache_)) {
        // Pass events to self first if it overrides handle events.
        if (generator.first->OverridesHandleOsEvent()) generator.first->ProcOsEvent(os_event_cache_);
        
        // Dispatch to all recievers
        for (auto* reciever : generator.second) {
          eProcRes proc_res = reciever->HandleOsEvent(os_event_cache_);
          if (proc_res == eProcRes::kStop) break; 
        }
      }
    }
  }

  constexpr eProcRes ProcEvents(EventBufferType& evs) {
    // Process os events first see `ProcessOSEvents()` for details.
    if (ProcessOSEvents() == eProcRes::kStop) return;
    
    // Process events for all detached entities and their subnodes.
    for (auto& ent : ents_) {
      if (ent->OverridesProcEvents()) {
        eProcRes res = ent->ProcEvents(evs);
        if (res == eProcRes::kStop) return res;
      }
    }
  }

  eProcRes ProcessFrame() { 
    // Events are processed first, always, regardless of anything.
    eProcRes res_proc_events = ProcEvents(event_buffer_);
    
    // Apply Async Preparation (start frame)
    eProcRes res_proc_systems = PrepSystems(event_buffer_);
    eProcRes res_proc_audio = PrepAudio(event_buffer_);
    eProcRes res_proc_graphics = PrepGraphics(event_buffer_);

    // Apply Sequential Processing (update)
    eProcRes res_proc_systems = ProcSystems(event_buffer_);
    eProcRes res_proc_audio = ProcAudio(event_buffer_);
    eProcRes res_proc_graphics = ProcGraphics(event_buffer_);
    
    // Apply Async Post Processing (finish frame).
    eProcRes res_post_systems = FinishSystems(event_buffer_);
    eProcRes res_post_audio = FinishAudio(event_buffer_);
    eProcRes res_post_graphics = FinishGraphics(event_buffer_);

    return eProcRes::kContinue;
  }
};
}  // namespace caf

/// @} // end of core_app_framework

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/07/26
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
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
