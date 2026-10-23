#pragma once
#include "BaseScreen.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

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
