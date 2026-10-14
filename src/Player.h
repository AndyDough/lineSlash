#pragma once
#include "Constants.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>

class Player : public sf::RectangleShape {
private:
  int m_score{0};
  int m_lives{GlobalVar::lives};
  sf::Vector2f m_endOfLine;

public:
  Player(const sf::Vector2f &startngPos)
      : sf::RectangleShape(GlobalVar::lineSize) {
    m_endOfLine = {this->getOrigin().x + GlobalVar::lineSize.x,
                   this->getOrigin().y + GlobalVar::lineSize.y};
    this->setPosition(startngPos);
  }

  sf::Vector2f getTip() const { return m_endOfLine; }

  void pivot() {
    m_endOfLine = this->getOrigin();
    this->setOrigin(m_endOfLine);
  }

  void rotate(sf::Angle angle) {
    this->sf::Transformable::rotate(angle);
    m_endOfLine = {this->getOrigin().x +
                       GlobalVar::lineSize.x * std::cos(angle.asRadians()),
                   this->getOrigin().y +
                       GlobalVar::lineSize.y * std::sin(angle.asRadians())};
  }
};

// x = start_x + length * cos(theta), y = start_y + length * sin(theta)
// SlateyDev's approach

//
// sf::RectangleShape line({150.f, 5.f});
// line.setOrigin(line.getSize() / 2.f);
// line.setPosition({400.f, 300.f});
// line.rotate(sf::degrees(degrees));
//
// if (line.getGlobalBounds().contains(ball.getPosition())) {
//   std::cout << "ball hit line\n";
// }
