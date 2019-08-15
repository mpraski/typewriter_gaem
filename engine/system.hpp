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
#include "utilities/general.hpp"

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
    throw std::runtime_error(std::string{#RMAP} + " category not found: " + res_category); \
  } \
}

namespace engine {
template<class T>
using resource_pack = std::unordered_map<std::string, T>;

template<class T>
using resource_map = std::unordered_map<std::string, resource_pack<T>>;

class system {
public:
    const constexpr static auto ROOT_RESOURCE_CATEGORY = "<root>";
    const constexpr static auto DEFAULT_DELAY = 100000.f;

    enum class cursor {
        ARROW,
        HAND
    };

    system(
        sf::VideoMode mode,
        const std::string &fonts_path,
        const std::string &sounds_path,
        const std::string &textures_path,
        const std::string &configs_path
    );

    system(const system &p) = delete;

    system(system &&p) = delete;

    system &operator=(const system &p) = delete;

    system &operator=(system &&p) = delete;

    RESOURCE_GETTER(fonts);

    RESOURCE_GETTER(sounds);

    RESOURCE_GETTER(textures);

    RESOURCE_GETTER(configs);

    void draw(const sf::Drawable &drawable) const;

    void display(const std::function<void(sf::RenderWindow &)> &f) const;

    void delay(float duration, float delay_factor = 1.f) const;

    bool mouse_moved() const;

    const sf::Vector2f &mouse_position() const;

    bool mouse_click_available() const;

    const sf::Vector2f &mouse_click_position() const;

    float effective_page_width() const;

    float effective_page_height() const;

    void set_cursor(cursor c) const;

    sf::VideoMode mode;
    const sf::Font *font;
    unsigned font_size;
    float page_width;
    float page_height;
    float margin_vertical;
    float margin_horizontal;
    float letter_spacing_factor;
    float line_spacing_factor;
    unsigned typing_delay;
    // Font dependent
    float whitespace_width;
    float letter_spacing;
    float line_spacing;
    float line_spacing_margin;
    float underline_offset;
    float underline_thickness;

private:
    resource_map<sf::Font> fonts;
    resource_map<sf::SoundBuffer> sounds;
    resource_map<sf::Texture> textures;
    resource_map<pt::ptree> configs;
    mutable cursor current_cursor;
    mutable sf::Cursor arrow_cursor;
    mutable sf::Cursor grab_cursor;
    mutable sf::Vector2f curr_mouse, prev_mouse;
    mutable sf::RenderWindow window;
    mutable bool mouse_pressed;
    mutable sf::Vector2f mouse_pressed_position;

    static std::string get_sub_directory(const fs::path &path) {
      constexpr const auto separator{fs::path::preferred_separator};
      std::vector<std::string> parts;
      gen::split(path.string(), separator, std::back_inserter(parts));

      if (parts.empty()) {
        throw std::runtime_error("Provided path does not contain sub directory: " + path.string());
      }

      return *gen::last(parts);
    }

    template<class T>
    static resource_map<T> load_resources(
        const std::string &resource_path,
        std::function<std::optional<T>(const std::string &)> resource_builder
    ) {
      resource_map<T> resources;

      auto creator{[&](const std::string &resource_category, const auto &resource_file_path) {
        if (fs::is_directory(resource_file_path)) {
          throw std::runtime_error("Resource file path is a directory: " + resource_file_path.path().string());
        }

        auto path{resource_file_path.path().string()};
        auto name{resource_file_path.path().stem().string()};

        if (auto res{resource_builder(path)}; res) {
          resources[resource_category].insert(std::make_pair(name, std::move(*res)));
        } else {
          throw std::runtime_error("Unable to load resource from path: " + path);
        }
      }};

      for (const auto &resource_dir_path : boost::make_iterator_range(fs::directory_iterator(resource_path), {})) {
        if (fs::is_directory(resource_dir_path)) {
          auto resource_category{get_sub_directory(resource_dir_path.path())};

          for (const auto &resource_file_path : boost::make_iterator_range(fs::directory_iterator(resource_dir_path),
                                                                           {})) {
            creator(resource_category, resource_file_path);
          }
        } else {
          creator(ROOT_RESOURCE_CATEGORY, resource_dir_path);
        }
      }

      return resources;
    }
};

using system_ptr = std::shared_ptr<const system>;

template<class ...Ts>
static auto make_system(Ts &&... args) {
  return std::make_shared<const system>(std::forward<Ts>(args)...);
}
}

#endif //TYPEWRITER_GAEM_SYSTEM_HPP
