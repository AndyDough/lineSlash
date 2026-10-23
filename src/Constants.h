#pragma once
#include <SFML/System/Vector2.hpp>

namespace GlobalVar {
const sf::Vector2u windowSize{800, 600};
const sf::Vector2f playAreaSize{sf::Vector2f{500, 300}};
const sf::Vector2f lineSize{50.f, 3.f};
const int lives{3};
const float ballRadius{5.f};
const float speedIncrease{1.25f};
const float maxBallSpeed{5.f};
const float startingBallSpeed{2.f};
} // namespace GlobalVar
