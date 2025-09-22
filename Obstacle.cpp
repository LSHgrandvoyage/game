#include "Obstacle.h"
#include "ResourceManager.h"
#include <cstdlib>

Obstacle::Obstacle(float x_, float gap_) : x(x_), gap(gap_), scored(false) {
    resetPosition(0);
    sprite.setTexture(ResourceManager::getInstance().getTexture("obstacle"));
}

void Obstacle::update(float dt, int score) {
    x -= SPEED * dt * 60.0f; // Normalize to 60 FPS
    if (x < -WIDTH) {
        x = SCREEN_WIDTH;
        resetPosition(score);
        scored = false;
    }
}

void Obstacle::render(sf::RenderWindow& window) {
    renderUpperObstacle(window);
    renderLowerObstacle(window);
}

bool Obstacle::checkCollision(const Spaceship& spaceship) const {
    if (!isInXRange(spaceship)) {
        return false;
    }
    
    float shipY = spaceship.getY();
    float shipRadius = spaceship.getRadius();
    
    // Check collision with upper obstacle
    if (shipY - shipRadius < y) {
        return true;
    }
    
    // Check collision with lower obstacle
    if (shipY + shipRadius > y + gap) {
        return true;
    }
    
    return false;
}

bool Obstacle::hasPassed(const Spaceship& ship) const {
    return (x + WIDTH < SPACESHIP_X) && !scored;
}

bool Obstacle::isScored() const { 
    return scored; 
}

void Obstacle::setScored(bool val) { 
    scored = val; 
}

void Obstacle::resetPosition(int score) {
    float currentGap = gap - (score / 10) * GAP_REDUCTION_PER_SCORE;
    if (currentGap < MIN_GAP) {
        currentGap = MIN_GAP;
    }
    
    float maxY = SCREEN_HEIGHT - currentGap - MIN_HEIGHT;
    y = rand() % (int)(maxY - MIN_HEIGHT + 1) + MIN_HEIGHT;
    gap = currentGap;
}

void Obstacle::renderUpperObstacle(sf::RenderWindow& window) {
    sprite.setPosition(x, 0);
    sprite.setScale(WIDTH / sprite.getTexture()->getSize().x, 
                   y / sprite.getTexture()->getSize().y);
    window.draw(sprite);
}

void Obstacle::renderLowerObstacle(sf::RenderWindow& window) {
    sprite.setPosition(x, y + gap);
    sprite.setScale(WIDTH / sprite.getTexture()->getSize().x, 
                   (SCREEN_HEIGHT - (y + gap)) / sprite.getTexture()->getSize().y);
    window.draw(sprite);
}

bool Obstacle::isInXRange(const Spaceship& spaceship) const {
    float shipX = SPACESHIP_X;
    float shipRadius = spaceship.getRadius();
    return (shipX + shipRadius > x && shipX - shipRadius < x + WIDTH);
}
