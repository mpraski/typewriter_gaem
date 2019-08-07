#include <SFML/Graphics.hpp>
#include "engine/engine.h"

int main() {
  auto resources{engine::make_resources(
      sf::VideoMode(460, 800),
      "./res/fonts",
      "./res/sounds",
      "./res/textures"
  )};

  auto sample_decision_tree{
      std::make_shared<engine::decision_node>(
          std::vector<engine::printable *>{
              new engine::paragraph(
                  resources,
                  L"Some text that shouldget centered. Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod.",
                  {
                      engine::text_effect{engine::text_effect::kind::COLOR, 0, 3}.with_color(sf::Color::Red),
                      engine::text_effect{engine::text_effect::kind::RIGHT, 5, 8},
                      engine::text_effect{engine::text_effect::kind::CENTER, 10, 13},
                      engine::text_effect{engine::text_effect::kind::BOLD, 10, 13},
                      engine::text_effect{engine::text_effect::kind::DELAY, 10, 13}.with_delay(5.0f),
                      engine::text_effect{engine::text_effect::kind::UNDERLINE, 15, 20},
                      engine::text_effect{engine::text_effect::kind::CENTER, 15, 20}
                  },
                  false
              ),
              new engine::paragraph(
                  resources,
                  L"Through the darkness of future past, the magician longs to see, one chance out between two worlds, fire walk with me!",
                  {
                      engine::text_effect{engine::text_effect::kind::DELAY, 99, 117}.with_delay(5.0f),
                      engine::text_effect{engine::text_effect::kind::COLOR, 99, 117}.with_color(sf::Color::Red)
                  }
              ),
              new engine::paragraph(
                  resources,
                  L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.",
                  {}
              )
          },
          std::vector<std::pair<engine::printable *, engine::decision_node * >>{
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
                          {
                              new engine::dialog{
                                  resources,
                                  L"Conway",
                                  L"Ah yes, very interesting"
                              },
                              nullptr
                          },
                          {
                              new engine::dialog{
                                  resources,
                                  L"Conway",
                                  L"Is this a KR0 ripoff?"
                              },
                              nullptr
                          },
                      }
                  }
              },
              {
                  new engine::dialog{
                      resources,
                      L"Conway",
                      L"It will only get later."
                  },
                  new engine::decision_node{
                      {
                          new engine::paragraph{
                              resources,
                              L"Another line",
                              {}
                          },
                          new engine::paragraph{
                              resources,
                              L"O two",
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