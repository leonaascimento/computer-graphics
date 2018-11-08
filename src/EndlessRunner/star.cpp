#include "star.h"

Star::Star() {
  positionX = 0;
  positionY = 0;
  velocityX = 0;
  scale = 1;
}

void Star::update(GLfloat deltaTime) {
  positionX += velocityX * deltaTime;

  if (positionX < -1.1f) {
    positionX = 1.1f;
  }
}
