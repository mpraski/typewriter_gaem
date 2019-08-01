#include <iostream>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "engine/engine.h"

int main() {
  auto resources{engine::make_resources(
      sf::VideoMode(600, 600),
      "./res/fonts",
      "./res/sounds",
      "./res/textures"
  )};

  engine::page page{
      resources,
      {
          new engine::paragraph(
              boost::uuids::random_generator()(),
              resources,
              L"Through the darkness of future past, the magician longs to see, one chance out between two worlds, fire walk with me!",
              {
                  engine::text_effect{engine::text_effect::kind::TEXTURE, 0, 7}.with_texture("sample"),
                  engine::text_effect{engine::text_effect::kind::DELAY, 100, 118}.with_delay(5.0f),
                  engine::text_effect{engine::text_effect::kind::COLOR, 100, 118}.with_color(sf::Color::Red)
              }
          ),
          new engine::paragraph(
              boost::uuids::random_generator()(),
              resources,
              L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
              {}
          ),
          new engine::dialog{
              boost::uuids::random_generator()(),
              resources,
              L"Conway",
              L"You just breathe the road."
          },
          new engine::dialog{
              boost::uuids::random_generator()(),
              resources,
              L"Conway",
              L"It will only get later."
          },
          new engine::dialog{
              boost::uuids::random_generator()(),
              resources,
              L"Conway",
              L"Aw shiet."
          },
      }
  };

  page.move(20.f, 20.f);

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