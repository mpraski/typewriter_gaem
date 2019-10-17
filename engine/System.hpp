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
class System final {
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

    static const auto &instance() {
      static System system{
          sf::VideoMode(500, 500),
          "./res/fonts",
          "./res/sounds",
          "./res/textures",
          "./res/configs"
      };
      return system;
    }

    System(const System &p) = delete;

    System(System &&p) = delete;

    System &operator=(const System &p) = delete;

    System &operator=(System &&p) = delete;

    RESOURCE_GETTER(fonts);

    RESOURCE_GETTER(sounds);

    RESOURCE_GETTER(textures);

    RESOURCE_GETTER(configs);

    float effectivePageWidth() const;

    float effectivePageHeight() const;

    sf::Vector2f mousePosition() const;

    void setMouseClick(const sf::Vector2f &pos) const;

    bool mouseClickAvailable() const;

    const sf::Vector2f &mouseClickPosition() const;

    void setCursor(Cursor c) const;

    sf::RenderWindow &getWindow() const;

    sf::VideoMode mVideoMode;
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
    System(
        sf::VideoMode mode,
        const std::string &fonts_path,
        const std::string &sounds_path,
        const std::string &textures_path,
        const std::string &configs_path
    );

    // Resource maps, can be publicly
    // accessed through methods generated
    // by RESOURCE_GETTER macro
    ResourceMap<sf::Font> fonts;
    ResourceMap<sf::SoundBuffer> sounds;
    ResourceMap<sf::Texture> textures;
    ResourceMap<pt::ptree> configs;
    // Some other shit
    mutable Cursor currentCursor;
    mutable sf::Cursor arrowCursor;
    mutable sf::Cursor grabCursor;
    mutable sf::RenderWindow mWindow;
    mutable bool mMousePressed;
    mutable sf::Vector2f mMousePressedPosition;

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

      for (const auto &resDirPath : boost::make_iterator_range(fs::directory_iterator(resourcePath), {})) {
        if (fs::is_directory(resDirPath)) {
          auto resource_category{getSubDirectory(resDirPath.path())};

          for (const auto &resource_file_path : boost::make_iterator_range(fs::directory_iterator(resDirPath),
                                                                           {})) {
            creator(resource_category, resource_file_path);
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
