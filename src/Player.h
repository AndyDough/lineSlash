#pragma once
#include "Constants.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <iostream>
#include <utility>

class Player : public sf::RectangleShape {
private:
  int m_score{0};
  int m_lives{GlobalVar::lives};
  sf::Vector2f m_endOfLine;
  float degree;
  bool m_turnClockwise{true};

public:
  Player(const sf::Vector2f &startingPos)
      : sf::RectangleShape(GlobalVar::lineSize) {
    this->setPosition(startingPos);
    m_endOfLine = {this->getPosition().x + GlobalVar::lineSize.x,
                   this->getPosition().y};
  }

  void swapRotation() { m_turnClockwise = !m_turnClockwise; }

  sf::Vector2f &getTipHitBox() { return m_endOfLine; }

  void pivot() {
    
    float degree = getRotation().asDegrees();

    if(degree >= 180) degree -= 180.f;
    else  degree += 180.f;
    setRotation(sf::degrees(degree));

    std::cout 
      << "Pivoted on: current_degrees: "<< getRotation().asDegrees() 
      <<", degrees: " << degree
      << ", m_endofline: " << m_endOfLine.x << ", " << m_endOfLine.y << '\n';

    this->setPosition(m_endOfLine);

    swapRotation();

    m_endOfLine = {this->getPosition().x +
                       GlobalVar::lineSize.x *
                           std::cos(getRotation().asRadians()),
                   this->getPosition().y +
                       GlobalVar::lineSize.x *
                           std::sin(getRotation().asRadians())};

    // auto temp{this->getPosition()};
    // this->setPosition(m_endOfLine);
    // m_endOfLine = temp;
  }

  void rotate() {
    if (m_turnClockwise) {
      this->sf::Transformable::rotate(sf::degrees(1.f));
    } else {
      this->sf::Transformable::rotate(sf::degrees(-1.f));
    }
    //std::cout << degree << " " << this->getRotation().asDegrees() << '\n';

    m_endOfLine = {this->getPosition().x +
                       GlobalVar::lineSize.x *
                           std::cos(getRotation().asRadians()),
                   this->getPosition().y +
                       GlobalVar::lineSize.x *
                           std::sin(getRotation().asRadians())};
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
