#include "AudioManager.h"
#include <iostream>
#include <cmath>
#include <algorithm>

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

AudioManager::AudioManager() : currentVolume(0.0f), micInitialized(false) {
    initializeMicrophone();
}

AudioManager::~AudioManager() {
    if (micInitialized) {
        recorder.stop();
    }
}

void AudioManager::loadMusic(const std::string& name, const std::string& filename) {
    auto music = std::make_unique<sf::Music>();
    if (music->openFromFile(filename)) {
        musics[name] = std::move(music);
    } else {
        std::cout << "Failed to load music: " << filename << std::endl;
    }
}

void AudioManager::playMusic(const std::string& name, bool loop) {
    if (musics.count(name)) {
        musics[name]->setLoop(loop);
        musics[name]->play();
    }
}

void AudioManager::stopMusic() {
    for (auto& music : musics) {
        if (music.second) {
            music.second->stop();
        }
    }
}

void AudioManager::loadSound(const std::string& name, const std::string& filename) {
    sf::SoundBuffer buffer;
    if (buffer.loadFromFile(filename)) {
        soundBuffers[name] = buffer;
        sounds[name].setBuffer(soundBuffers[name]);
    } else {
        std::cout << "Failed to load sound: " << filename << std::endl;
    }
}

void AudioManager::playSound(const std::string& name) {
    if (sounds.count(name)) {
        sounds[name].play();
    }
}

void AudioManager::updateMicVolume() {
    if (!micInitialized) {
        currentVolume = 0.0f;
        return;
    }
    
    const sf::SoundBuffer& buffer = recorder.getBuffer();
    currentVolume = calculateRMSVolume(buffer);
}

float AudioManager::getMicVolume() const {
    return currentVolume;
}

bool AudioManager::isMicAvailable() const {
    return micInitialized;
}

void AudioManager::initializeMicrophone() {
    // Check available audio devices
    std::cout << "Available audio devices:" << std::endl;
    auto devices = sf::SoundRecorder::getAvailableDevices();
    for (size_t i = 0; i < devices.size(); ++i) {
        std::cout << "  " << i << ": " << devices[i] << std::endl;
    }
    
    if (recorder.start()) {
        micInitialized = true;
        std::cout << "Microphone initialized successfully" << std::endl;
        std::cout << "Default device: " << sf::SoundRecorder::getDefaultDevice() << std::endl;
    } else {
        std::cout << "Microphone initialization failed. Using default values." << std::endl;
        std::cout << "Error: Cannot start audio recording" << std::endl;
    }
}

float AudioManager::calculateRMSVolume(const sf::SoundBuffer& buffer) const {
    const sf::Int16* samples = buffer.getSamples();
    std::size_t sampleCount = buffer.getSampleCount();
    
    if (sampleCount == 0) {
        return 0.0f;
    }
    
    float sum = 0.0f;
    float maxSample = 0.0f;
    for (std::size_t i = 0; i < sampleCount; ++i) {
        float sample = static_cast<float>(samples[i]) / 32768.0f; // Normalize to -1.0 ~ 1.0
        sum += sample * sample;
        maxSample = std::max(maxSample, std::abs(sample));
    }
    
    float rms = std::sqrt(sum / sampleCount);
    float volume = rms * 100.0f; // Convert to 0 ~ 100 range
    
    // Debug output every 60 frames (approximately 1 second at 60fps)
    static int debugCounter = 0;
    if (++debugCounter >= 60) {
        debugCounter = 0;
        std::cout << "Mic Debug - RMS: " << rms << ", Volume: " << volume 
                  << ", Max Sample: " << maxSample << ", Samples: " << sampleCount << std::endl;
    }
    
    return volume;
}

