#pragma once
#include <SFML/Graphics.hpp>

class Spaceship {
public:
    Spaceship();
    void update(float dt);
    void render(sf::RenderWindow& window);
    void onInput(float volume);
    void reset();
    float getY() const;
    float getRadius() const;
    sf::FloatRect getBounds() const;
private:
    float y, ySpeed, gravity, radius;
    sf::Sprite sprite;
};