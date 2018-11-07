#include "player.h"

Player::Player() {
  state = Running;
  positionX = 0;
  positionY = 0;
  velocityY = 0;
  colliderWidth = 0;
  colliderHeight = 0;
  scale = 1;
}

void Player::update(GLfloat deltaTime) {
  GLfloat gravity = 20.0f;

  if (state == JumpingUp || state == JumpingDown) {
    positionY += velocityY * deltaTime;
    velocityY += -gravity * deltaTime;

    if (state == JumpingUp && velocityY <= 0) {
      state = JumpingDown;
    } else if (positionY < -0.8f) {
      positionY = -0.8f;
      velocityY = 0;
      state = Running;
    }
  }
}

void Player::jump() {
  if (state == Running) {
    state = JumpingUp;
    velocityY = 5;
  }
}
