#pragma once
#include "Constants.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>

class Ball : public sf::CircleShape {
private:
  const sf::Vector2f m_startingSpeed;
  sf::Vector2f m_speed;
  const sf::Vector2f m_startingPos;

public:
  Ball(float radius, float speed, sf::Vector2f startPos)
      : sf::CircleShape(radius), m_startingSpeed{speed, speed},
        m_startingPos{startPos}, m_speed{m_startingSpeed} {
    this->setPosition(m_startingPos);
  }

  sf::Vector2f &getSpeed() { return m_speed; }

  void reset() {
    m_speed = m_startingSpeed;
    this->setPosition(m_startingPos);
  }

  void flipSpeedX() { m_speed.x *= -1; }
  void flipSpeedY() { m_speed.y *= -1; }

  void increaseBallSpeed() {
    m_speed.x = std::clamp(m_speed.x * GlobalVar::speedIncrease,
                           -GlobalVar::maxBallSpeed, GlobalVar::maxBallSpeed);
    m_speed.y = std::clamp(m_speed.y * GlobalVar::speedIncrease,
                           -GlobalVar::maxBallSpeed, GlobalVar::maxBallSpeed);
  }
};
