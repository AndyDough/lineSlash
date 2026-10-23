#pragma once
#include "BaseScreen.h"
#include "GameScreen.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

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
    //m_gameOverText.setLineAlignment(sf::Text::LineAlignment::Center);
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
