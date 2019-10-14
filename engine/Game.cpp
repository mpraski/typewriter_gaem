//
// Created by marcin on 8/16/19.
//

#include "Game.hpp"

const sf::Time engine::Game::mTimePerFrame = sf::seconds(1.f / 60.f);

engine::Game::Game(std::unique_ptr<Entity> rootEntity)
    : window{System::instance().getWindow()},
      mWorldView{window.getDefaultView()},
      mSceneGraph{std::move(rootEntity)},
      mSceneLayers{},
      mHoldsMouse{} {

}

void engine::Game::run() {
  sf::Clock clock;
  sf::Time since_last_update = sf::Time::Zero;

  while (window.isOpen()) {
    sf::Time elapsed = clock.restart();
    since_last_update += elapsed;

    while (since_last_update > mTimePerFrame) {
      since_last_update -= mTimePerFrame;
      processEvents();
      update(mTimePerFrame);
    }

    draw();
  }
}

void engine::Game::processEvents() {
  sf::Event event;
  while (window.pollEvent(event)) {
    switch (event.type) {
      case sf::Event::Closed:
        window.close();
        break;
      case sf::Event::MouseButtonPressed:
        mHoldsMouse = true;
        break;
      case sf::Event::MouseButtonReleased:
        if (mHoldsMouse) {
          System::instance().setMouseClick(sf::Vector2f{sf::Mouse::getPosition(window)});
          mHoldsMouse = false;
        }
        break;
      default:
        break;
    }
  }
}

void engine::Game::update(sf::Time dt) {
  mSceneGraph->update(dt);
}

void engine::Game::draw() {
  window.clear();
  window.setView(mWorldView);
  window.draw(*mSceneGraph);

  window.display();
}
