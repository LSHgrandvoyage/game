import ddf.minim.*;
import ddf.minim.analysis.*;

Minim minim;
AudioPlayer bgm;
AudioPlayer boomsound;
AudioPlayer levelupsound;
AudioInput in;

float radius = 40;
float y;
float ySpeed;
float gravity = 0.2;

        PImage spaceship, littleplanet, spacebackground, explode;

PVector[] obstacles;
int numObstacles = 5;
float obstacleGap = 200;
float obstacleWidth = 50;
float speed = 2;

int score = 0;
int highScore = 0;
boolean gameOver = false;
boolean restart = false;

boolean boomsoundPlayed = false;

float initialGap = 300;  // initial gap of pass road
float minGap = 100;      // minimum gap of pass road

boolean showLevelUp = false;
float levelUpTime = 0;
float levelUpDuration = 2; // 2 seconds

void setup() {
    size(800, 600);
    minim = new Minim(this);
    in = minim.getLineIn(Minim.MONO, 2048);

    // Load sounds
    bgm = minim.loadFile("bgm.mp3");
    boomsound = minim.loadFile("boomsound.mp3");
    levelupsound = minim.loadFile("levelup.mp3");
    bgm.loop();

    // Load images
    spaceship = loadImage("spaceship.png");
    littleplanet = loadImage("littleplanet.png");
    spacebackground = loadImage("spacebackground.png");
    explode = loadImage("explode.png");

    y = height / 2;
    ySpeed = 0;

    obstacles = new PVector[numObstacles];
    for (int i = 0; i < numObstacles; i++) {
        float x = width + i * (obstacleGap + obstacleWidth);
        float h = random(50, height - initialGap);
        obstacles[i] = new PVector(x, h);
    }
}

void draw() {
    if (gameOver) {
        if (!boomsoundPlayed) {
            bgm.pause();
            boomsound.rewind();
            boomsound.play();
            boomsoundPlayed = true;
        }

        image(explode, 0, 0, width, height);
        fill(0); // Change text color to black
        textSize(64);
        textAlign(CENTER);
        textFont(createFont("Arial", 64));
        text("BOOM!!", width / 2, height / 2 - 50);
        textSize(32);
        text("Score: " + score, width / 2, height / 2);
        text("High Score: " + highScore, width / 2, height / 2 + 50);
        restart = true;
        return;
    } else if (!bgm.isPlaying()) {
        bgm.loop();
    }

    // Draw background
    if (spacebackground != null) {
        background(spacebackground);
    } else {
        background(0);
    }

    // Update spaceship
    float volume = in.mix.level() * 1000; // Adjust spaceship's movement speed based on user's voice volume
    ySpeed += gravity;
    y += ySpeed;

    // Update spaceship's position
    if (volume > 5) {
        ySpeed = -volume * 0.05; // Increase lift value based on volume size
    }

    // Prevent spaceship from going off-screen
    if (y < radius) {
        y = radius;
        ySpeed = 0;
    }

    // Game over if spaceship falls to the ground
    if (y > height + radius * 2) {
        gameOver = true;
    }

    // Update obstacles' positions
    for (int i = 0; i < numObstacles; i++) {
        obstacles[i].x -= speed;
        if (obstacles[i].x < -obstacleWidth) {
            obstacles[i].x = width;

            // Adjust gap size based on score
            float currentGap = initialGap - (score / 10) * 20;
            if (currentGap < minGap) {
                currentGap = minGap;
            }

            obstacles[i].y = random(50, height - currentGap);
            score++;
            if (score > highScore) {
                highScore = score;
            }

            if (score % 10 == 0) {
                showLevelUp = true;
                levelUpTime = millis() / 1000.0;
                levelupsound.rewind();
                levelupsound.play();
            }
        }

        fill(0);
        float currentGap = initialGap - (score / 10) * 20;
        if (currentGap < minGap) {
            currentGap = minGap;
        }
        image(littleplanet, obstacles[i].x, 0, obstacleWidth, obstacles[i].y);
        image(littleplanet, obstacles[i].x, obstacles[i].y + currentGap, obstacleWidth, height - obstacles[i].y - currentGap);

        if (collides(obstacles[i], currentGap)) {
            gameOver = true;
        }
    }

    // Draw spaceship
    image(spaceship, 50 - radius, y - radius, radius * 2, radius * 2);

    // Display score
    fill(255);
    textSize(32);
    text("Score: " + score, 100, 50);
    text("High Score: " + highScore, 100, 100);

    // print Level up
    if (showLevelUp) {
        fill(255, 255, 0); // Change text color to yellow
        textSize(48);
        textAlign(CENTER);
        text("Level UP!", width / 2, height / 2);

        if (millis() / 1000.0 - levelUpTime > levelUpDuration) {
            showLevelUp = false;
        }
    }
}

// Function to check collision
boolean collides(PVector obstacle, float gap) {
    float spaceshipWidth = radius * 0.5;
    float spaceshipHeight = radius * 0.5;

    // Define spaceship bounds
    float spaceshipLeft = 50 - spaceshipWidth / 2;
    float spaceshipRight = 50 + spaceshipWidth / 2;
    float spaceshipTop = y - spaceshipHeight / 2;
    float spaceshipBottom = y + spaceshipHeight / 2;

    // Check collision with the obstacles
    if (spaceshipRight > obstacle.x && spaceshipLeft < obstacle.x + obstacleWidth) {
        if (spaceshipTop < obstacle.y || spaceshipBottom > obstacle.y + gap) {
            return true;
        }
    }
    return false;
}

void resetGame() {
    y = height / 2;
    ySpeed = 0;
    score = 0;
    for (int i = 0; i < numObstacles; i++) {
        float x = width + i * (obstacleGap + obstacleWidth);
        float h = random(50, height - initialGap);
        obstacles[i] = new PVector(x, h);
    }
    gameOver = false;
    restart = false;
    boomsoundPlayed = false;
    bgm.rewind();
    boomsound.pause();
    levelupsound.pause();
}

void mousePressed() {
    if (gameOver && restart) {
        resetGame();
    }
}

void stop() {
    in.close();
    minim.stop();
    super.stop();
}
