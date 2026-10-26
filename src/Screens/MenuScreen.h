#pragma once

#include "BaseScreen.h"
#include "GameMode.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <optional>

class MenuScreen : public BaseScreen {
private:
  sf::RenderWindow &m_window;
  sf::Text m_titleText;
  sf::Text m_menuItemsText;
  int m_playerSelection{0};
  GameMode &m_mode;
  mutable bool m_choiceSelected{false};

public:
  MenuScreen(sf::RenderWindow &window, sf::Font &font, GameMode &mode)
      : m_window{window}, m_titleText(font), m_menuItemsText(font),
        m_mode{mode}, m_playerSelection{0} {
    m_titleText.setString("Not Pong");
    m_titleText.setCharacterSize(50);
    m_titleText.setLineAlignment(sf::Text::LineAlignment::Center);

    m_menuItemsText.setString("Singleplayer\n"
                              "Muliplayer\n"
                              "Quit Game\n");
    m_menuItemsText.setLineAlignment(sf::Text::LineAlignment::Center);
  }

  bool isChoiceSelected() const {
    if (m_choiceSelected) {
      m_choiceSelected = false;
      return true;
    } else {
      return false;
    }
  }

  void update() {
    while (const std::optional event = m_window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        m_window.close();
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        m_playerSelection = std::clamp(++m_playerSelection, 0, 2);
      }
      if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key::Down))) {
        m_playerSelection = std::clamp(--m_playerSelection, 0, 2);
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
        m_mode = static_cast<GameMode>(m_playerSelection);
      }
    }
  };
  void draw();
};
