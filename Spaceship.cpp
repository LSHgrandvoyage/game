#include "Spaceship.h"
#include "ResourceManager.h"

Spaceship::Spaceship() : y(300), ySpeed(0), gravity(0.2f), radius(40) {
    sprite.setTexture(ResourceManager::getInstance().getTexture("spaceship"));
    sprite.setOrigin(radius, radius);
    sprite.setScale(0.15f, 0.1f);
    sprite.setPosition(50, y);
}

void Spaceship::update(float dt){
    ySpeed += gravity;
    y += ySpeed;
    if (y < radius){
        y = radius;
        ySpeed = 0;
    }
    sprite.setPosition(50, y);
}

void Spaceship::render(sf::RenderWindow& window){
    window.draw(sprite);
}

void Spaceship::onInput(float volume){
    if (volume > 5.0f) ySpeed = -volume * 0.05f;
}

void Spaceship::reset() {
    y = 300;
    ySpeed = 0;
    sprite.setPosition(50, y);
}

float Spaceship::getY() const { return y; }
float Spaceship::getRadius() const { return radius; }
sf::FloatRect Spaceship::getBounds() const { return sprite.getGlobalBounds(); }