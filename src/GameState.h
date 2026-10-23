#pragma once

#include "Ball.h"
#include "Constants.h"
#include "Player.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <format>
#include <optional>

enum class GameState {
  starting_screen,
  running,
  hit,
  paused,
  game_over,
  win,
  max_game_states
};

class BaseScreen {
public:
  virtual void update() = 0;
  virtual void draw() = 0;
};

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
  GameState &m_gameState;
  sf::Text m_scoreText;
  sf::Text m_livesText;

public:
  GameScreen(sf::RenderWindow &window, GameState &state, sf::Font &font)
      : m_window{window}, m_scoreText{font},
        m_screenLeftCorner{(static_cast<sf::Vector2f>(window.getSize()) / 2.f) -
                           GlobalVar::playAreaSize / 2.f},
        m_ball{GlobalVar::ballRadius, GlobalVar::startingBallSpeed,
               (m_screenLeftCorner * 2.f)},
        m_gameState{state},
        m_player1({m_screenLeftCorner.x + 20.f, m_screenLeftCorner.y * 2.f}),
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
    if (m_gameState != GameState::game_over) {
      m_window.draw(m_scoreText);
      m_window.draw(m_livesText);
    }
  }
};

class StartScreen : public BaseScreen {
private:
  sf::RenderWindow &m_window;
  sf::Font &m_font;
  sf::RectangleShape m_background;
  sf::Text m_startText;
  mutable bool m_buttonPressed;

public:
  StartScreen(sf::RenderWindow &window, sf::Font &font)
      : m_window{window}, m_font{font}, m_buttonPressed{false},
        m_startText{font} {
    auto windowSize = static_cast<sf::Vector2f>(window.getSize());
    m_background.setSize(windowSize);
    m_background.setFillColor(sf::Color(0, 0, 0, 155));
    m_background.setPosition({0, 0});

    m_startText.setString("Press any key to start");
    m_startText.setCharacterSize(24);
    m_startText.setFillColor(sf::Color::White);
    m_startText.setStyle(sf::Text::Bold);
    m_startText.setPosition(sf::Vector2f{
        windowSize.x / 2.f - m_startText.getGlobalBounds().size.x / 2.f,
        windowSize.y / 2.f - m_startText.getGlobalBounds().size.y / 2.f});
  }

  void update() override {
    while (const std::optional event = m_window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        m_window.close();
      }
      if (event->is<sf::Event::MouseButtonPressed>()) {
        std::cout << "A key was pressed!" << std::endl;
        m_buttonPressed = true;
      }
    }
  }
  bool isButtonPressed() const {
    if (m_buttonPressed) {
      m_buttonPressed = false; // reset the button pressed state
      return true;
    } else
      return false;
  }

  void draw() override {
    m_window.draw(m_background);
    m_window.draw(m_startText);
  }
};

class HitScreen : public BaseScreen {
private:
  sf::RenderWindow &m_window;
  sf::Text m_hitText;
  sf::RectangleShape m_background;
  mutable bool m_buttonPressed;

public:
  HitScreen(sf::RenderWindow &window, sf::Font &font)
      : m_window{window}, m_hitText{font}, m_buttonPressed{false},
        m_background{static_cast<sf::Vector2f>(window.getSize())} {
    m_background.setFillColor(sf::Color(0, 0, 0, 155));
    m_background.setPosition({0, 0});

    m_hitText.setString("You Got HIT Press A Key To Continue");
    m_hitText.setCharacterSize(24);
    m_hitText.setFillColor(sf::Color::White);
    m_hitText.setStyle(sf::Text::Bold);
    m_hitText.setPosition(sf::Vector2f{
        m_window.getSize().x / 2.f - m_hitText.getGlobalBounds().size.x / 2.f,
        m_window.getSize().y / 2.f - m_hitText.getGlobalBounds().size.y});
  }

  bool isButtomPressed() const {
    if (m_buttonPressed) {
      m_buttonPressed = false;
      return true;
    } else {
      return false;
    }
  }

  void update() override {
    while (const std::optional event = m_window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        m_window.close();
      }
      if (event->is<sf::Event::MouseButtonPressed>()) {
        m_buttonPressed = true;
      }
    }
  }
  void draw() override {
    m_window.draw(m_background);
    m_window.draw(m_hitText);
  }
};

class GameOverScreen : public BaseScreen {
private:
  sf::RectangleShape m_background;
  sf::Text m_gameOverText;
  sf::RenderWindow &m_window;
  GameScreen &m_gameScreenRef;
  mutable bool m_buttonPressed{false};

public:
  GameOverScreen(sf::RenderWindow &window, sf::Font &font,
                 GameScreen &gameScreen)
      : m_window{window}, m_gameOverText{font},
        m_background{static_cast<sf::Vector2f>(window.getSize())},
        m_gameScreenRef{gameScreen} {
    m_background.setFillColor(sf::Color{0, 0, 0, 155});
    m_background.setPosition({0, 0});

    m_gameOverText.setCharacterSize(24);
    m_gameOverText.setFillColor(sf::Color::White);
    m_gameOverText.setStyle(sf::Text::Bold);
  }
  bool isButtomPressed() const {
    if (m_buttonPressed) {
      m_buttonPressed = false;
      return true;
    } else {
      return false;
    }
  }

  void update() override {
    m_gameOverText.setString(std::format("You have ran out of lives\n"
                                         "Your final score\n"
                                         "{}\n\n"
                                         "Press button to continue",
                                         m_gameScreenRef.getScore()));
    m_gameOverText.setLineAlignment(sf::Text::LineAlignment::Center);
    m_gameOverText.setPosition(
        sf::Vector2f{m_window.getSize().x / 2.f,
                     m_window.getSize().y / 2.f -
                         (m_gameOverText.getGlobalBounds().size.y - 50.f)});
    while (const std::optional event = m_window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        m_window.close();
      }
      if (event->is<sf::Event::MouseButtonPressed>()) {
        m_buttonPressed = true;
      }
    }
  }
  void draw() override {
    m_window.draw(m_background);
    m_window.draw(m_gameOverText);
  };
};

// Load font
// sf::Font font;
// if (!font.openFromFile("./assets/BebasNeue-Regular.ttf")) {
//   std::cerr << "Failed to load font from file\n";
// }
//
// // Load text
// sf::Text text(font);
// text.setString("");
// text.setCharacterSize(24);
// text.setFillColor(sf::Color::White);
// text.setStyle(sf::Text::Bold);
