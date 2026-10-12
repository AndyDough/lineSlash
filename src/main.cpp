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

namespace GlobalVar {
const sf::Vector2u windowSize{800, 600};
const sf::Vector2f playAreaSize{sf::Vector2f{500, 300}};
const float ballRadius{10.f};
} // namespace GlobalVar

int main() {
  sf::RenderWindow window(sf::VideoMode(GlobalVar::windowSize), "My Window");
  window.setFramerateLimit(60);

  auto center{(static_cast<sf::Vector2f>(window.getSize()) / 2.f) -
              GlobalVar::playAreaSize / 2.f};

  auto ballCenter{center * 2.f};

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

  // sf::RectangleShape s_right{rightBound.size};
  // s_right.setFillColor(sf::Color::Red);
  // s_right.setPosition(rightBound.position);
  // sf::RectangleShape s_left{leftBound.size};
  // s_left.setFillColor(sf::Color::Red);
  // s_left.setPosition(leftBound.position);
  // sf::RectangleShape s_bottom{bottomBound.size};
  // s_bottom.setFillColor(sf::Color::Red);
  // s_bottom.setPosition(bottomBound.position);
  // sf::RectangleShape s_top{topBound.size};
  // s_top.setFillColor(sf::Color::Red);
  // s_top.setPosition(topBound.position);

  // Initialize Ball
  sf::CircleShape ball(GlobalVar::ballRadius);
  ball.setFillColor(sf::Color::White);
  sf::Vector2f speedOfBall{5.f, 5.f};
  ball.setPosition(ballCenter);

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

    ball.move(speedOfBall);
    if (topBound.contains(ball.getPosition()) ||
        bottomBound.contains(ball.getPosition())) {
      speedOfBall.y *= -1;
      std::cout << "hit top/bot\n";
    }
    if (rightBound.contains(ball.getPosition()) ||
        leftBound.contains(ball.getPosition())) {
      speedOfBall.x *= -1;
      std::cout << "hit right/left\n";
    }

    window.draw(playArea);
    window.draw(ball);
    // window.draw(s_top);
    // window.draw(s_bottom);
    // window.draw(s_left);
    // window.draw(s_right);

    // // sf::CircleShape shape(50.f);
    // // shape.setFillColor(sf::Color(100, 250, 50));
    // sf::RectangleShape line({150.f, 5.f});
    // line.setOrigin(line.getSize() / 2.f);
    // line.setPosition({400.f, 300.f});
    // line.rotate(sf::degrees(degrees));
    // window.draw(line);
    //
    // ++degrees;

    window.display();
  }

  return 0;
}
