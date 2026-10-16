#include "Ball.h"
#include "Constants.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <iostream>
#include <optional>

int main() {
  sf::RenderWindow window(sf::VideoMode(GlobalVar::windowSize), "My Window");
  window.setFramerateLimit(60);

  auto center{(static_cast<sf::Vector2f>(window.getSize()) / 2.f) -
              GlobalVar::playAreaSize / 2.f};

  // Initialize play area
  sf::RectangleShape playArea;
  playArea.setSize(GlobalVar::playAreaSize);
  playArea.setFillColor(sf::Color::Black);
  playArea.setOutlineColor(sf::Color::White);
  playArea.setOutlineThickness(10.0);
  playArea.setPosition(center);

  // Walls with that can be hit lol
  sf::Vector2f playAreaPos{playArea.getPosition()};
  sf::Vector2f playAreaSize{playArea.getSize()};

  sf::FloatRect topBound{playAreaPos, sf::Vector2f{playAreaSize.x, 10.f}};
  sf::FloatRect bottomBound{
      sf::Vector2f{playAreaPos.x, playAreaPos.y + playAreaSize.y},
      sf::Vector2f{playAreaSize.x + 100.f, 10.f}};
  sf::FloatRect leftBound{playAreaPos, sf::Vector2f{10.f, playAreaSize.y}};
  sf::FloatRect rightBound{
      sf::Vector2f{playAreaPos.x + playAreaSize.x, playAreaPos.y},
      sf::Vector2f{10.f, playAreaSize.y + 100.f}};

  sf::RectangleShape s_right{rightBound.size};
  s_right.setFillColor(sf::Color::Red);
  s_right.setPosition(rightBound.position);
  sf::RectangleShape s_left{leftBound.size};
  s_left.setFillColor(sf::Color::Red);
  s_left.setPosition(leftBound.position);
  sf::RectangleShape s_bottom{bottomBound.size};
  s_bottom.setFillColor(sf::Color::Red);
  s_bottom.setPosition(bottomBound.position);
  sf::RectangleShape s_top{topBound.size};
  s_top.setFillColor(sf::Color::Red);
  s_top.setPosition(topBound.position);

  Ball ball{GlobalVar::ballRadius, GlobalVar::startingBallSpeed,
            (center * 2.f)};

  Player line({playAreaPos.x + 20.f, playAreaPos.y * 2.f}, 45.f);

  sf::Clock clock;

  // Main loop
  while (window.isOpen()) {

    // check for closing window event
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }
    window.clear(sf::Color::Black);
    // ready to be updated after

    float delta = clock.restart().asSeconds();

    ball.move(ball.getSpeed());
    if (topBound.contains(ball.getPosition()) ||
        bottomBound.contains(ball.getPosition())) {
      ball.flipSpeedY();
      ball.increaseBallSpeed();
    }
    if (rightBound.contains(ball.getPosition()) ||
        leftBound.contains(ball.getPosition())) {
      ball.flipSpeedX();
      ball.increaseBallSpeed();
    }

    window.draw(playArea);
    window.draw(ball);
    window.draw(s_top);
    window.draw(s_bottom);
    window.draw(s_left);
    window.draw(s_right);

    if (topBound.contains(line.getTipHitBox()) ||
        bottomBound.contains(line.getTipHitBox())) {
      line.swapRotation();
      //std::cout << "tip hit wall\n";
    }
    if (rightBound.findIntersection(line.getGlobalBounds()) ||
        leftBound.findIntersection(line.getGlobalBounds())) {
      line.swapRotation();
      //std::cout << "tip hit wall\n";
    }

    if(line.getGlobalBounds().findIntersection(ball.getGlobalBounds())) {
      //std::cout << "ball hit line\n";
      ball.flipSpeedX();
    }
    line.rotate();
    window.draw(line);

    window.display();
  }

  return 0;
}
