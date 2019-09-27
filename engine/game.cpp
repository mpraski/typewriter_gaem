//
// Created by marcin on 8/16/19.
//

#include "game.hpp"

const sf::Time engine::game::time_per_frame = sf::seconds(1.f / 60.f);

engine::game::game(
    const SystemPtr &rptr,
    const audio_system_ptr &aptr,
    const story_ptr &sptr
)
    : GameObject{rptr},
      window{rptr->get_window()},
      world_view{window.getDefaultView()},
      scene_graph{},
      scene_layers{},
      holds_mouse{} {
  auto pc{make_page_container(rptr, aptr, sptr)};
  pc->move(20.f, 20.f);
  scene_graph.addChild(std::move(pc));
}

void engine::game::run() {
  sf::Clock clock;
  sf::Time since_last_update = sf::Time::Zero;

  while (window.isOpen()) {
    sf::Time elapsed = clock.restart();
    since_last_update += elapsed;

    while (since_last_update > time_per_frame) {
      since_last_update -= time_per_frame;
      process_events();
      update(time_per_frame);
    }

    draw();
  }
}

void engine::game::process_events() {
  sf::Event event;
  while (window.pollEvent(event)) {
    switch (event.type) {
      case sf::Event::Closed:
        window.close();
        break;
      case sf::Event::MouseButtonPressed:
        holds_mouse = true;
        break;
      case sf::Event::MouseButtonReleased:
        if (holds_mouse) {
          mSystem->set_mouse_click(sf::Vector2f{sf::Mouse::getPosition(window)});
          holds_mouse = false;
        }
        break;
      default:
        break;
    }
  }
}

void engine::game::update(sf::Time dt) {
  scene_graph.update(dt);
}

void engine::game::draw() {
  window.clear();
  window.setView(world_view);
  window.draw(scene_graph);

  window.display();
}