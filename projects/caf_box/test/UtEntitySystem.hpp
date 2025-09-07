///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup coreappfw
/// @brief [UNIT-TEST] UtEntitySystem module
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

// clang-format off
#pragma once
#include "minitest.hpp"
#include "cxxx.hpp"
#include "caf.hpp"
// clang-format on

namespace caf_unit_test {

// Mock end-user entity class.
struct TUserMock {};

struct TSampleWindow {
  using UnderType = sf::RenderWindow;
  std::unique_ptr<UnderType> under_{nullptr};

  void entCreate(caf::Ent& self_ent) { std::cout << "THudContext::entCreate" << std::endl; }
  void entDestroy(caf::Ent& self_ent) { std::cout << "THudContext::entDestroy" << std::endl; }
  void entHandleOsEvent(const caf::OsEventType& event) { std::cout << "THudContext::entHandleOsEvent" << std::endl; }
  // bool entProcessEvents(caf::Ent& self_ent, caf::EventBufferType& evs) {
  //   // Process systems here.
  //   std::cout << "THudContext::entProcessEvents" << std::endl;
  //   //return self_ent.EntProcessBranchEvents(evs);
  //   return true;
  // }
};
//static_assert(caf::UserObjectRequirements<TSampleWindow>);

TEST(UtEntitySystem, NodeDataStructure_Constructor) {
  TUserMock userMock{};
  caf::Ent e1 = TUserMock{};      // forward ref -> direct init and allocate
  caf::Ent e2 = new TUserMock{};  // pointer -> take ownership and direct re-alloc
  caf::Ent e3 = userMock;         // const ref -> copy and allocate
}

TEST(UtEntitySystem, NodeDataStructure_Push) {
  caf::Ent e1 = TUserMock{};
  caf::Ent* e2 = e1.Push(TUserMock{});
  caf::Ent* e3 = e1.Push(TUserMock{});
  caf::Ent* e4 = e2->Push(TUserMock{});
  caf::Ent* e5 = e2->Push(TUserMock{});

  // Check if the entity's pointers are correctly linked
  EXPECT_EQ(e1.Stem(), nullptr);
  EXPECT_EQ(e2->Stem(), &e1);
  EXPECT_EQ(e3->Stem(), &e1);
  EXPECT_EQ(e4->Stem(), e2);
  EXPECT_EQ(e5->Stem(), e2);

  // Levels
  EXPECT_EQ(e1.Level(), 0);
  EXPECT_EQ(e2->Level(), 1);
  EXPECT_EQ(e3->Level(), 1);
  EXPECT_EQ(e4->Level(), 2);
  EXPECT_EQ(e5->Level(), 2);

  // Branch counts
  EXPECT_EQ(e1.BranchCount(), 2);   // e2 and e3
  EXPECT_EQ(e2->BranchCount(), 2);  // e4 and e5
  EXPECT_TRUE(e3->IsLeaf());
  EXPECT_TRUE(e4->IsLeaf());
  EXPECT_TRUE(e5->IsLeaf());
}

TEST(UtEntitySystem, NodeDataStructureImpl) {
  caf::Ent e1 = TUserMock{};
  caf::Ent* e2 = e1.Push(TUserMock{});
  caf::Ent* e3 = e2->Push(TUserMock{});

  // IsRoot
  EXPECT_TRUE(e1.IsRoot());
  EXPECT_FALSE(e2->IsRoot());
  EXPECT_FALSE(e3->IsRoot());

  // IsLeaf
  EXPECT_FALSE(e1.IsLeaf());
  EXPECT_FALSE(e2->IsLeaf());
  EXPECT_TRUE(e3->IsLeaf());

  // IsTrunk
  EXPECT_FALSE(e1.IsTrunk());
  EXPECT_TRUE(e2->IsTrunk());
  EXPECT_FALSE(e3->IsTrunk());

  EXPECT_EQ(e1.Level(), 0);
  EXPECT_EQ(e2->Level(), 1);
  EXPECT_EQ(e3->Level(), 2);
}

TEST(UtEntitySystem, NodeDataStructureImpl_PopBack) {
  caf::Ent e1 = TUserMock{};
  caf::Ent* e2 = e1.Push(TUserMock{});
  caf::Ent* e3 = e2->Push(TUserMock{});

  // IsRoot
  EXPECT_TRUE(e1.IsRoot());
  EXPECT_FALSE(e2->IsRoot());
  EXPECT_FALSE(e3->IsRoot());

  // IsLeaf
  EXPECT_FALSE(e1.IsLeaf());
  EXPECT_FALSE(e2->IsLeaf());
  EXPECT_TRUE(e3->IsLeaf());

  // IsTrunk
  EXPECT_FALSE(e1.IsTrunk());
  EXPECT_TRUE(e2->IsTrunk());
  EXPECT_FALSE(e3->IsTrunk());

  EXPECT_EQ(e1.Level(), 0);
  EXPECT_EQ(e2->Level(), 1);
  EXPECT_EQ(e3->Level(), 2);

  caf::EventBufferType eb{};
  e1.ProcEvents(eb);
}

TEST(UtEntitySystem, BasicWindowTree) {
  caf::Ent e1 = TSampleWindow{};
  caf::Ent* e2 = e1.Push(TSampleWindow{});
  caf::Ent* e3 = e2->Push(TSampleWindow{});

  EXPECT_TRUE(e1.IsRoot());
  EXPECT_FALSE(e2->IsRoot());
  EXPECT_FALSE(e3->IsRoot());

  EXPECT_FALSE(e1.IsLeaf());
  EXPECT_FALSE(e2->IsLeaf());
  EXPECT_TRUE(e3->IsLeaf());

  EXPECT_FALSE(e1.IsTrunk());
  EXPECT_TRUE(e2->IsTrunk());
  EXPECT_FALSE(e3->IsTrunk());

  EXPECT_EQ(e1.Level(), 0);
  EXPECT_EQ(e2->Level(), 1);
  EXPECT_EQ(e3->Level(), 2);

  auto& pwin = e1.As<TSampleWindow>().under_;
  auto& pwin2 = e2->As<TSampleWindow>().under_;
  auto& pwin3 = e3->As<TSampleWindow>().under_;
  pwin = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Main");
  pwin2 = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Main->Child");
  pwin3 = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Main->Child->Child");
  while (pwin->isOpen()) {
    sf::Event event;
    // Poll all window events
    while (pwin->pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        pwin->close();
      }
    }

    if (pwin2 && pwin2->isOpen()) {
      while (pwin2->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          pwin2->close();
        }
      }
    }

