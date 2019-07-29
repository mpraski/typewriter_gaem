//
// Created by marcin on 7/20/19.
//

#include "text_effect.h"

engine::text_effect::text_effect(
    enum kind kind,
    size_t begin,
    size_t end,
    float delay_factor,
    float letter_spacing_factor,
    sf::Color color
) : kind{kind},
    begin{begin},
    end{end},
    delay_factor{delay_factor},
    letter_spacing_factor{letter_spacing_factor},
    color{color} {

}

engine::text_effect &engine::text_effect::with_delay(float df) {
  delay_factor = df;
  return *this;
}

engine::text_effect &engine::text_effect::with_letter_spacing(float ls) {
  letter_spacing_factor = ls;
  return *this;
}

engine::text_effect &engine::text_effect::with_color(sf::Color c) {
  color = c;
  return *this;
}