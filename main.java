import ddf.minim.*;
import ddf.minim.analysis.*;

Minim minim;
AudioInput in;
float radius = 20;
float y;
float ySpeed;
float gravity = 0.2;

PVector[] obstacles;
int numObstacles = 5;
float obstacleGap = 200;
float obstacleWidth = 50;
float speed = 2;

int score = 0;
int highScore = 0;
boolean gameOver = false;

float initialGap = 300;  // initial gap of pass road
float minGap = 100;      // minimum gap of pass road

void setup() {
  size(800, 600);
  minim = new Minim(this);
  in = minim.getLineIn(Minim.MONO, 2048);
  
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
  background(200);

  if (gameOver) {
    fill(0);
    textSize(32);
    textAlign(CENTER);
    text("Game Over", width / 2, height / 2 - 50);
    text("Score: " + score, width / 2, height / 2);
    text("High Score: " + highScore, width / 2, height / 2 + 50);
    return;
  }

  // 공 업데이트
  float volume = in.mix.level() * 1000; // 사용자 목소리 크기 이용하여 공의 이동 속도 조정
  ySpeed += gravity;
  y += ySpeed;
  
  // 볼의 위치 업데이트
  if (volume > 5) {
    ySpeed = -volume * 0.05; // 볼륨 크기에 따라 리프트 값 증가
  }

  // 최대 높이 화면 안에 위치
  if (y < radius) {
    y = radius;
    ySpeed = 0;
  }

  // 공이 땅에 떨어졌을 때
  if (y > height - radius) {
    y = height - radius;
    ySpeed = 0;
    gameOver = true;
  }

  // 벽
  for (int i = 0; i < numObstacles; i++) {
    obstacles[i].x -= speed;
    if (obstacles[i].x < -obstacleWidth) {
      obstacles[i].x = width;
      
      // 통과 공간 조절
      float currentGap = initialGap - (score / 10) * 20;
      if (currentGap < minGap) {
        currentGap = minGap;
      }
      
      obstacles[i].y = random(50, height - currentGap);
      score++;
      if (score > highScore) {
        highScore = score;
      }
    }
    
    fill(0);
    float currentGap = initialGap - (score / 10) * 20;
    if (currentGap < minGap) {
      currentGap = minGap;
    }
    rect(obstacles[i].x, 0, obstacleWidth, obstacles[i].y);
    rect(obstacles[i].x, obstacles[i].y + currentGap, obstacleWidth, height - obstacles[i].y - currentGap);

    if (collides(obstacles[i], currentGap)) {
      gameOver = true;
    }
  }

  // 공 그리기
  fill(255, 0, 0);
  ellipse(50, y, radius * 2, radius * 2);

  // 점수 표시
  fill(0);
  textSize(32);
  text("Score: " + score, 100, 50);
  text("High Score: " + highScore, 100, 100);
}

boolean collides(PVector obstacle, float gap) {
  if (50 + radius > obstacle.x && 50 - radius < obstacle.x + obstacleWidth) {
    if (y - radius < obstacle.y || y + radius > obstacle.y + gap) {
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
}

void mousePressed() {
  if (gameOver) {
    resetGame();
  }
}

void stop() {
  in.close();
  minim.stop();
  super.stop();
}
