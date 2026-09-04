#include "objects/Spaceship.h"
#include "managers/ResourceManager.h"

Spaceship::Spaceship() : y(300), ySpeed(0), radius(40),
                         sprite(ResourceManager::getInstance().getTexture("spaceship")) {
    // Center the origin on the texture so the sprite is drawn around (50, y),
    // matching the collision circle, and scale it to the collision diameter.
    sf::Vector2u texSize = sprite.getTexture().getSize();
    sprite.setOrigin({texSize.x / 2.0f, texSize.y / 2.0f});
    sprite.setScale({2.0f * radius / texSize.x, 2.0f * radius / texSize.y});
    sprite.setPosition({50, y});
}

void Spaceship::update(float dt) {
    applyPhysics(dt);
    constrainMovement();
    sprite.setPosition({50, y});
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
    sprite.setPosition({50, y});
}

void Spaceship::applyPhysics(float dt) {
    float frames = dt * 60.0f; // Normalize to 60 FPS, like Obstacle::update
    ySpeed += GRAVITY * frames;
    y += ySpeed * frames;
}

void Spaceship::constrainMovement() {
    // Top boundary only — falling below the screen must stay possible,
    // Game::checkCollisions treats it as hitting the ground (game over).
    if (y < radius) {
        y = radius;
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
