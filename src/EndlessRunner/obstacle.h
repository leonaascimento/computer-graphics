#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QtOpenGL>

class Obstacle {
 public:
  GLfloat positionX, positionY;
  GLfloat velocityX;
  GLfloat colliderWidth, colliderHeight;
  GLfloat scale;

  Obstacle();

  void update(GLfloat deltaTime);
};

#endif  // OBSTACLE_H
