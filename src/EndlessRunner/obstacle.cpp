#include "obstacle.h"

Obstacle::Obstacle() {
  positionX = 0;
  positionY = 0;
  velocityX = 0;
  colliderWidth = 0;
  colliderHeight = 0;
  scale = 1;
}

void Obstacle::update(GLfloat deltaTime) {
  positionX += velocityX * deltaTime;

  if (positionX < -1.1f) {
    positionX = 1.1f;
  }
}
