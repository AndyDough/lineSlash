// Starting to learn game dev,
// 1. Open a game window (Done)
// 2. Goals draw a line that spins an a circle
//
// Contemplating using SFML or raylib
// SFML to learn c++ or raylib to just get started
// Proabably SFML honestly since I already have it installed
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <optional>

int main() {
  sf::RenderWindow window(sf::VideoMode({800, 600}), "My Window");
  window.setFramerateLimit(60);

  auto degrees{0};
  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }
    window.clear(sf::Color::Black);
    // sf::CircleShape shape(50.f);
    // shape.setFillColor(sf::Color(100, 250, 50));
    sf::RectangleShape line({150.f, 5.f});
    line.setOrigin(line.getSize() / 2.f);
    line.setPosition({400.f, 300.f});
    line.rotate(sf::degrees(degrees));
    window.draw(line);

    ++degrees;

    window.display();
  }

  return 0;
}
