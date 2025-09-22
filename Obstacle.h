#pragma once
#include <SFML/Graphics.hpp>
#include "Spaceship.h"

class Obstacle {
public:
    Obstacle(float x, float gap);
    void update(float dt, int score);
    void render(sf::RenderWindow& window);
    
    // Collision detection
    bool checkCollision(const Spaceship& spaceship) const;
    bool hasPassed(const Spaceship& ship) const;
    
    // Scoring
    bool isScored() const;
    void setScored(bool val);
    
public:
    // Constants
    static constexpr float WIDTH = 80.0f;
    static constexpr float SPEED = 2.5f;
    static constexpr float MIN_HEIGHT = 100.0f;
    static constexpr float MIN_GAP = 150.0f;
    static constexpr float GAP_REDUCTION_PER_SCORE = 30.0f;
    static constexpr float SPACESHIP_X = 50.0f;
    static constexpr float SCREEN_WIDTH = 1200.0f;
    static constexpr float SCREEN_HEIGHT = 900.0f;

private:
    
    // Properties
    float x;
    float y;
    float gap;
    bool scored;
    
    // Rendering
    sf::Sprite sprite;
    
    // Helper methods
    void resetPosition(int score);
    void renderUpperObstacle(sf::RenderWindow& window);
    void renderLowerObstacle(sf::RenderWindow& window);
    bool isInXRange(const Spaceship& spaceship) const;
};