#include <SFML/Graphics.hpp>
#include "engine/Engine.hpp"

int main() {
  auto sample_decision_tree{
      std::make_shared<engine::decision_node>(
          std::vector<engine::printable *>{
              new engine::paragraph(
                  engine::printable::builder()
                      .with_system(game_system)
                      .with_contents(
                          L"Some text that shouldget centered. Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod."
                      )
                      .with_effects(
                          {
                              engine::TextEffect{engine::TextEffect::Kind::COLOR, 0, 3}.withColor(
                                  sf::Color::Magenta),
                              engine::TextEffect{engine::TextEffect::Kind::RIGHT, 5, 8},
                              engine::TextEffect{engine::TextEffect::Kind::CENTER, 10, 13},
                              engine::TextEffect{engine::TextEffect::Kind::BOLD, 10, 13},
                              engine::TextEffect{engine::TextEffect::Kind::DELAY, 10, 13}.withDelay(5.0f),
                              engine::TextEffect{engine::TextEffect::Kind::ITALIC, 15, 20},
                              engine::TextEffect{engine::TextEffect::Kind::CENTER, 15, 20}
                          }
                      ),
                  false
              ),
              new engine::paragraph(
                  engine::printable::builder()
                      .with_system(game_system)
                      .with_contents(L"Jak masz czas to przejdz sie do speak up i zapytaj o kurs intensywny wakacyjny")
                      .with_font_size(48),
                  false
              ),
              new engine::paragraph(
                  engine::printable::builder()
                      .with_system(game_system)
                      .with_contents(
                          L"Through the darkness of future past, the magician longs to see, one chance out between two worlds, fire walk with me!"
                      )
                      .with_effects(
                          {
                              engine::TextEffect{engine::TextEffect::Kind::DELAY, 99, 117}.withDelay(5.0f),
                              engine::TextEffect{engine::TextEffect::Kind::COLOR, 99, 117}.withColor(sf::Color::Red)
                          }
                      )

              ),
              new engine::paragraph(
                  engine::printable::builder()
                      .with_system(game_system)
                      .with_contents(
                          L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa."
                      )
              )
          },
          std::vector<std::pair<engine::printable *, engine::decision_node * >>{
              {
                  new engine::dialog{
                      engine::printable::builder().with_system(game_system),
                      L"Conway",
                      L"You just breathe the road."
                  },
                  new engine::decision_node{
                      {
                          new engine::paragraph{
                              engine::printable::builder()
                                  .with_system(game_system)
                                  .with_contents(L"Something")
                          },
                          new engine::paragraph{
                              engine::printable::builder()
                                  .with_system(game_system)
                                  .with_contents(L"Anything")
                          },
                      },
                      {
                          {
                              new engine::dialog{
                                  engine::printable::builder().with_system(game_system),
                                  L"Conway",
                                  L"Ah yes, very interesting"
                              },
                              nullptr
                          },
                          {
                              new engine::dialog{
                                  engine::printable::builder().with_system(game_system),
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
                      engine::printable::builder().with_system(game_system),
                      L"Conway",
                      L"It will only get later."
                  },
                  new engine::decision_node{
                      {
                          new engine::paragraph{
                              engine::printable::builder()
                                  .with_system(game_system)
                                  .with_contents(L"Another line")
                          },
                          new engine::paragraph{
                              engine::printable::builder()
                                  .with_system(game_system)
                                  .with_contents(L"Or two")
                          },
                      },
                      {

                      }
                  }
              },
              {
                  new engine::dialog{
                      engine::printable::builder().with_system(game_system),
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
    engine::Game game{
        game_system,
        audio_system,
        sample_story
    };

    game.run();
  } catch (std::exception &e) {
    throw std::runtime_error(engine::gen::str("Main loop error: ", e.what()));
  }

  return 0;
}