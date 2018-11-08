#ifndef STAR_H
#define STAR_H

#include <QtOpenGL>

class Star {
 public:
  GLfloat positionX, positionY;
  GLfloat velocityX;
  GLfloat scale;

  Star();

  void update(GLfloat deltaTime);
};

#endif  // STAR_H
