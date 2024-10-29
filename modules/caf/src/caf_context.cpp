#include "caf_context.hpp"

namespace caf {
std::unique_ptr<sf::RenderWindow> MakeSFMLWindow(std::string win_title,
                                                 const WinHints& hints) {
  std::unique_ptr<sf::RenderWindow> window_uptr;
  sf::Uint32 style = sf::Style::Default;
  sf::VideoMode video_mode = sf::VideoMode::getDesktopMode();
  sf::String title = win_title;
  sf::ContextSettings gl_settings{};

  // Handle Pre-Launch settings
  // set window size
  if (hints.InitialWidth) video_mode.width = hints.InitialWidth.value();
  if (hints.InitialHeight) video_mode.height = hints.InitialHeight.value();

  // window border/titlebar/fullscreen flags
  if (hints.Fullscreen.value_or(kFalseInt))
    style = sf::Style::Fullscreen;
  else if (hints.NoTitleBar.value_or(kFalseInt))
    style = sf::Style::None;
  else if (hints.NoResize.value_or(kFalseInt))
    if (hints.NoCloseButton.value_or(kFalseInt))
      style = sf::Style::Titlebar;
    else
      style = sf::Style::Close;
  else if (hints.NoCloseButton.value_or(kFalseInt))
    style = sf::Style::Resize;
  else
    style = sf::Style::Default;

  // Opengl context hints
  if (hints.GlAntialiasingLevel)
    gl_settings.antialiasingLevel = hints.GlAntialiasingLevel.value();
  if (hints.GlStencilBits)
    gl_settings.stencilBits = hints.GlStencilBits.value();
  if (hints.GlMajorVersion)
    gl_settings.majorVersion = hints.GlMajorVersion.value();
  if (hints.GlMinorVersion)
    gl_settings.minorVersion = hints.GlMinorVersion.value();
  if (hints.GlAttributeFlags)
    gl_settings.attributeFlags = hints.GlAttributeFlags.value();
  if (hints.GlsRgbCapable)
    gl_settings.sRgbCapable = hints.GlsRgbCapable.value();

  // Create window
  window_uptr =
      std::make_unique<sf::RenderWindow>(video_mode, title, style, gl_settings);

  // Handle post-launch settings
  // Initalize imgui
  if (hints.EnableImGui.value_or(kFalseInt)) ImGui::SFML::Init(*window_uptr);
  if (hints.EnableImGui.value_or(kFalseInt))
    window_uptr->setFramerateLimit(hints.FrameLimit.value());
  if (hints.EnableVsync.value_or(kFalseInt))
    window_uptr->setVerticalSyncEnabled(true);
  if (hints.Hidden.value_or(kFalseInt)) window_uptr->setVisible(false);
  if (hints.RequestFocus.value_or(kFalseInt)) window_uptr->requestFocus();

  return window_uptr;
}
}  // namespace caf