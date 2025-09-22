#include "Game.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <ctime>
#include <algorithm>

Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Spaceship Game"),
               score(0), highScore(0), gameOver(false), showLevelUp(false), levelUpTime(0),
               spacePressed(false), simulatedVolume(0.0f) {
    srand(static_cast<unsigned>(time(nullptr)));
    initializeResources();
    initializeGameObjects();
}

void Game::run(){
    sf::Clock clock;
    while (window.isOpen()){
        float dt = clock.restart().asSeconds();
        handleEvents();
        update(dt);
        render();
    }
}

void Game::handleEvents(){
    sf::Event event;
    while (window.pollEvent(event)){
        if (event.type == sf::Event::Closed){
            window.close();
        }
        if (gameOver && event.type == sf::Event::MouseButtonPressed){
            resetGame();
        }
        
        // Handle keyboard input for mic simulation
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                spacePressed = true;
                simulatedVolume = 50.0f; // Simulate medium volume
            }
        }
        if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Space) {
                spacePressed = false;
                simulatedVolume = 0.0f;
            }
        }
    }
}

void Game::update(float dt) {
    if (gameOver) return;
    
    updateGameplay(dt);
    updateObstacles(dt);
    checkCollisions();
    updateScore();
}

void Game::render() {
    window.clear();
    
    renderBackground();
    
    for (auto& obs : obstacles) {
        obs.render(window);
    }
    
    spaceship->render(window);
    renderUI();
    renderDebugInfo();
    
    if (showLevelUp) {
        renderLevelUp();
    }
    
    if (gameOver) {
        renderGameOver();
    }
    
    window.display();
}

void Game::resetGame() {
    score = 0;
    gameOver = false;
    showLevelUp = false;
    spacePressed = false;
    simulatedVolume = 0.0f;
    spaceship->reset();
    
    for (size_t i = 0; i < obstacles.size(); i++) {
        float x = WINDOW_WIDTH + i * (OBSTACLE_SPACING + Obstacle::WIDTH);
        obstacles[i] = Obstacle(x, INITIAL_GAP);
    }
    
    AudioManager::getInstance().playMusic("bgm", true);
}

void Game::initializeResources() {
    ResourceManager::getInstance().loadTexture("spaceship", "data/spaceship.png");
    ResourceManager::getInstance().loadTexture("obstacle", "data/littleplanet.png");
    ResourceManager::getInstance().loadTexture("background", "data/spacebackground.png");
    ResourceManager::getInstance().loadTexture("explode", "data/explode.png");
    ResourceManager::getInstance().loadFont("main", "data/arial.ttf");

    AudioManager::getInstance().loadMusic("bgm", "data/bgm.mp3");
    AudioManager::getInstance().loadSound("boom", "data/boomsound.mp3");
    AudioManager::getInstance().loadSound("levelup", "data/levelup.mp3");
    AudioManager::getInstance().playMusic("bgm", true);
}

void Game::initializeGameObjects() {
    spaceship = std::make_unique<Spaceship>();
    
    for (int i = 0; i < INITIAL_OBSTACLE_COUNT; i++) {
        float x = WINDOW_WIDTH + i * (OBSTACLE_SPACING + Obstacle::WIDTH);
        obstacles.emplace_back(x, INITIAL_GAP);
    }
}

void Game::updateGameplay(float dt) {
    AudioManager::getInstance().updateMicVolume();
    
    // Use real mic volume if available, otherwise use simulated volume
    float volume;
    if (AudioManager::getInstance().isMicAvailable()) {
        volume = AudioManager::getInstance().getMicVolume();
    } else {
        volume = simulatedVolume;
    }
    
    spaceship->handleMicInput(volume);
    spaceship->update(dt);
}

void Game::updateObstacles(float dt) {
    for (auto& obs : obstacles) {
        obs.update(dt, score);
    }
}

void Game::checkCollisions() {
    for (auto& obs : obstacles) {
        if (obs.checkCollision(*spaceship)) {
            handleGameOver();
            return;
        }
    }
    
    // Check if spaceship hit the ground
    if (spaceship->getY() > WINDOW_HEIGHT + spaceship->getRadius()) {
        handleGameOver();
    }
}

void Game::updateScore() {
    for (auto& obs : obstacles) {
        if (obs.hasPassed(*spaceship) && !obs.isScored()) {
            score++;
            obs.setScored(true);
            if (score > highScore) {
                highScore = score;
            }
            if (score != 0 && score % SCORE_PER_LEVEL == 0) {
                showLevelUp = true;
                levelUpTime = clock() / (float)CLOCKS_PER_SEC;
                AudioManager::getInstance().playSound("levelup");
            }
        }
    }
    
    if (showLevelUp) {
        float now = clock() / (float)CLOCKS_PER_SEC;
        if (now - levelUpTime > LEVEL_UP_DURATION) {
            showLevelUp = false;
        }
    }
}

