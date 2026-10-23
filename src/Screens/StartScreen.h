#pragma once
#include "BaseScreen.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

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
        // std::cout << "A key was pressed!" << std::endl;
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
