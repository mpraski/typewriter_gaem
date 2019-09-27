//
// Created by marcin on 9/27/19.
//

#include "Action.hpp"

engine::Action::Action(ActionKind kind, sf::Uint64 uid)
    : kind{kind},
      uid{uid} {

}