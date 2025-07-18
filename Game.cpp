#include "Game.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <ctime>

Game::Game() : window(sf::VideoMode(1200, 900), "Spaceship Game") {
    srand(static_cast<unsigned>(time(nullptr)));
    // Resource loading
    ResourceManager::getInstance().loadTexture("spaceship", "data/spaceship.png");
    ResourceManager::getInstance().loadTexture("obstacle", "data/littleplanet.png");
    ResourceManager::getInstance().loadTexture("background", "data/spacebackground.png");
    ResourceManager::getInstance().loadTexture("explode", "data/explode.png");
    ResourceManager::getInstance().loadFont("main", "data/arial.ttf");

    AudioManager::getInstance().loadMusic("bgm", "data/bgm.mp3");
    AudioManager::getInstance().loadSound("boom", "data/boomsound.mp3");
    AudioManager::getInstance().loadSound("levelup", "data/levelup.mp3");
    AudioManager::getInstance().playMusic("bgm", true);

    spaceship = std::make_unique<Spaceship>();

    // Obstacles initialization
    for (int i = 0; i < 5; i++){
        float x = 1200 + i * (300 + 80); // x 시작점, 간격, 장애물 폭 증가
        float gap = 400; // 장애물 gap 증가
        obstacles.emplace_back(x, gap);
    }
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
    }
}

void Game::update(float dt){
    if (gameOver) return;

    float volume = AudioManager::getInstance().getMicVolume();
    spaceship->onInput(volume);
    spaceship->update(dt);

    for (auto& obs : obstacles){
        obs.update(dt, score);
        if (obs.checkCollision(*spaceship)){
            gameOver = true;
            AudioManager::getInstance().playSound("boom");
            AudioManager::getInstance().stopMusic();
        }

        if (obs.passed(*spaceship) && !obs.isScored()){
            score++;
            obs.setScored(true);
            if (score > highScore) highScore = score;
            if (score != 0 &&score % 10 == 0){
                showLevelUp = true;
                levelUpTime = clock() / (float)CLOCKS_PER_SEC;
                AudioManager::getInstance().playSound("levelup");
            }
        }
    }

    if (showLevelUp){
        float now = clock() / (float)CLOCKS_PER_SEC;
        if (now - levelUpTime > levelUpDuration) showLevelUp = false;
    }

    if (spaceship->getY() > 900 + spaceship->getRadius()){
        gameOver = true;
        AudioManager::getInstance().playSound("boom");
        AudioManager::getInstance().stopMusic();
    }
}

void Game::render(){
    window.clear();

    sf::Sprite background(ResourceManager::getInstance().getTexture("background"));
    // 배경을 창 크기에 맞게 스케일 조정
    background.setScale(1200.0f / background.getTexture()->getSize().x, 900.0f / background.getTexture()->getSize().y);
    window.draw(background);

    for (auto& obs : obstacles){
        obs.render(window);
    }

    spaceship->render(window);

    sf::Font& font = ResourceManager::getInstance().getFont("main");
    sf::Text scoreText("Score: " + std::to_string(score), font, 32);
    scoreText.setPosition(30, 20); // score location
    window.draw(scoreText);

    sf::Text highScoreText("High Score: " + std::to_string(highScore), font, 32);
    highScoreText.setPosition(30, 60); // high score location
    window.draw(highScoreText);

    if (showLevelUp){
        sf::Text levelUpText("Level Up!", font, 48);
        levelUpText.setFillColor(sf::Color::Yellow);
        levelUpText.setPosition(600, 450); // Center of the window
        levelUpText.setOrigin(levelUpText.getLocalBounds().width / 2, levelUpText.getLocalBounds().height / 2);
        window.draw(levelUpText);
    }

    if (gameOver){
        sf::Sprite boom(ResourceManager::getInstance().getTexture("explode"));
        // Set the boom background to center
        boom.setScale(1200.0f / boom.getTexture()->getSize().x, 900.0f / boom.getTexture()->getSize().y);
        boom.setPosition(0, 0);
        window.draw(boom);

        sf::Text boomText("BOOM!", font, 96);
        boomText.setFillColor(sf::Color::Red);
        boomText.setOrigin(boomText.getLocalBounds().width / 2, boomText.getLocalBounds().height / 2);
        boomText.setPosition(600, 350);
        window.draw(boomText);

        sf::Text scoreText("Score: " + std::to_string(score), font, 48);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setOrigin(scoreText.getLocalBounds().width / 2, scoreText.getLocalBounds().height / 2);
        scoreText.setPosition(600, 450);
        window.draw(scoreText);

        sf::Text highScoreText("High Score: " + std::to_string(highScore), font, 48);
        highScoreText.setFillColor(sf::Color::White);
        highScoreText.setOrigin(highScoreText.getLocalBounds().width / 2, highScoreText.getLocalBounds().height / 2);
        highScoreText.setPosition(600, 520);
        window.draw(highScoreText);
    }

    window.display();
}

void Game::resetGame(){
    score = 0;
    gameOver = false;
    showLevelUp = false;
    spaceship->reset();
    float gap = 400;
    for (size_t i = 0; i < obstacles.size(); i++){
        float x = 1200 + i * (300 + 80);
        obstacles[i] = Obstacle(x, gap);
    }
    AudioManager::getInstance().playMusic("bgm", true);
}