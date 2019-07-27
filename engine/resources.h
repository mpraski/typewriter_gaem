//
// Created by marcin on 7/21/19.
//

#ifndef TYPEWRITER_GAEM_RESOURCES_H
#define TYPEWRITER_GAEM_RESOURCES_H

#include <memory>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <functional>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace fs = std::filesystem;

#define RESOURCE_GETTER(RMAP) \
inline const auto &get_##RMAP(const std::string &res_category = ROOT_RESOURCE_CATEGORY) const { \
  try { \
    return (RMAP).at(res_category); \
  } catch (const std::out_of_range &ex) { \
    throw std::runtime_error(std::string{#RMAP} + " category not found: " + res_category); \
  } \
}

namespace engine {
template<class T>
using resource_pack = std::unordered_map<std::string, T>;

template<class T>
using resource_map = std::unordered_map<std::string, resource_pack<T>>;

class resources {
    const constexpr static auto DEFAULT_FONT = "MonoSpatial";
public:
    const constexpr static auto ROOT_RESOURCE_CATEGORY = "<root>";

    resources(
        sf::VideoMode mode,
        const std::string &fonts_path,
        const std::string &sounds_path,
        unsigned font_size = 24u,
        float page_width = 400.f,
        float page_height = 1000.f,
        float margin_vertical = 20.f,
        float margin_horizontal = 20.f,
        float letter_spacing_factor = 1.f,
        float line_spacing_factor = 1.f,
        unsigned typing_delay = 500u
    );

    resources(const resources &p) = delete;

    resources(resources &&p) = delete;

    resources &operator=(const resources &p) = delete;

    resources &operator=(resources &&p) = delete;

    RESOURCE_GETTER(fonts);

    RESOURCE_GETTER(sounds);

    void display(const std::function<void(sf::RenderWindow &w)> &f) const;

    sf::Vector2i mouse_position() const;

    bool mouse_moved() const;

    void set_cursor(sf::Cursor::Type type) const;

    sf::VideoMode mode;
    mutable sf::Vector2i curr_mouse, prev_mouse;
    mutable sf::Cursor::Type cursor_type;
    mutable sf::RenderWindow window;
    const sf::Font *font;
    unsigned font_size;
    float page_width;
    float page_height;
    float margin_vertical;
    float margin_horizontal;
    float letter_spacing_factor;
    float line_spacing_factor;
    unsigned typing_delay;

private:
    resource_map<sf::Font> fonts;
    resource_map<sf::SoundBuffer> sounds;

    template<typename It>
    static void split(const std::string &s, char delim, It result) {
      std::stringstream ss(s);
      std::string item;
      while (std::getline(ss, item, delim)) {
        *(result++) = item;
      }
    }

    static std::string get_sub_directory(const std::filesystem::path &path) {
      constexpr const auto separator{fs::path::preferred_separator};

      std::vector<std::string> parts;
      split(path, separator, std::back_inserter(parts));

      if (parts.empty()) {
        throw std::runtime_error("Provided path does not contain sub directory: " + path.string());
      }

      return parts[parts.size() - 1];
    }

    template<class T>
    static resource_map<T> load_resources(
        const std::string &resource_path,
        std::function<std::optional<T>(const std::string &)> resource_builder
    ) {
      resource_map<T> resources;

      auto creator{[&](const std::string &resource_category, const auto &resource_file_path) {
        if (resource_file_path.is_directory()) {
          throw std::runtime_error("Resource file path is a directory: " + resource_file_path.path().string());
        }

        auto path{resource_file_path.path().string()};
        auto name{resource_file_path.path().stem().string()};

        if (auto res{resource_builder(path)}; res) {
          resources[resource_category].emplace(name, std::move(*res));
        } else {
          throw std::runtime_error("Unable to load resource from path: " + path);
        }
      }};

      for (const auto &resource_dir_path : fs::directory_iterator(resource_path)) {
        if (resource_dir_path.is_directory()) {
          auto resource_category{get_sub_directory(resource_dir_path.path())};

          for (const auto &resource_file_path : fs::directory_iterator(resource_dir_path)) {
            creator(resource_category, resource_file_path);
          }
        } else {
          creator(ROOT_RESOURCE_CATEGORY, resource_dir_path);
        }
      }

      return resources;
    }
};

using resources_ptr = std::shared_ptr<const resources>;

template<class ...Ts>
static auto make_resources(Ts &&... args) {
  return std::make_shared<const resources>(std::forward<Ts>(args)...);
}

template<class Base, class T>
inline bool instanceof(const T *ptr) {
  return dynamic_cast<const Base *>(ptr) != nullptr;
}
}

#endif //TYPEWRITER_GAEM_RESOURCES_H
