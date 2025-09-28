#include "objects/Spaceship.h"
#include "managers/ResourceManager.h"

Spaceship::Spaceship() : y(300), ySpeed(0), radius(40) {
    sprite.setTexture(ResourceManager::getInstance().getTexture("spaceship"));
    sprite.setOrigin(radius, radius);
    sprite.setScale(0.15f, 0.1f);
    sprite.setPosition(50, y);
}

void Spaceship::update(float dt) {
    applyPhysics(dt);
    constrainMovement();
    sprite.setPosition(50, y);
}

void Spaceship::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Spaceship::handleMicInput(float volume) {
    if (volume > MIN_VOLUME_THRESHOLD) {
        float lift = -volume * LIFT_MULTIPLIER;
        ySpeed += lift;
        if (ySpeed < MAX_ASCENT_SPEED) {
            ySpeed = MAX_ASCENT_SPEED;
        }
    }
    if (ySpeed > MAX_DESCENT_SPEED) {
        ySpeed = MAX_DESCENT_SPEED;
    }
}

void Spaceship::giveBoost() {
    ySpeed += BOOST_FORCE;
    if (ySpeed < MAX_ASCENT_SPEED) {
        ySpeed = MAX_ASCENT_SPEED;
    }
}

void Spaceship::reset() {
    y = 300;
    ySpeed = 0;
    sprite.setPosition(50, y);
}

void Spaceship::applyPhysics(float dt) {
    ySpeed += GRAVITY;
    y += ySpeed;
}

void Spaceship::constrainMovement() {
    // Top boundary
    if (y < radius) {
        y = radius;
        ySpeed = 0;
    }
    // Bottom boundary
    if (y > 900 - radius) {
        y = 900 - radius;
        ySpeed = 0;
    }
}

float Spaceship::getY() const { 
    return y; 
}

float Spaceship::getRadius() const { 
    return radius; 
}

sf::FloatRect Spaceship::getBounds() const { 
    return sprite.getGlobalBounds(); 
}