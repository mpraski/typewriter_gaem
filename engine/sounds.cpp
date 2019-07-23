//
// Created by marcin on 7/23/19.
//

#include "sounds.h"

engine::sounds::sounds(const resources_ptr &rptr) : game_state{rptr}, sound_cache{} {
  typewriter_clicks = &resources->get_sounds("typewriter_clicks");

  if (typewriter_clicks->empty()) {
    throw std::runtime_error("No typewriter click sounds found");
  }
}

void engine::sounds::play_typewriter_click() const {
  auto rand_buf{random(std::begin(*typewriter_clicks),
                       std::end(*typewriter_clicks))};

  if (auto it = sound_cache.find(rand_buf->first); it != sound_cache.end()) {
    it->second.play();
  } else {
    sound_cache.emplace(rand_buf->first, rand_buf->second);
    sound_cache[rand_buf->first].play();
  }
}

void engine::sounds::play_typewriter_space() const {
  constexpr auto key{"typewriter_spacebar"};

  if (auto it = sound_cache.find(key); it != sound_cache.end()) {
    it->second.play();
  } else {
    const auto &buf{resources->get_sounds(resources::ROOT_RESOURCE_CATEGORY).at(key)};
    sound_cache.emplace(key, buf);
    sound_cache[key].play();
  }
}