//
// Created by marcin on 7/23/19.
//

#include "audio_system.hpp"

engine::audio_system::audio_system(const resources_ptr &rptr)
    : game_state{rptr},
      sound_cache{} {
  typewriter_clicks = &resources->get_sounds("typewriter_clicks");

  if (typewriter_clicks->empty()) {
    throw std::runtime_error("No typewriter click audio_system found");
  }
}

void engine::audio_system::play_typewriter_click() const {
  auto rand_buf{random(std::begin(*typewriter_clicks),
                       std::end(*typewriter_clicks))};

  play_with_cache(rand_buf->first, rand_buf->second);
}

void engine::audio_system::play_typewriter_space() const {
  constexpr auto key{"typewriter_spacebar"};
  const auto &buf{resources->get_sounds(resources::ROOT_RESOURCE_CATEGORY).at(key)};

  play_with_cache(key, buf);
}

void engine::audio_system::play_with_cache(const std::string &key, const sf::SoundBuffer &buf) const {
  if (auto it{sound_cache.find(key)}; it != sound_cache.end()) {
    it->second.play();
  } else if (auto[itt, ok]{sound_cache.emplace(key, buf)}; ok) {
    itt->second.play();
  }
}