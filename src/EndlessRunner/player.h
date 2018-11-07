#ifndef PLAYER_H
#define PLAYER_H

#include <QOpenGLExtraFunctions>
#include <QtOpenGL>

class Player : protected QOpenGLExtraFunctions {
 public:
  Player();
  ~Player();

  GLfloat getPositionX();
  void setPositionX(GLfloat x);

  GLfloat getPositionY();
  void setPositionY(GLfloat y);

  GLfloat getVelocityX();
  void setVelocityX(GLfloat x);

  GLfloat getVelocityY();
  void setVelocityY(GLfloat y);

  GLfloat getLength();
  void setLength(GLfloat length);

  GLfloat getWidth();
  void setWidth(GLfloat width);

  GLfloat getScale();
  void setScale(GLfloat scale);

 private:
  GLfloat m_positionX, m_positionY;
  GLfloat m_velocityX, m_velocityY;
  GLfloat m_length, m_width;
  GLfloat m_scale;
};

#endif  // PLAYER_H
