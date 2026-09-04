#include "ResourceManager.h"
#include <iostream>

ResourceManager& ResourceManager::getInstance(){
    static ResourceManager instance;
    return instance;
}

void ResourceManager::loadTexture(const std::string& name, const std::string& filename){
    sf::Texture texture;
    if (texture.loadFromFile(filename)) {
        textures.insert_or_assign(name, std::move(texture));
    } else {
        std::cout << "Failed to load texture: " << filename << std::endl;
    }
}

sf::Texture& ResourceManager::getTexture(const std::string& name){
    return textures.at(name);
}

void ResourceManager::loadFont(const std::string& name, const std::string& filename){
    sf::Font font;
    if (font.openFromFile(filename)) {
        fonts.insert_or_assign(name, std::move(font));
    } else {
        std::cout << "Failed to load font: " << filename << std::endl;
    }
}

sf::Font& ResourceManager::getFont(const std::string& name){
    return fonts.at(name);
}

void ResourceManager::clear(){
    textures.clear();
    fonts.clear();
}
