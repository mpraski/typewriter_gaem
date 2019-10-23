//
// Created by marcin on 7/21/19.
//

#include "System.hpp"

engine::System::System(
    sf::VideoMode mode,
    const std::string &fonts_path,
    const std::string &sounds_path,
    const std::string &textures_path,
    const std::string &configs_path,
    const std::string &shaders_path
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
      shaders{loadShaders(shaders_path)},
      mMousePressed{},
      mMousePressedPosition{} {
  const auto &pageConfig{get_configs("page")};
  mFontSize = pageConfig.get<unsigned>("page.font_size");
  mPageWidth = pageConfig.get<float>("page.page_width");
  mPageHeight = pageConfig.get<float>("page.page_height");
  mMarginVertical = pageConfig.get<float>("page.margin_vertical");
  mMarginHorizontal = pageConfig.get<float>("page.margin_horizontal");
  mLetterSpacingFactor = pageConfig.get<float>("page.letter_spacing_factor");
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

void engine::System::setMouseClick(const sf::Vector2f &pos) {
  mMousePressed = true;
  mMousePressedPosition = pos;
}

bool engine::System::mouseClickAvailable() const {
  return mMousePressed;
}

const sf::Vector2f &engine::System::mouseClickPosition() {
  mMousePressed = false;
  return mMousePressedPosition;
}

void engine::System::setCursor(Cursor c) {
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

sf::RenderWindow &engine::System::getWindow() {
  return mWindow;
}

float engine::System::effectivePageWidth() const {
  return mPageWidth - 2 * mMarginHorizontal;
}

float engine::System::effectivePageHeight() const {
  return mPageHeight - 2 * mMarginVertical;
}

engine::System::ResourceMap<std::unique_ptr<sf::Shader>>
engine::System::loadShaders(const std::string &resourcePath) {
  std::unordered_map<
      std::string,
      std::tuple<
          std::string,
          std::string,
          std::string
      >
  > foundFiles;
  ResourceMap<std::unique_ptr<sf::Shader>> shaders;

  auto creator{[&](const std::string &resCategory, const auto &resFilePath) {
    if (fs::is_directory(resFilePath)) {
      throw std::runtime_error("Resource file path is a directory: " + resFilePath.path().string());
    }

    auto path{resFilePath.path().string()};
    auto name{resFilePath.path().stem().string()};
    auto ext{resFilePath.path().extension().string()};

    auto it{gen::find_default(foundFiles, name)};

    std::get<0>(it->second) = resCategory;
    if (ext == "vert") {
      std::get<1>(it->second) = path;
    } else if (ext == "frag") {
      std::get<2>(it->second) = path;
    }
  }};

  if (!fs::exists(resourcePath)) {
    throw std::runtime_error(gen::str("Shader path ", resourcePath, " does not exist!"));
  }

  for (const auto &resDirPath : boost::make_iterator_range(fs::directory_iterator(resourcePath), {})) {
    if (fs::is_directory(resDirPath)) {
      auto resCat{getSubDirectory(resDirPath.path())};

      for (const auto &resFilePath : boost::make_iterator_range(fs::directory_iterator(resDirPath),
                                                                {})) {
        creator(resCat, resFilePath);
      }
    } else {
      creator(ROOT_RESOURCE_CATEGORY, resDirPath);
    }
  }

  for (const auto&[k, v] : foundFiles) {
    const auto&[cat, vert, frag] = v;

    auto shader{std::make_unique<sf::Shader>()};
    if (!vert.empty() && !frag.empty()) {
      if (!shader->loadFromFile(vert, frag)) {
        throw std::runtime_error("Could not load shader: " + k);
      }
    } else if (!vert.empty()) {
      if (!shader->loadFromFile(vert, sf::Shader::Vertex)) {
        throw std::runtime_error("Could not load vertex shader: " + k);
      }
    } else if (!frag.empty()) {
      if (!shader->loadFromFile(frag, sf::Shader::Fragment)) {
        throw std::runtime_error("Could not load fragment shader: " + k);
      }
    }

    shaders[cat].insert(std::make_pair(k, std::move(shader)));
  }

  return shaders;
}