#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "GameObject.h"
#include "Spaceship.h"
#include "Obstacle.h"
#include "ResourceManager.h"
#include "AudioManager.h"

class Game {
public:
    Game();
    void run();
private:
    void handleEvents();
    void update(float dt);
    void render();
    void resetGame();

    sf::RenderWindow window;
    std::unique_ptr<Spaceship> spaceship;
    std::vector<Obstacle> obstacles;
    int score = 0;
    int highScore = 0;
    bool gameOver = false;
    bool showLevelUp = false;
    float levelUpTime = 0;
    float levelUpDuration = 2.0f;
};