#include <SFML/Graphics.hpp>
#include "engine/engine.h"

int main() {
  auto resources{engine::make_resources(
      sf::VideoMode(600, 600),
      "./res/fonts",
      "./res/sounds",
      "./res/textures"
  )};

  auto sample_decision_tree{
      std::make_shared<engine::decision_node>(
          std::vector<engine::printable *>{
              new engine::paragraph(
                  resources,
                  L"Through the darkness of future past, the magician longs to see, one chance out between two worlds, fire walk with me!",
                  {
                      engine::text_effect{engine::text_effect::kind::TEXTURE, 0, 7}.with_texture("sample"),
                      engine::text_effect{engine::text_effect::kind::DELAY, 100, 118}.with_delay(5.0f),
                      engine::text_effect{engine::text_effect::kind::COLOR, 100, 118}.with_color(sf::Color::Red)
                  }
              ),
              new engine::paragraph(
                  resources,
                  L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
                  {}
              )
          },
          std::vector<std::pair<engine::printable *, engine::decision_node *>>{
              {
                  new engine::dialog{
                      resources,
                      L"Conway",
                      L"You just breathe the road."
                  },
                  new engine::decision_node{
                      {
                          new engine::paragraph{
                              resources,
                              L"Something",
                              {}
                          },
                          new engine::paragraph{
                              resources,
                              L"Anything",
                              {}
                          },
                      },
                      {

                      }
                  }
              },
              {
                  new engine::dialog{
                      resources,
                      L"Conway",
                      L"It will only get later."
                  },
                  nullptr
              },
              {
                  new engine::dialog{
                      resources,
                      L"Conway",
                      L"Aw shiet."
                  },
                  nullptr
              }
          }
      )
  };

  auto sample_story{std::make_shared<engine::story>(sample_decision_tree)};

  engine::page page{
      resources,
      sample_story
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