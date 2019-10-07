//
// Created by marcin on 7/21/19.
//

#include "System.hpp"

engine::System::System(
    sf::VideoMode mode,
    const std::string &fonts_path,
    const std::string &sounds_path,
    const std::string &textures_path,
    const std::string &configs_path
)
    : mVideoMode{mode},
      currentCursor{Cursor::ARROW},
      arrowCursor{},
      grabCursor{},
      mWindow{mode, "Typewriter Gaem"},
      mFont{},
      fonts{
          loadResources<sf::Font>(
              fonts_path,
              [](const auto &path) -> std::optional<sf::Font> {
                if (sf::Font f; f.loadFromFile(path)) {
                  return f;
                }
                return {};
              }
          )
      },
      sounds{
          loadResources<sf::SoundBuffer>(
              sounds_path,
              [](const auto &path) -> std::optional<sf::SoundBuffer> {
                if (sf::SoundBuffer buffer; buffer.loadFromFile(path)) {
                  return buffer;
                }
                return {};
              }
          )
      },
      textures{
          loadResources<sf::Texture>(
              textures_path,
              [](const auto &path) -> std::optional<sf::Texture> {
                if (sf::Texture texture; texture.loadFromFile(path)) {
                  return texture;
                }
                return {};
              }
          )
      },
      configs{
          loadResources<pt::ptree>(
              configs_path,
              [](const auto &path) -> std::optional<pt::ptree> {
                pt::ptree pt;
                try {
                  pt::json_parser::read_json(path, pt);
                } catch (const pt::json_parser_error &e) {
                  throw std::runtime_error("Could not parse JSON config file: " + path);
                }
                return pt;
              }
          )
      },
      mMousePressed{},
      mMousePressedPosition{} {
  const auto &pageConfig{get_configs("page")};
  mFontSize = pageConfig.get<unsigned>("page.font_size");
  mPageWidth = pageConfig.get<float>("page.page_width");
  mPageHeight = pageConfig.get<float>("page.page_height");
  mMarginVertical = pageConfig.get<float>("page.margin_vertical");
  mMarginHorizontal = pageConfig.get<float>("page.margin_horizontal");
  mLetterSpacingFactor = pageConfig.get<float>("page.mLetterSpacingFactor");
  mLineSpacingFactor = pageConfig.get<float>("page.line_spacing_factor");
  mTypingDelay = pageConfig.get<unsigned>("page.typing_delay");

  auto page_font{pageConfig.get<std::string>("page.font")};
  mFont = &get_fonts(page_font);

  if (!arrowCursor.loadFromSystem(sf::Cursor::Type::Arrow)) {
    throw std::runtime_error("Arrow Cursor loading error");
  }

  if (!grabCursor.loadFromSystem(sf::Cursor::Type::Hand)) {
    throw std::runtime_error("Grab Cursor loading error");
  }

  mWhitespaceWidth = mFont->getGlyph(L' ', mFontSize, false).advance;
  mLetterSpacing = (mWhitespaceWidth / 3.f) * (mLetterSpacingFactor - 1.f);
  mLineSpacing = mFont->getLineSpacing(mFontSize) * mLineSpacingFactor;
  mLineSpacingMargin = mLineSpacing / 1.3f;
  mUnderlineOffset = mFont->getUnderlinePosition(mFontSize);
  mUnderlineThickness = mFont->getUnderlineThickness(mFontSize);

  mWhitespaceWidth += mLetterSpacing;
}

sf::Vector2f engine::System::mousePosition() const {
  return sf::Vector2f{sf::Mouse::getPosition(mWindow)};
}

void engine::System::set_mouse_click(const sf::Vector2f &pos) const {
  mMousePressed = true;
  mMousePressedPosition = pos;
}

bool engine::System::mouse_click_available() const {
  return mMousePressed;
}

const sf::Vector2f &engine::System::mouse_click_position() const {
  mMousePressed = false;
  return mMousePressedPosition;
}

void engine::System::set_cursor(Cursor c) const {
  if (c == currentCursor) return;
  currentCursor = c;
  switch (c) {
    case Cursor::ARROW:
      mWindow.setMouseCursor(arrowCursor);
      break;
    case Cursor::HAND:
      mWindow.setMouseCursor(grabCursor);
      break;
  }
}

sf::RenderWindow &engine::System::get_window() const {
  return mWindow;
}

float engine::System::effectivePageWidth() const {
  return mPageWidth - 2 * mMarginHorizontal;
}

float engine::System::effectivePageHeight() const {
  return mPageHeight - 2 * mMarginVertical;
}
