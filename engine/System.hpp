//
// Created by marcin on 7/21/19.
//

#ifndef TYPEWRITER_GAEM_SYSTEM_HPP
#define TYPEWRITER_GAEM_SYSTEM_HPP

#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Utilities/General.hpp"
#include "Utilities/NonCopyable.h"
#include "Utilities/NonMovable.h"
#include "EventBus/EventBus.hpp"
#include "EventBus/SynchronousListeners.hpp"
#include "EventBus/QueuedListeners.hpp"
#include "Utilities/Logger.hpp"

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

#define RESOURCE_GETTER(RMAP) \
inline const auto &get_##RMAP##_category(const std::string &res_category = ROOT_RESOURCE_CATEGORY) const { \
  try { \
    return (RMAP).at(res_category); \
  } catch (const std::out_of_range &ex) { \
    throw std::runtime_error(std::string{#RMAP} + " category not found: " + res_category); \
  } \
} \
inline const auto &get_##RMAP(const std::string & res_id, const std::string &res_category = ROOT_RESOURCE_CATEGORY) const { \
  try { \
    return (RMAP).at(res_category).at(res_id); \
  } catch (const std::out_of_range &ex) { \
    throw std::runtime_error(std::string{#RMAP} + " category or resource not found: (" + res_category + ", " + res_id + ")"); \
  } \
}

namespace engine {
class System final : private NonCopyable, private NonMovable {
public:
    template<class T>
    using ResourcePack = std::unordered_map<std::string, T>;

    template<class T>
    using ResourceMap = std::unordered_map<std::string, ResourcePack<T>>;

    const constexpr static auto ROOT_RESOURCE_CATEGORY = "<root>";

    enum class Cursor {
        ARROW,
        HAND
    };

    static auto &instance() {
      static System system{
          sf::VideoMode(500, 500),
          "./res/fonts",
          "./res/sounds",
          "./res/textures",
          "./res/configs",
          "./res/shaders"
      };
      return system;
    }

    static auto &bus() {
      static EventBus<SynchronousListeners> instance;
      return instance;
    }

    static auto &logger() {
      static Logger instance;
      return instance;
    }

    RESOURCE_GETTER(fonts);

    RESOURCE_GETTER(sounds);

    RESOURCE_GETTER(textures);

    RESOURCE_GETTER(configs);

    RESOURCE_GETTER(shaders);

    float effectivePageWidth() const;

    float effectivePageHeight() const;

    sf::Vector2f mousePosition() const;

    void setMouseClick(const sf::Vector2f &pos);

    bool mouseClickAvailable() const;

    const sf::Vector2f &mouseClickPosition();

    void setCursor(Cursor c);

    sf::RenderWindow &getWindow();

public:
    const sf::Font *mFont;
    unsigned mFontSize;
    unsigned mTypingDelay;
    float mPageWidth;
    float mPageHeight;
    float mMarginVertical;
    float mMarginHorizontal;
    float mLetterSpacingFactor;
    float mLineSpacingFactor;
    float mWhitespaceWidth;
    float mLetterSpacing;
    float mLineSpacing;
    float mLineSpacingMargin;
    float mUnderlineOffset;
    float mUnderlineThickness;

private:
    ResourceMap<sf::Font> fonts;
    ResourceMap<sf::SoundBuffer> sounds;
    ResourceMap<sf::Texture> textures;
    ResourceMap<pt::ptree> configs;
    ResourceMap<std::unique_ptr<sf::Shader>> shaders;

    sf::VideoMode mVideoMode;
    sf::RenderWindow mWindow;
    bool mMousePressed;
    Cursor currentCursor;
    sf::Cursor arrowCursor;
    sf::Cursor grabCursor;
    sf::Vector2f mMousePressedPosition;

private:
    System(
        sf::VideoMode mode,
        const std::string &fonts_path,
        const std::string &sounds_path,
        const std::string &textures_path,
        const std::string &configs_path,
        const std::string &shaders_path
    );

    static ResourceMap<std::unique_ptr<sf::Shader>> loadShaders(const std::string &resourcePath);

    static std::string getSubDirectory(const fs::path &path) {
      constexpr const auto separator{fs::path::preferred_separator};
      std::vector<std::string> parts;
      gen::split(path.string(), separator, std::back_inserter(parts));

      if (parts.empty()) {
        throw std::runtime_error("Provided path does not contain sub directory: " + path.string());
      }

      return *gen::last(parts);
    }

    template<class T>
    static ResourceMap<T> loadResources(
        const std::string &resourcePath,
        std::function<std::optional<T>(const std::string &)> resourceBuilder
    ) {
      ResourceMap<T> resources;

      auto creator{[&](const std::string &resCategory, const auto &resFilePath) {
        if (fs::is_directory(resFilePath)) {
          throw std::runtime_error("Resource file path is a directory: " + resFilePath.path().string());
        }

        auto path{resFilePath.path().string()};
        auto name{resFilePath.path().stem().string()};

        if (auto res{resourceBuilder(path)}; res) {
          resources[resCategory].insert(std::make_pair(name, std::move(*res)));
        } else {
          throw std::runtime_error("Unable to load resource from path: " + path);
        }
      }};

      if (!fs::exists(resourcePath)) {
        throw std::runtime_error(gen::str("Resouce path ", resourcePath, " does not exist!"));
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

      return resources;
    }
};
}

#endif //TYPEWRITER_GAEM_SYSTEM_HPP
