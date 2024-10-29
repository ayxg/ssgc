#pragma once
#include "caf_config.hpp"
#include "cxxx.hpp"

namespace caf {
enum class eAPIError { kNone = 0, kFatal };

struct WinHints {
  std::optional<int> RequestFocus{std::nullopt};   // Request focus
  std::optional<int> Hidden{std::nullopt};         // Window is hidden
  std::optional<int> Fullscreen{std::nullopt};     // Fullscreen window
  std::optional<int> NoTitleBar{std::nullopt};     // No title bar
  std::optional<int> NoResize{std::nullopt};       // Window is resizable
  std::optional<int> NoCloseButton{std::nullopt};  // Show close button
  std::optional<int> InitialWidth{std::nullopt};   // Initial window width
  std::optional<int> InitialHeight{std::nullopt};  // Initial window height
  std::optional<int> FrameLimit{
      std::nullopt};  // default is FalseInt; which means no limit
  std::optional<int> EnableVsync{std::nullopt};  // Enable vertical sync

  std::optional<int> GlDepthBits{std::nullopt};
  std::optional<int> GlAntialiasingLevel{std::nullopt};
  std::optional<int> GlStencilBits{std::nullopt};
  std::optional<int> GlMajorVersion{std::nullopt};
  std::optional<int> GlMinorVersion{std::nullopt};
  std::optional<int> GlAttributeFlags{
      std::nullopt};  // One of sf::ContextSettings::AttributeFlags
  std::optional<int> GlsRgbCapable{std::nullopt};

  std::optional<int> EnableImGui{std::nullopt};
};

std::unique_ptr<sf::RenderWindow> MakeSFMLWindow(std::string win_title,
                                                 const WinHints& hint_map);

struct SFMLWindow {
  std::unique_ptr<sf::RenderWindow> win_uptr;
  WinHints hints;  ///> Hints passed to create method

  SFMLWindow(std::string win_title, const WinHints& hints)
      : hints(hints), win_uptr(MakeSFMLWindow(win_title, hints)) {}
};

struct Context {
  std::vector<SFMLWindow> windows_;
  sf::Event event_cache_;  ///> Current event processor cache
  bool is_imgui_frame_updated_{false};
  sf::RenderWindow& MakeWindow(std::string win_title, const WinHints& hints) {
    windows_.push_back(SFMLWindow(win_title, hints));
    return *windows_.back().win_uptr;
  }

  eAPIError ProcessEvents(
      sf::RenderWindow& window,
      std::function<void(const sf::Event&)> event_handler =
          [](const sf::Event&) {}) {
    sf::Event event_cache_;
    while (window.pollEvent(event_cache_)) {  // SFML Event Loop
      ImGui::SFML::ProcessEvent(window,
                                event_cache_);  // IMGUI Events Processor
      event_handler(event_cache_);              // User defined event handler
    }
    return eAPIError::kNone;
  }

  eAPIError UpdateFrame(
      sf::RenderWindow& window, const sf::Time& delta,
      std::function<eAPIError(sf::RenderWindow&, const sf::Time&)>
          frame_handler = [](sf::RenderWindow&, const sf::Time&) {
            return eAPIError::kNone;
          }) {
    if (!is_imgui_frame_updated_)
      for (auto& win : windows_) {
        if (win.win_uptr != nullptr && win.win_uptr->isOpen())
          ImGui::SFML::Update(*win.win_uptr, delta);
        is_imgui_frame_updated_ = true;
      };
    ImGui::SFML::SetCurrentWindow(window);
    return frame_handler(window, delta);
  }

  eAPIError CleanupContextFrame() {
    is_imgui_frame_updated_ = false;
    return eAPIError::kNone;
  }
};

}  // namespace caf
