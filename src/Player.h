#pragma once
#include "Constants.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <iostream>

extern float getDeltaTime();

class Player : public sf::RectangleShape {
private:
  int m_score{0};
  int m_highScore{0};
  int m_lives{GlobalVar::lives};
  sf::Vector2f m_endOfLine;
  sf::Vector2f m_startingPos;
  bool m_turnClockwise{true};
  int m_IFrames{20};

public:
  Player(const sf::Vector2f &startingPos)
      : sf::RectangleShape(GlobalVar::lineSize), m_startingPos{startingPos} {
    this->setPosition(startingPos);
    m_endOfLine = {this->getPosition().x + GlobalVar::lineSize.x,
                   this->getPosition().y};
  }

  void reset() {
    m_lives = GlobalVar::lives;
    m_score = 0;
    m_turnClockwise = true;
    this->setPosition(m_startingPos);
    this->setRotation(sf::degrees(0));
    m_endOfLine = {this->getPosition().x + GlobalVar::lineSize.x,
                   this->getPosition().y};
  }

  void swapRotation() { m_turnClockwise = !m_turnClockwise; }

  int getScore() const { return m_score; }
  void updateScore() { ++m_score; }

  int getLives() const { return m_lives; }
  void minusLife() { --m_lives; }
  bool isAlive() const { return m_lives > 0; }

  const sf::Vector2f &getTipHitBox() { return m_endOfLine; }

  void procIFrames() { m_IFrames = 20; }

  bool checkIniviciblity() {
    if (m_IFrames > 0) {
      --m_IFrames;
      return true;
      std::cout << "I'm invincible\n";
    }
    return false;
  }

  void pivot() {

    float degree = getRotation().asDegrees();

    if (degree >= 180)
      degree -= 180.f;
    else
      degree += 180.f;
    setRotation(sf::degrees(degree));

    std::cout << "Pivoted on: current_degrees: " << getRotation().asDegrees()
              << ", degrees: " << degree << ", m_endofline: " << m_endOfLine.x
              << ", " << m_endOfLine.y << '\n';

    this->setPosition(m_endOfLine);

    swapRotation();

    m_endOfLine = {
        this->getPosition().x +
            GlobalVar::lineSize.x * std::cos(getRotation().asRadians()),
        this->getPosition().y +
            GlobalVar::lineSize.x * std::sin(getRotation().asRadians())};

    // auto temp{this->getPosition()};
    // this->setPosition(m_endOfLine);
    // m_endOfLine = temp;
  }

  void rotate() {
    if (m_turnClockwise) {
      this->sf::Transformable::rotate(sf::degrees(GlobalVar::angleIncrement * getDeltaTime()));
    } else {
      this->sf::Transformable::rotate(sf::degrees(-GlobalVar::angleIncrement * getDeltaTime()));
    }
    // std::cout << degree << " " << this->getRotation().asDegrees() << '\n';

    m_endOfLine = {
        this->getPosition().x +
            GlobalVar::lineSize.x * std::cos(getRotation().asRadians()),
        this->getPosition().y +
            GlobalVar::lineSize.x * std::sin(getRotation().asRadians())};
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
