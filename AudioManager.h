#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>

class AudioManager {
public:
    static AudioManager& getInstance();
    void loadMusic(const std::string& name, const std::string& filename);
    void playMusic(const std::string& name, bool loop = true);
    void stopMusic();
    void loadSound(const std::string& name, const std::string& filename);
    void playSound(const std::string& name);
    float getMicVolume();
private:
    AudioManager();
    std::map<std::string, std::unique_ptr<sf::Music>> musics;
    std::map<std::string, sf::SoundBuffer> soundBuffers;
    std::map<std::string, sf::Sound> sounds;
};