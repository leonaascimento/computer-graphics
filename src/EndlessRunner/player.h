#ifndef PLAYER_H
#define PLAYER_H

#include <QtOpenGL>

class Player {
 public:
  enum State { Running, JumpingUp, JumpingDown, Dead };

  State state;
  GLfloat positionX, positionY;
  GLfloat velocityY;

  Player();

  void update(GLfloat deltaTime);
  void jump();
};

#endif  // PLAYER_H
