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
  GameOverScreen gameOverScreen{window, font, gameScreen};

  // Main loop
  while (window.isOpen()) {

    window.clear(sf::Color::Black);
    // ready to be updated after

    float delta = clock.restart().asSeconds();

    switch (currentState) {
    case GameState::starting_screen:
      startScreen.update();
      if (startScreen.isButtonPressed()) {
        currentState = GameState::running;
        continue;
      }
      gameScreen.draw();
      startScreen.draw();
      break;
    case GameState::hit:
      hitScreen.update();
      if (hitScreen.isButtomPressed()) {
        currentState = GameState::running;
        continue;
      }
      gameScreen.draw();
      hitScreen.draw();
      break;
    case GameState::game_over:
      gameOverScreen.update();
      if (gameOverScreen.isButtomPressed()) {
        currentState = GameState::starting_screen;
        continue;
      }
      gameScreen.draw();
      gameOverScreen.draw();
      break;
    default:
      gameScreen.update();
      gameScreen.draw();
    }

    window.display();
  }

  return 0;
}
