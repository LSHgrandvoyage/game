#pragma once
#include <SFML/Audio.hpp>
#include <atomic>
#include <map>
#include <memory>
#include <string>

// Computes RMS volume per captured chunk. Polling SoundBufferRecorder's whole
// buffer made the volume an all-time average that grew without bound.
class MicVolumeRecorder : public sf::SoundRecorder {
public:
    float getVolume() const;

protected:
    bool onProcessSamples(const std::int16_t* samples, std::size_t sampleCount) override;

private:
    std::atomic<float> volume{0.0f};
};

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

    // Release audio resources and stop the recorder while the audio engine is
    // still alive — the singleton's own destructor runs too late (after main).
    void shutdown();

    // Microphone input
    void updateMicVolume();
    float getMicVolume() const;      // ambient-noise-adjusted (0 while calibrating)
    float getRawMicVolume() const;
    float getNoiseFloor() const;
    bool isCalibrating() const;
    bool isMicAvailable() const;

private:
    AudioManager();
    ~AudioManager();

    // Audio resources
    std::map<std::string, std::unique_ptr<sf::Music>> musics;
    std::map<std::string, sf::SoundBuffer> soundBuffers;
    std::map<std::string, sf::Sound> sounds;

    // Microphone handling
    MicVolumeRecorder recorder;
    float currentVolume;
    float rawVolume;
    float noiseFloor;
    float calibrationSum;
    int calibrationFramesLeft;
    bool micInitialized;

    // Ambient noise level varies per machine, so the floor is measured at
    // startup instead of hardcoded: keep quiet for the first ~1.5s.
    static constexpr int CALIBRATION_FRAME_COUNT = 90;

    // Helper methods
    void initializeMicrophone();
};
