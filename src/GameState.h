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
  sf::Vector2f m_center;
  sf::RectangleShape m_playArea;
  sf::FloatRect m_topBound;
  sf::FloatRect m_bottomBound;
  sf::FloatRect m_rightBound;
  sf::FloatRect m_leftBound;
  Ball m_ball;
  Player m_player1;
  GameState &m_gameState;

public:
  GameScreen(sf::RenderWindow &window, GameState &state)
      : m_window{window},
        m_center{(static_cast<sf::Vector2f>(window.getSize()) / 2.f) -
                 GlobalVar::playAreaSize / 2.f},
        m_ball{GlobalVar::ballRadius, GlobalVar::startingBallSpeed,
               (m_center * 2.f)},
        m_gameState{state}, m_player1({m_center.x + 20.f, m_center.y * 2.f}),
        // since we know that playarea position is m_center and size is
        // GlobalVar::playAreaSize, we can just hardcode the bounds
        m_topBound{{m_center.x - 10.f, m_center.y - 10.f},
                   sf::Vector2f{GlobalVar::playAreaSize.x + 20.f, 10.f}},
        m_bottomBound{{m_center.x, m_center.y + GlobalVar::playAreaSize.y},
                      sf::Vector2f{GlobalVar::playAreaSize.x + 10.f, 10.f}},
        m_leftBound{{m_center.x - 10.f, m_center.y},
                    sf::Vector2f{10.f, GlobalVar::playAreaSize.y + 10.f}},
        m_rightBound{{m_center.x + GlobalVar::playAreaSize.x, m_center.y},
                     sf::Vector2f{10.f, GlobalVar::playAreaSize.y + 10.f}} {
    m_playArea.setSize(GlobalVar::playAreaSize);
    m_playArea.setFillColor(sf::Color::Black);
    m_playArea.setOutlineColor(sf::Color::White);
    m_playArea.setOutlineThickness(10.0);
    m_playArea.setPosition(m_center);

    // just for reference

    // Walls with that can be hit lol
    // sf::Vector2f playAreaPos{m_playArea.getPosition()};
    // sf::Vector2f playAreaSize{m_playArea.getSize()};

    // sf::FloatRect topBound{{playAreaPos.x - 10.f, playAreaPos.y - 10.f},
    //                        sf::Vector2f{playAreaSize.x + 20.f, 10.f}};
    // sf::FloatRect bottomBound{
    //     sf::Vector2f{playAreaPos.x, playAreaPos.y + playAreaSize.y},
    //     sf::Vector2f{playAreaSize.x + 10.f, 10.f}};

    // sf::FloatRect leftBound{{playAreaPos.x - 10.f, playAreaPos.y},
    //                         sf::Vector2f{10.f, playAreaSize.y + 10.f}};
    // sf::FloatRect rightBound{
    //     sf::Vector2f{playAreaPos.x + playAreaSize.x, playAreaPos.y},
    //     sf::Vector2f{10.f, playAreaSize.y + 10.f}};
  }

  void update() override {
    // check for closing window event
    while (const std::optional event = m_window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        m_window.close();
      }
      if (const auto *mouseButtonPressed =
              event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
          std::cout << "the right button was pressed" << std::endl;
          m_player1.pivot();
        }
      }
    }

    m_ball.move(m_ball.getSpeed());
    if (m_topBound.findIntersection(m_ball.getGlobalBounds()) ||
        m_bottomBound.findIntersection(m_ball.getGlobalBounds())) {
      m_ball.flipSpeedY();
      m_ball.increaseBallSpeed();
    }
    if (m_rightBound.findIntersection(m_ball.getGlobalBounds()) ||
        m_leftBound.findIntersection(m_ball.getGlobalBounds())) {
      m_ball.flipSpeedX();
      m_ball.increaseBallSpeed();
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
      if (m_player1.getGlobalBounds().findIntersection(
              m_ball.getGlobalBounds())) {
        std::cout << "ball hit line\n";
        m_player1.procIFrames();
        m_gameState = GameState::hit;
        // m_ball.flipSpeedX(); // Finicky could instead of bouncy just explode
        // line/player
      }
    }
    m_player1.rotate();
  }
  void draw() override {
    m_window.draw(m_playArea);
    m_window.draw(m_ball);
    m_window.draw(m_player1);
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

class ScreenState {
private:
  GameState m_state{GameState::starting_screen};

public:
  ScreenState() = default;
  GameState getState() const { return m_state; }

  sf::Text getStartScreen(sf::Font font) {
    sf::Text text(font);

    return text;
  }

  sf::Text getPauseScreen(sf::Font font);
  sf::Text getGameOverScreen(sf::Font font, int score);
  sf::Text getWinScreen(sf::Font font);

  void draw();
  void update();
};

class HitScreen : public BaseScreen {
private:
  sf::RenderWindow &m_window;
  sf::Text m_hitText;
  mutable bool m_buttonPressed;

public:
  HitScreen(sf::RenderWindow &window, sf::Font &font)
      : m_window{window}, m_hitText{font}, m_buttonPressed{false} {
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
  void draw() override { m_window.draw(m_hitText); }
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
