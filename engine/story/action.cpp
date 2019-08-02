//
// Created by marcin on 7/27/19.
//

#include "action.h"

engine::action::action(enum kind k, boost::uuids::uuid pid)
    : kind{k},
      pid{pid} {

}