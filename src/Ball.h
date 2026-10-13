#pragma once
#include "Constants.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

class Ball : public sf::CircleShape {
private:
  sf::Vector2f m_speed;
  sf::Vector2f m_startingPos;

public:
  Ball(float radius, float speed, sf::Vector2f startPos)
      : sf::CircleShape(radius), m_speed{speed, speed} {
    this->setPosition(startPos);
  }

  sf::Vector2f &getSpeed() { return m_speed; };

  void flipSpeedX() { m_speed.x *= -1; }
  void flipSpeedY() { m_speed.y *= -1; }

  void increaseBallSpeed() {
    m_speed.x = std::min(std::max(m_speed.x * GlobalVar::speedIncrease,
                                  -GlobalVar::maxBallSpeed),
                         GlobalVar::maxBallSpeed);
    m_speed.y = std::min(std::max(m_speed.y * GlobalVar::speedIncrease,
                                  -GlobalVar::maxBallSpeed),
                         GlobalVar::maxBallSpeed);
    // m_speed.x = std::min(std::max(m_speed.x + GlobalVar::speedIncrease,
    //                               -GlobalVar::maxBallSpeed),
    //                      GlobalVar::maxBallSpeed);
    // m_speed.y = std::min(std::max(m_speed.y + GlobalVar::speedIncrease,
    //                               -GlobalVar::maxBallSpeed),
    //                      GlobalVar::maxBallSpeed);
  }
};
