#include "Obstacle.h"
#include "ResourceManager.h"
#include <cstdlib>

Obstacle::Obstacle(float x_, float gap_) : x(x_), width(80), gap(gap_), scored(false) {
    float minHeight = 100.0f;
    float maxY = 900 - gap - minHeight;
    y = rand() % (int)(maxY - minHeight + 1) + minHeight;
    sprite.setTexture(ResourceManager::getInstance().getTexture("obstacle"));
    sprite.setPosition(x, y);
}

void Obstacle::update(float dt, int score){
    float speed = 2.5f;
    x -= speed;
    if (x < -width){
        x = 1200;
        float minGap = 150.0f;
        float minHeight = 100.0f;
        float currentGap = gap - (score / 10) * 30.0f;
        if (currentGap < minGap) currentGap = minGap;
        float maxY = 900 - currentGap - minHeight;
        y = rand() % (int)(maxY - minHeight + 1) + minHeight;
        gap = currentGap;
        scored = false;
    }
    sprite.setPosition(x, y);
}

void Obstacle::render(sf::RenderWindow& window){
    // upper obstacle
    sprite.setPosition(x, 0);
    sprite.setScale(width / sprite.getTexture()->getSize().x, y / sprite.getTexture()->getSize().y);
    window.draw(sprite);

    // lower obstacle
    sprite.setPosition(x, y + gap);
    sprite.setScale(width / sprite.getTexture()->getSize().x, (900 - (y + gap)) / sprite.getTexture()->getSize().y);
    window.draw(sprite);
}

bool Obstacle::checkCollision(const Spaceship& spaceship) const {
    return (x + width < 50) && !scored;
}

bool Obstacle::passed(const Spaceship& ship) const {
    return (x + width < 50) && !scored;
}

bool Obstacle::isScored() const { return scored; }
void Obstacle::setScored(bool val) { scored = val; }