    if (pwin3 && pwin3->isOpen()) {
      while (pwin3->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          pwin3->close();
        }
      }
    }

    // Render all windows
    pwin->clear(sf::Color::Black);
    pwin->display();

    if (pwin2 && pwin2->isOpen()) {
      pwin2->clear(sf::Color::Black);
      pwin2->display();
    }

    if (pwin3 && pwin3->isOpen()) {
      pwin3->clear(sf::Color::Black);
      pwin3->display();
    }
  }
}

TEST(UtEntitySystem, UsingTheCafEngine) { 
  caf::Ent t_app = caf::TGUIApp{};

  // Main Window setup
  caf::GLWindowHints win_hints{};
  win_hints.InitialTitle = "Main Window";
  auto* main_window = t_app.Push(caf::TGLWindow{win_hints});

  main_window->PushEventHandler([](caf::Ent& self_ent, const caf::OsEventType& ev) {
    if (ev.type == sf::Event::Closed) {
      self_ent.As<caf::TGLWindow>().Close();
      //self_ent.DestroyLater();
    }
    return caf::eProcRes::kContinue;
  });

  // Add a HUD context
  auto* hud_ctx = main_window->Push(caf::TImContext{});
  auto* main_frame = hud_ctx->Push(caf::THUDWindow{})->Push(caf::TImFrame{});
  main_frame->Push(caf::TImTextLabel{"This is a label."});
  main_frame->Push(caf::TImButton{"A Button."});

  while (main_window->IsOpen()) {
    sf::Event event;
    // Poll all window events
    while (main_window->PollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        main_window->Close();
      }
      main_window->HandleOsEvent(event);
    }
    // Process events in the HUD context
    hud_ctx->ProcEvents(caf::EventBufferType{});
    // Render the main window
    main_window->Clear(sf::Color::Black);
    main_window->Display();
  }



}

}  // namespace caf_unit_test

/// @} // end of coreappfw

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/07/23
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
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
