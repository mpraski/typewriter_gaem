#include <iostream>
#include <SFML/Graphics.hpp>

#include "engine/engine.h"

int main() {
  auto resources{engine::make_resources(
      sf::VideoMode(200, 200),
      "./res/fonts",
      "./res/sounds",
      24u,
      600.f
  )};

  engine::page page{
      resources,
      {
          new engine::paragraph(
              resources,
              L"Through the darkness of future past, the magician longs to see, one chance out between two worlds, fire walk with me!",
              {
                  engine::text_effect{engine::text_effect::kind::BOLD, 0, 7},
                  engine::text_effect{engine::text_effect::kind::DELAY, 100, 118}.with_delay(5.0f),
                  engine::text_effect{engine::text_effect::kind::COLOR, 100, 118}.with_color(sf::Color::Red)
              }
          ),
          new engine::dialog{
              resources,
              L"Conway",
              L"You just breathe the road."
          },
          new engine::dialog{
              resources,
              L"Conway",
              L"It will only get later."
          },
          new engine::dialog{
              resources,
              L"Conway",
              L"Ah shiet."
          },
      }
  };

  resources->display([&](auto &window) {
    window.clear();
    window.draw(page);
    window.display();

    if (!page.text_end()) {
      page.advance();
    } else {
      page.input();
    }
  });

  return 0;
}