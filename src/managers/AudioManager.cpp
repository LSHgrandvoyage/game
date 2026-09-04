#include "AudioManager.h"
#include <algorithm>
#include <iostream>
#include <cmath>

float MicVolumeRecorder::getVolume() const {
    return volume.load();
}

bool MicVolumeRecorder::onProcessSamples(const std::int16_t* samples, std::size_t sampleCount) {
    if (sampleCount == 0) {
        return true;
    }

    float sum = 0.0f;
    for (std::size_t i = 0; i < sampleCount; ++i) {
        float sample = static_cast<float>(samples[i]) / 32768.0f; // Normalize to -1.0 ~ 1.0
        sum += sample * sample;
    }

    float rms = std::sqrt(sum / sampleCount);
    volume.store(rms * 100.0f); // Convert to 0 ~ 100 range
    return true;
}

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

AudioManager::AudioManager() : currentVolume(0.0f), rawVolume(0.0f), noiseFloor(0.0f),
                               calibrationSum(0.0f), calibrationFramesLeft(CALIBRATION_FRAME_COUNT),
                               micInitialized(false) {
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
        musics[name]->setLooping(loop);
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
        auto [it, inserted] = soundBuffers.insert_or_assign(name, std::move(buffer));
        sounds.emplace(name, sf::Sound(it->second));
    } else {
        std::cout << "Failed to load sound: " << filename << std::endl;
    }
}

void AudioManager::shutdown() {
    if (micInitialized) {
        recorder.stop();
        micInitialized = false;
    }
    stopMusic();
    sounds.clear();
    soundBuffers.clear();
    musics.clear();
}

void AudioManager::playSound(const std::string& name) {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        it->second.play();
    }
}

void AudioManager::updateMicVolume() {
    if (!micInitialized) {
        currentVolume = 0.0f;
        return;
    }
    rawVolume = recorder.getVolume();

    if (calibrationFramesLeft > 0) {
        calibrationSum += rawVolume;
        calibrationFramesLeft--;
        if (calibrationFramesLeft == 0) {
            float ambientAvg = calibrationSum / CALIBRATION_FRAME_COUNT;
            noiseFloor = ambientAvg * 2.0f + 0.5f; // headroom over ambient average
            std::cout << "Mic calibrated - ambient avg: " << ambientAvg
                      << ", noise floor: " << noiseFloor << std::endl;
        }
        currentVolume = 0.0f;
        return;
    }

    currentVolume = std::max(0.0f, rawVolume - noiseFloor);
}

float AudioManager::getMicVolume() const {
    return currentVolume;
}

float AudioManager::getRawMicVolume() const {
    return rawVolume;
}

float AudioManager::getNoiseFloor() const {
    return noiseFloor;
}

bool AudioManager::isCalibrating() const {
    return micInitialized && calibrationFramesLeft > 0;
}

bool AudioManager::isMicAvailable() const {
    return micInitialized;
}

void AudioManager::initializeMicrophone() {
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
