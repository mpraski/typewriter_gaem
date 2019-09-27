//
// Created by marcin on 9/24/19.
//

#include "Identifiable.hpp"

engine::Identifiable::Identifiable()
    : mUID{gen::next_uid()} {}

sf::Uint64 engine::Identifiable::getUID() const noexcept {
  return mUID;
}