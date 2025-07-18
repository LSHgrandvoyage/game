#include "ResourceManager.h"

ResourceManager& ResourceManager::getInstance(){
    static ResourceManager instance;
    return instance;
}

void ResourceManager::loadTexture(const std::string& name, const std::string& filename){
    sf::Texture texture;
    texture.loadFromFile(filename);
    textures[name] = texture;
}

sf::Texture& ResourceManager::getTexture(const std::string& name){
    return textures.at(name);
}

void ResourceManager::loadFont(const std::string& name, const std::string& filename){
    sf::Font font;
    font.loadFromFile(filename);
    fonts[name] = font;
}

sf::Font& ResourceManager::getFont(const std::string& name){
    return fonts.at(name);
}