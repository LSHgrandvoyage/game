#pragma once
#include <SFML/Graphics.hpp>

class Spaceship {
public:
    Spaceship();
    void update(float dt);
    void render(sf::RenderWindow& window);
    void handleMicInput(float volume);
    void giveBoost();
    void reset();
    
    // Getters
    float getY() const;
    float getRadius() const;
    sf::FloatRect getBounds() const;
    
private:
    // Physics constants
    static constexpr float GRAVITY = 0.05f;
    static constexpr float MAX_ASCENT_SPEED = -6.0f;
    static constexpr float MAX_DESCENT_SPEED = 8.0f;
    static constexpr float LIFT_MULTIPLIER = 0.12f;
    static constexpr float MIN_VOLUME_THRESHOLD = 0.5f;
    static constexpr float BOOST_FORCE = -4.0f;
    
    // Physics properties
    float y;
    float ySpeed;
    float radius;
    
    // Rendering
    sf::Sprite sprite;
    
    // Helper methods
    void applyPhysics(float dt);
    void constrainMovement();
};