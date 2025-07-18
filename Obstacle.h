#pragma once
#include <SFML/Graphics.hpp>
#include "Spaceship.h"

class Obstacle {
public:
    Obstacle(float x, float gap);
    void update(float dt, int score);
    void render(sf::RenderWindow& window);
    bool checkCollision(const Spaceship& spaceship) const;
    bool passed(const Spaceship& ship) const;
    bool isScored() const;
    void setScored(bool val);
private:
    float x, y, width, gap;
    bool scored;
    sf::Sprite sprite;
};