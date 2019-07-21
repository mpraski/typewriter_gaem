#include <iostream>
#include <SFML/Graphics.hpp>

#include "engine/page.h"

int main() {
  sf::RenderWindow window(sf::VideoMode(200, 200), "FSML works");

  auto page_config{engine::make_page_config(
      "./res/jmh_typewriter_bold.ttf",
      24u,
      500.f
  )};

  engine::page page{
      page_config,
      {
          engine::paragraph{L"Some few words that will overflow page width quite          considerably."},
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