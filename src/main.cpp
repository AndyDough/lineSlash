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

int FPS_SET = 60;
int UPS_SET = 80;

float getDeltaTime() {
  return 1.0f / UPS_SET;
}


int main() {
  using namespace std::chrono;
  sf::RenderWindow window(sf::VideoMode(GlobalVar::windowSize), "My Window");
  //window.setFramerateLimit(60);


  double timePerDraw = 1000'000'000.0 / FPS_SET;
  double timePerUpdate = 1000'000'000.0 / UPS_SET;
  auto previousTime = steady_clock::now();

  int frames = 0;
  int updates = 0;
  auto lastCheck = system_clock::now();

  double deltaF = 0;
  double deltaU = 0;


  // Starting game state
  GameState currentState{GameState::starting_screen};

  // Initializing font
  sf::Font font;
  if (!font.openFromFile("../assets/font.ttf")) {
    std::cerr << "Failed to load font from file\n";
  }
  GameScreen gameScreen{window, currentState, font};
  StartScreen startScreen{window, font};
  HitScreen hitScreen{window, font};
  GameOverScreen gameOverScreen{window, font, gameScreen};

  // Main loop
  while (window.isOpen()) {

    auto currentTime = steady_clock::now();

    deltaU += (currentTime - previousTime).count() / timePerUpdate;
    deltaF += (currentTime - previousTime).count() / timePerDraw;
    previousTime = currentTime;

    window.clear(sf::Color::Black);
    // ready to be updated after
    switch (currentState) {
      case GameState::running:
      {
        if(deltaF >= 1)
        {
          gameScreen.draw();
          window.display();
          frames++;
          deltaF--;
        }
        if (deltaU >= 1) {
          gameScreen.update();
          updates++;
          deltaU--;
			}
        break;
      }
    case GameState::starting_screen:
    {
      if(deltaF >= 1)
      {
        gameScreen.draw();
        startScreen.draw();
        window.display();
				frames++;
				deltaF--;
      }

      if (deltaU >= 1) {
        if (startScreen.isButtonPressed()) 
        {
          currentState = GameState::running;
          continue;
        }
        startScreen.update();
				updates++;
				deltaU--;
			}
      break;
    }
    case GameState::hit:
    {
      if(deltaF >= 1)
      {
        gameScreen.draw();
        hitScreen.draw();
        window.display();
				frames++;
				deltaF--;
      }

      if (deltaU >= 1) {
        if (hitScreen.isButtomPressed()) {
          currentState = GameState::running;
          continue;
        }
        hitScreen.update();
				updates++;
				deltaU--;
			}

      break;
    }
    case GameState::game_over:
    {
      if(deltaF >= 1)
      {
        gameScreen.draw();
        gameOverScreen.draw();
        window.display();
				frames++;
				deltaF--;
      }
      if (deltaU >= 1) {      
        if (gameOverScreen.isButtomPressed()) {
          currentState = GameState::starting_screen;
          gameScreen.reset();
          gameScreen.update();
          continue;
        }
        gameOverScreen.update();
				updates++;
				deltaU--;
			}

      break;
    }
    default:
      break;
    }

    auto endTime = lastCheck.time_since_epoch();
    if (duration_cast<milliseconds>(endTime).count() >= 1000) 
    {
      lastCheck = system_clock::now();
      frames = 0;
      updates = 0;
    }
  }

  return 0;
}
