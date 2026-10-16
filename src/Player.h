#pragma once
#include "Constants.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <iostream>

class Player : public sf::RectangleShape {
private:
  int m_score{0};
  int m_lives{GlobalVar::lives};
  sf::Vector2f m_endOfLine;
  float m_degree;
  bool m_turnClockwise{true};

public:
  Player(const sf::Vector2f &startingPos, float startingDegrees)
      : sf::RectangleShape(GlobalVar::lineSize), m_degree{startingDegrees} {
    this->setPosition(startingPos);
    m_endOfLine = {this->getPosition().x + GlobalVar::lineSize.x,
                   this->getPosition().y};
  }

  float getDegree() { return m_degree; }

  void swapRotation() { m_turnClockwise = !m_turnClockwise; }

  sf::Vector2f &getTipHitBox() { return m_endOfLine; }

  void pivot() {
    auto temp{this->getPosition()};
    this->setPosition(m_endOfLine); // This is probably the problem
    m_endOfLine = temp;
  }

  void rotate() {
    if (m_turnClockwise) {
      this->sf::Transformable::rotate(sf::degrees(1.f));
    } else {
      this->sf::Transformable::rotate(sf::degrees(-1.f));
    }
    
    m_endOfLine = {this->getPosition().x +
                       GlobalVar::lineSize.x *
                           std::cos(sf::radians(m_degree).asRadians()),
                   this->getPosition().y +
                       GlobalVar::lineSize.x *
                           std::sin(sf::radians(m_degree).asRadians())};
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
