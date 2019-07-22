//
// Created by marcin on 7/21/19.
//

#ifndef TYPEWRITER_GAEM_PAGE_CONFIG_H
#define TYPEWRITER_GAEM_PAGE_CONFIG_H

#include <memory>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace fs = std::filesystem;

namespace engine {
class page_config {
    const constexpr static auto ROOT_RESOURCE_CATEGORY = "<root>";
public:
    explicit page_config(
        const std::string &font_path,
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

    ~page_config();

    page_config(const page_config &p) = delete;

    page_config(page_config &&p) = delete;

    page_config &operator=(const page_config &p) = delete;

    page_config &operator=(page_config &&p) = delete;

    inline const auto &sounds_for(const std::string &sound_category = ROOT_RESOURCE_CATEGORY) const {
      return sounds.at(sound_category);
    }

    sf::Font *font;
    unsigned font_size;
    float page_width;
    float page_height;
    float margin_vertical;
    float margin_horizontal;
    float letter_spacing_factor;
    float line_spacing_factor;
    unsigned typing_delay;

private:
    std::unordered_map<std::string, std::vector<sf::SoundBuffer>> sounds;

    std::string get_sub_directory(const std::filesystem::path &path) const;

    template<class T>
    std::unordered_map<std::string, std::vector<T>> load_resources(
        const std::string &resource_path,
        std::function<std::optional<T>(const std::string &)> resource_builder
    ) const {
      std::unordered_map<std::string, std::vector<T>> resources;

      auto creator{[&](const auto &resource_category, const auto &resource_file_path) {
        auto path{resource_file_path.path().string()};
        auto name{resource_file_path.path().stem().string()};

        if (auto res{resource_builder(path)}; res) {
          resources[resource_category].emplace_back(std::move(*res));
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

using page_config_ptr = std::shared_ptr<const page_config>;

template<class ...Ts>
static auto make_page_config(Ts &&... args) {
  return std::make_shared<const page_config>(std::forward<Ts>(args)...);
}
}

#endif //TYPEWRITER_GAEM_PAGE_CONFIG_H
