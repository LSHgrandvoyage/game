#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "objects/Spaceship.h"
#include "objects/Obstacle.h"
#include "managers/ResourceManager.h"
#include "managers/AudioManager.h"

class Game {
public:
    Game();
    void run();
    
private:
    // Game constants
    static constexpr int WINDOW_WIDTH = 1200;
    static constexpr int WINDOW_HEIGHT = 900;
    static constexpr int INITIAL_OBSTACLE_COUNT = 5;
    static constexpr float OBSTACLE_SPACING = 300.0f;
    static constexpr float INITIAL_GAP = 400.0f;
    static constexpr float LEVEL_UP_DURATION = 2.0f;
    static constexpr int SCORE_PER_LEVEL = 10;
    
    // Core game loop
    void handleEvents();
    void update(float dt);
    void render();
    void resetGame();
    
    // Initialization
    void initializeResources();
    void initializeGameObjects();
    
    // Game state management
    void updateGameplay(float dt);
    void updateObstacles(float dt);
    void checkCollisions();
    void updateScore();
    void handleGameOver();
    
    // Rendering
    void renderBackground();
    void renderUI();
    void renderGameOver();
    void renderLevelUp();
    void renderDebugInfo();
    
    // Game objects
    sf::RenderWindow window;
    std::unique_ptr<Spaceship> spaceship;
    std::vector<Obstacle> obstacles;
    
    // Game state
    int score;
    int highScore;
    bool gameOver;
    bool showLevelUp;
    float levelUpTime;
    
    // Debug/fallback
    bool spacePressed;
    float simulatedVolume;
};