void Game::handleGameOver() {
    gameOver = true;
    AudioManager::getInstance().playSound("boom");
    AudioManager::getInstance().stopMusic();
}

void Game::renderBackground() {
    sf::Sprite background(ResourceManager::getInstance().getTexture("background"));
    
    // Calculate scale to maintain aspect ratio and fill screen
    float scaleX = (float)WINDOW_WIDTH / background.getTexture()->getSize().x;
    float scaleY = (float)WINDOW_HEIGHT / background.getTexture()->getSize().y;
    float scale = std::max(scaleX, scaleY); // Use max to ensure full coverage
    
    background.setScale(scale, scale);
    
    // Center the background
    background.setPosition(
        (WINDOW_WIDTH - background.getGlobalBounds().width) / 2,
        (WINDOW_HEIGHT - background.getGlobalBounds().height) / 2
    );
    
    window.draw(background);
}

void Game::renderUI() {
    sf::Font& font = ResourceManager::getInstance().getFont("main");
    
    sf::Text scoreText("Score: " + std::to_string(score), font, 32);
    scoreText.setPosition(30, 20);
    window.draw(scoreText);

    sf::Text highScoreText("High Score: " + std::to_string(highScore), font, 32);
    highScoreText.setPosition(30, 60);
    window.draw(highScoreText);
}

void Game::renderDebugInfo() {
    sf::Font& font = ResourceManager::getInstance().getFont("main");
    
    // Mic status
    bool micAvailable = AudioManager::getInstance().isMicAvailable();
    sf::Text micStatusText("Mic Status: " + std::string(micAvailable ? "OK" : "FAILED"), font, 24);
    micStatusText.setPosition(30, 100);
    micStatusText.setFillColor(micAvailable ? sf::Color::Green : sf::Color::Red);
    window.draw(micStatusText);
    
    // Mic volume
    float volume = AudioManager::getInstance().getMicVolume();
    sf::Text volumeText("Mic Volume: " + std::to_string(static_cast<int>(volume)), font, 24);
    volumeText.setPosition(30, 130);
    volumeText.setFillColor(sf::Color::Yellow);
    window.draw(volumeText);

    // Spaceship position
    sf::Text spaceshipYText("Spaceship Y: " + std::to_string(static_cast<int>(spaceship->getY())), font, 24);
    spaceshipYText.setPosition(30, 160);
    spaceshipYText.setFillColor(sf::Color::Cyan);
    window.draw(spaceshipYText);
    
    // Instructions
    if (!micAvailable) {
        sf::Text instructionText("Press SPACE to simulate mic input", font, 20);
        instructionText.setPosition(30, 190);
        instructionText.setFillColor(sf::Color::White);
        window.draw(instructionText);
    }
}

void Game::renderLevelUp() {
    sf::Font& font = ResourceManager::getInstance().getFont("main");
    sf::Text levelUpText("Level Up!", font, 48);
    levelUpText.setFillColor(sf::Color::Yellow);
    levelUpText.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    levelUpText.setOrigin(levelUpText.getLocalBounds().width / 2, levelUpText.getLocalBounds().height / 2);
    window.draw(levelUpText);
}

void Game::renderGameOver() {
    sf::Font& font = ResourceManager::getInstance().getFont("main");
    
    sf::Sprite boom(ResourceManager::getInstance().getTexture("explode"));
    
    // Calculate scale to maintain aspect ratio and fill screen
    float scaleX = (float)WINDOW_WIDTH / boom.getTexture()->getSize().x;
    float scaleY = (float)WINDOW_HEIGHT / boom.getTexture()->getSize().y;
    float scale = std::max(scaleX, scaleY);
    
    boom.setScale(scale, scale);
    
    // Center the explosion background
    boom.setPosition(
        (WINDOW_WIDTH - boom.getGlobalBounds().width) / 2,
        (WINDOW_HEIGHT - boom.getGlobalBounds().height) / 2
    );
    
    window.draw(boom);

    sf::Text boomText("BOOM!", font, 96);
    boomText.setFillColor(sf::Color::Red);
    boomText.setOrigin(boomText.getLocalBounds().width / 2, boomText.getLocalBounds().height / 2);
    boomText.setPosition(WINDOW_WIDTH / 2, 350);
    window.draw(boomText);

    sf::Text scoreText("Score: " + std::to_string(score), font, 48);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setOrigin(scoreText.getLocalBounds().width / 2, scoreText.getLocalBounds().height / 2);
    scoreText.setPosition(WINDOW_WIDTH / 2, 450);
    window.draw(scoreText);

    sf::Text highScoreText("High Score: " + std::to_string(highScore), font, 48);
    highScoreText.setFillColor(sf::Color::White);
    highScoreText.setOrigin(highScoreText.getLocalBounds().width / 2, highScoreText.getLocalBounds().height / 2);
    highScoreText.setPosition(WINDOW_WIDTH / 2, 520);
    window.draw(highScoreText);
}