#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class ResourceManager {
public:
    static ResourceManager& getInstance();
    void loadTexture(const std::string& name, const std::string& filename);
    sf::Texture& getTexture(const std::string& name);
    void loadFont(const std::string& name, const std::string& filename);
    sf::Font& getFont(const std::string& name);
    // Destroying sf::Texture needs a live GL context. The singleton outlives
    // main(), so call this while the window still exists (Game::~Game).
    void clear();
private:
    ResourceManager() = default;
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Font> fonts;
};