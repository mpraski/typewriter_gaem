//
// Created by marcin on 8/14/19.
//

#ifndef TYPEWRITER_GAEM_GAME_OBJECT_HPP
#define TYPEWRITER_GAEM_GAME_OBJECT_HPP

#include "system.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/functional/hash.hpp>

namespace engine {
using id_t = boost::uuids::uuid;

class game_object {
public:
    explicit game_object(const system_ptr &rptr);

    id_t get_id() const;

    system_ptr get_system() const;

    inline bool operator==(const game_object &go) const noexcept {
      return id == go.id;
    }

protected:
    id_t id;
    system_ptr system;
};

inline bool operator==(const game_object &go, id_t id) {
  return go.get_id() == id;
}

inline bool operator==(id_t id, const game_object &go) {
  return go.get_id() == id;
}
}

namespace std {
template<>
struct hash<engine::game_object> {
    size_t operator()(const engine::game_object &go) const {
      return boost::hash<engine::id_t>()(go.get_id());
    }
};

template<>
struct equal_to<engine::game_object> {
    bool operator()(const engine::game_object &go1, const engine::game_object &go2) const {
      return go1 == go2;
    }
};
}
#endif //TYPEWRITER_GAEM_GAME_OBJECT_HPP
