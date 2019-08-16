#include <SFML/Graphics.hpp>
#include "engine/engine.hpp"

int main() {
  auto game_system{engine::make_system(
      sf::VideoMode(500, 400),
      "./res/fonts",
      "./res/sounds",
      "./res/textures",
      "./res/configs"
  )};

  auto audio_system{engine::make_audio_system(game_system)};

  auto sample_decision_tree{
      std::make_shared<engine::decision_node>(
          std::vector<engine::printable *>{
              new engine::paragraph(
                  game_system,
                  L"Some text that shouldget centered. Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod.",
                  {
                      engine::text_effect{engine::text_effect::kind::COLOR, 0, 3}.with_color(sf::Color::Magenta),
                      engine::text_effect{engine::text_effect::kind::RIGHT, 5, 8},
                      engine::text_effect{engine::text_effect::kind::CENTER, 10, 13},
                      engine::text_effect{engine::text_effect::kind::BOLD, 10, 13},
                      engine::text_effect{engine::text_effect::kind::DELAY, 10, 13}.with_delay(5.0f),
                      engine::text_effect{engine::text_effect::kind::ITALIC, 15, 20},
                      engine::text_effect{engine::text_effect::kind::CENTER, 15, 20}
                  },
                  false
              ),
              new engine::paragraph(
                  game_system,
                  L"Through the darkness of future past, the magician longs to see, one chance out between two worlds, fire walk with me!",
                  {
                      engine::text_effect{engine::text_effect::kind::DELAY, 99, 117}.with_delay(5.0f),
                      engine::text_effect{engine::text_effect::kind::COLOR, 99, 117}.with_color(sf::Color::Red)
                  }
              ),
              new engine::paragraph(
                  game_system,
                  L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.",
                  {}
              )
          },
          std::vector<std::pair<engine::printable *, engine::decision_node * >>{
              {
                  new engine::dialog{
                      game_system,
                      L"Conway",
                      L"You just breathe the road."
                  },
                  new engine::decision_node{
                      {
                          new engine::paragraph{
                              game_system,
                              L"Something",
                              {}
                          },
                          new engine::paragraph{
                              game_system,
                              L"Anything",
                              {}
                          },
                      },
                      {
                          {
                              new engine::dialog{
                                  game_system,
                                  L"Conway",
                                  L"Ah yes, very interesting"
                              },
                              nullptr
                          },
                          {
                              new engine::dialog{
                                  game_system,
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
                      game_system,
                      L"Conway",
                      L"It will only get later."
                  },
                  new engine::decision_node{
                      {
                          new engine::paragraph{
                              game_system,
                              L"Another line",
                              {}
                          },
                          new engine::paragraph{
                              game_system,
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
                      game_system,
                      L"Conway",
                      L"Aw shiet."
                  },
                  nullptr
              }
          }
      )
  };

  auto sample_story{engine::make_story(sample_decision_tree)};

  try {
    engine::game game{
        game_system,
        audio_system,
        sample_story
    };

    game.run();
  } catch (std::exception &e) {
    throw std::runtime_error(engine::gen::str("Game error ", e.what()));
  }

  return 0;
}