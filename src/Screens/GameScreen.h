#pragma once
#include "../Ball.h"
#include "../Constants.h"
#include "../Player.h"
#include "BaseScreen.h"
#include "GameState.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
class GameScreen : public BaseScreen {
private:
  sf::RenderWindow &m_window;
  sf::Vector2f m_screenLeftCorner;
  sf::Vector2f m_center;
  sf::RectangleShape m_playArea;
  sf::FloatRect m_topBound;
  sf::FloatRect m_bottomBound;
  sf::FloatRect m_rightBound;
  sf::FloatRect m_leftBound;
  Ball m_ball;
  Player m_player1;
  sf::CircleShape m_lineTipHitbox;
  GameState &m_gameState;
  sf::Text m_scoreText;
  sf::Text m_livesText;
  sf::Text m_highScoreText;

public:
  GameScreen(sf::RenderWindow &window, GameState &state, sf::Font &font)
      : m_window{window}, m_scoreText{font}, m_highScoreText{font},
        m_screenLeftCorner{(static_cast<sf::Vector2f>(window.getSize()) / 2.f) -
                           GlobalVar::playAreaSize / 2.f},
        m_ball{GlobalVar::ballRadius, GlobalVar::startingBallSpeed,
               (m_screenLeftCorner * 2.f)},
        m_gameState{state},
        m_player1({m_screenLeftCorner.x + 20.f, m_screenLeftCorner.y * 2.f}),
        m_lineTipHitbox(5.f),
        // since we know that playarea position is m_screenLeftCorner and size
        // is GlobalVar::playAreaSize, we can just hardcode the bounds
        m_livesText(font),
        m_topBound{{m_screenLeftCorner.x - 10.f, m_screenLeftCorner.y - 10.f},
                   sf::Vector2f{GlobalVar::playAreaSize.x + 20.f, 10.f}},
        m_bottomBound{{m_screenLeftCorner.x,
                       m_screenLeftCorner.y + GlobalVar::playAreaSize.y},
                      sf::Vector2f{GlobalVar::playAreaSize.x + 10.f, 10.f}},
        m_leftBound{{m_screenLeftCorner.x - 10.f, m_screenLeftCorner.y},
                    sf::Vector2f{10.f, GlobalVar::playAreaSize.y + 10.f}},
        m_rightBound{{m_screenLeftCorner.x + GlobalVar::playAreaSize.x,
                      m_screenLeftCorner.y},
                     sf::Vector2f{10.f, GlobalVar::playAreaSize.y + 10.f}} {
    m_lineTipHitbox.setFillColor(sf::Color::Red);

    m_playArea.setSize(GlobalVar::playAreaSize);
    m_playArea.setFillColor(sf::Color::Black);
    m_playArea.setOutlineColor(sf::Color::White);
    m_playArea.setOutlineThickness(10.0);
    m_playArea.setPosition(m_screenLeftCorner);
  }

  void reset() {
    m_player1.reset();
    m_ball.reset();
  }

  int getScore() const { return m_player1.getScore(); }

  void update() override {
    m_livesText.setString(std::format("lives: {}", m_player1.getLives()));
    m_livesText.setPosition(
        sf::Vector2f{m_screenLeftCorner.x, m_topBound.position.y - 40.f});
    m_lineTipHitbox.setPosition(m_player1.getTipHitBox());

    m_scoreText.setString(std::format("{}", m_player1.getScore()));
    m_scoreText.setPosition(sf::Vector2f{
        m_topBound.position.x +
            ((m_topBound.size.x - m_scoreText.getGlobalBounds().size.x) / 2.f),
        m_topBound.position.y - 40.f});
    // check for closing window event
    while (const std::optional event = m_window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        m_window.close();
      }
      if (const auto *mouseButtonPressed =
              event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
          // std::cout << "the right button was pressed" << std::endl;
          sf::Vector2f oldPos{m_player1.getPosition()};
          sf::Angle oldAngle{m_player1.getRotation()};
          m_player1.pivot();
          if (m_topBound.findIntersection(m_player1.getGlobalBounds()) ||
              m_bottomBound.findIntersection(m_player1.getGlobalBounds()) ||
              m_rightBound.findIntersection(m_player1.getGlobalBounds()) ||
              m_leftBound.findIntersection(m_player1.getGlobalBounds())) {
            m_player1.setRotation(oldAngle);
            m_player1.setPosition(oldPos);
          }
        }
      }
    }

    m_ball.move(m_ball.getSpeed());
    if (m_topBound.findIntersection(m_ball.getGlobalBounds()) ||
        m_bottomBound.findIntersection(m_ball.getGlobalBounds())) {
      m_ball.flipSpeedY();
      m_ball.increaseBallSpeed();
      m_player1.updateScore();
    }
    if (m_rightBound.findIntersection(m_ball.getGlobalBounds()) ||
        m_leftBound.findIntersection(m_ball.getGlobalBounds())) {
      m_ball.flipSpeedX();
      m_ball.increaseBallSpeed();
      m_player1.updateScore();
    }

    if (m_topBound.contains(m_player1.getTipHitBox()) ||
        m_bottomBound.contains(m_player1.getTipHitBox())) {
      m_player1.swapRotation();
    }
    if (m_rightBound.contains(m_player1.getTipHitBox()) ||
        m_leftBound.contains(m_player1.getTipHitBox())) {
      m_player1.swapRotation();
    }

    if (!m_player1.checkIniviciblity()) {
      if (m_ball.getGlobalBounds().findIntersection(
              m_player1.getGlobalBounds())) {
        m_player1.procIFrames();
        m_player1.minusLife();
        if (!m_player1.isAlive()) {
          m_gameState = GameState::game_over;
        } else {
          m_gameState = GameState::hit;
        }
      }
    }
    m_player1.rotate();
  }
  void draw() override {
    m_window.draw(m_playArea);
    m_window.draw(m_ball);
    m_window.draw(m_player1);
    m_window.draw(m_lineTipHitbox);
    if (m_gameState != GameState::game_over) {
      m_window.draw(m_scoreText);
      m_window.draw(m_livesText);
    }
  }
};
