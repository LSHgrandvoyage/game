#pragma once
#include <SFML/Graphics.hpp>

class GameObject {
public:
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual ~GameObject() = default;
};