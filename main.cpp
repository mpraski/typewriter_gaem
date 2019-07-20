#include <iostream>
#include <SFML/Graphics.hpp>

#include "engine/page.h"

int main() {
  sf::RenderWindow window(sf::VideoMode(200, 200), "FSML works");
  sf::CircleShape circle(100.0f);
  sf::Text text;
  sf::Font font;
  if (!font.loadFromFile("./res/arial.ttf")) {
    std::cout << "fucking fonts..." << std::endl;
    return 0;
  }

  text.setFont(font);
  text.setString("Wut wut");
  text.setCharacterSize(128);
  text.setFillColor(sf::Color::White);
  text.setStyle(sf::Text::Bold);
  circle.setFillColor(sf::Color::Green);

  while(window.isOpen()) {
    sf::Event event;
    while(window.pollEvent(event)) {
      if(event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear();
    window.draw(circle);
    window.draw(text);
    window.display();
  }

  return 0;
}