//
// Created by marcin on 8/14/19.
//

#include "game_object.hpp"

engine::game_object::game_object(const system_ptr &rptr)
    : game_state{rptr},
      id{boost::uuids::random_generator()()} {

}

engine::id_t engine::game_object::get_id() const {
  return id;
}