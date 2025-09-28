#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>

class AudioManager {
public:
    static AudioManager& getInstance();
    
    // Music management
    void loadMusic(const std::string& name, const std::string& filename);
    void playMusic(const std::string& name, bool loop = true);
    void stopMusic();
    
    // Sound effects
    void loadSound(const std::string& name, const std::string& filename);
    void playSound(const std::string& name);
    
    // Microphone input
    void updateMicVolume();
    float getMicVolume() const;
    bool isMicAvailable() const;
    
private:
    AudioManager();
    ~AudioManager();
    
    // Audio resources
    std::map<std::string, std::unique_ptr<sf::Music>> musics;
    std::map<std::string, sf::SoundBuffer> soundBuffers;
    std::map<std::string, sf::Sound> sounds;
    
    // Microphone handling
    sf::SoundBufferRecorder recorder;
    float currentVolume;
    bool micInitialized;
    
    // Helper methods
    void initializeMicrophone();
    float calculateRMSVolume(const sf::SoundBuffer& buffer) const;
};