#include "Constants.h"
#include "Screens/GameOverScreen.h"
#include "Screens/GameScreen.h"
#include "Screens/GameState.h"
#include "Screens/HitScreen.h"
#include "Screens/StartScreen.h"
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

    gameScreen.draw();
    switch (currentState) {
    case GameState::starting_screen:
      startScreen.draw();
      if (startScreen.isButtonPressed()) {
        currentState = GameState::running;
        continue;
      }
      startScreen.update();
      break;
    case GameState::hit:
      hitScreen.draw();
      if (hitScreen.isButtomPressed()) {
        currentState = GameState::running;
        continue;
      }
      hitScreen.update();
      break;
    case GameState::game_over:
      gameOverScreen.draw();
      if (gameOverScreen.isButtomPressed()) {
        currentState = GameState::starting_screen;
        gameScreen.reset();
        gameScreen.update();
        continue;
      }
      gameOverScreen.update();
      break;
    default:
      gameScreen.update();
    }

    window.display();
  }

  return 0;
}
