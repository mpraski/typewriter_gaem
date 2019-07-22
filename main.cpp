#include <iostream>
#include <SFML/Graphics.hpp>

#include "engine/page.h"

int main() {
  sf::RenderWindow window(sf::VideoMode(200, 200), "FSML works");

  auto page_config{engine::make_page_config(
      "./res/type_right.ttf",
      "./res/sounds",
      32u,
      600.f
  )};

  engine::page page{
      page_config,
      {
          engine::paragraph{
              L"Some few words that will overflow page width quite considerably.",
              {
                  engine::text_effect{engine::text_effect::kind::BOLD, 0, 4},
                  engine::text_effect{engine::text_effect::kind::ITALIC, 5, 8},
                  engine::text_effect{engine::text_effect::kind::DELAY, 10, 14}.with_delay(15.f),
                  engine::text_effect{engine::text_effect::kind::SPACING, 15, 18}.with_letter_spacing(10.f),
                  engine::text_effect{engine::text_effect::kind::COLOR, 20, 24}.with_color(sf::Color::Red)
              }
          },
          engine::paragraph{
              L"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."}
      }
  };

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear();
    window.draw(page);
    window.display();

    if (!page.text_end())
      page.advance();
  }

  return 0;
}