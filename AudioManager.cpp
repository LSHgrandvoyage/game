#include "AudioManager.h"

AudioManager& AudioManager::getInstance(){
    static AudioManager instance;
    return instance;
}

AudioManager::AudioManager(){
}

void AudioManager::loadMusic(const std::string& name, const std::string& filename){
    auto music = std::make_unique<sf::Music>();
    music->openFromFile(filename);
    musics[name] = std::move(music);
}

void AudioManager::playMusic(const std::string& name, bool loop){
    if (musics.count(name)){
        musics[name]->setLoop(loop);
        musics[name]->play();
    }
}

void AudioManager::stopMusic(){
    for (auto& music : musics){
        if (music.second) music.second->stop();
    }
}

void AudioManager::loadSound(const std::string& name, const std::string& filename){
    sf::SoundBuffer buffer;
    buffer.loadFromFile(filename);
    soundBuffers[name] = buffer;
    sounds[name].setBuffer(soundBuffers[name]);
}

void AudioManager::playSound(const std::string& name){
    sounds[name].play();
}
float AudioManager::getMicVolume(){
    return 10.0f;
}

