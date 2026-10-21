#include "Ball.h"
#include "Constants.h"
#include "GameState.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <iostream>
#include <optional>

int main() {
  sf::RenderWindow window(sf::VideoMode(GlobalVar::windowSize), "My Window");
  window.setFramerateLimit(60);

  sf::Clock clock;

  // Starting game state
  GameState currentState{GameState::starting_screen};

  // Initializing font
  sf::Font font;
  if (!font.openFromFile("./assets/font.ttf")) {
    std::cerr << "Failed to load font from file\n";
  }
  GameScreen gameScreen{window, currentState, font};
  StartScreen startScreen{window, font};
  HitScreen hitScreen{window, font};

  // Main loop
  while (window.isOpen()) {

    window.clear(sf::Color::Black);
    // ready to be updated after

    float delta = clock.restart().asSeconds();

    if (currentState == GameState::starting_screen) {
      startScreen.update();
      if (startScreen.isButtonPressed()) {
        currentState = GameState::running;
        continue;
      }
      gameScreen.draw();
      startScreen.draw();
    } else if (currentState == GameState::hit) {
      hitScreen.update();
      if (hitScreen.isButtomPressed()) {
        currentState = GameState::running;
        continue;
      }
      gameScreen.draw();
      hitScreen.draw();
    } else {
      gameScreen.update();
      gameScreen.draw();
    }

    // if (currentState.getState() != GameState::running) {
    //   switch (currentState.getState()) {
    //   case GameState::starting_screen:
    //     currentState.getStartScreen(font);
    //     break;
    //   case GameState::paused:
    //     currentState.getPauseScreen(font);
    //     break;
    //   case GameState::game_over:
    //     currentState.getGameOverScreen(font);
    //     break;
    //   case GameState::win:
    //     currentState.getWinScreen(font);
    //     break;
    //   default:
    //     break;
    //   }
    // }

    window.display();
  }

  return 0;
}
