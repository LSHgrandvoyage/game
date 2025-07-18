#include "Obstacle.h"
#include "ResourceManager.h"
#include <cstdlib>

Obstacle::Obstacle(float x_, float gap_) : x(x_), width(50), gap(gap_), scored(false) {
    y = rand() % 400 + 50;
    sprite.setTexture(ResourceManager::getInstance().getTexture("obstacle"));
    sprite.setPosition(x, y);
}

void Obstacle::update(float dt, int score){
    float speed = 2.0f;
    x -= speed;
    if (x < -width){
        x = 800;
        float minGap = 100.0f;
        float currentGap = gap - (score / 10) * 20.0f;
        if (currentGap < minGap) currentGap = minGap;
        y = rand() % (int)(600 - currentGap - 50) + 50;
        gap = currentGap;
        scored = false;
    }
    sprite.setPosition(x, y);
}

void Obstacle::render(sf::RenderWindow& window){
    sprite.setPosition(x, 0);
    sprite.setScale(width / sprite.getTexture()->getSize().x, y / sprite.getTexture()->getSize().y);
    window.draw(sprite);

    sprite.setPosition(x, y + gap);
    sprite.setScale(width / sprite.getTexture()->getSize().x, (600 - y - gap) / sprite.getTexture()->getSize().y);
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
