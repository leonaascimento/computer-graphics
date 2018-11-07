#include "player.h"

Player::Player() {}

Player::~Player() {}

GLfloat Player::getPositionX() {
  return m_positionX;
}

void Player::setPositionX(GLfloat x) {
  m_positionX = x;
}

GLfloat Player::getPositionY() {
  return m_positionY;
}

void Player::setPositionY(GLfloat y) {
  m_positionY = y;
}

GLfloat Player::getVelocityX() {
  return m_velocityX;
}

void Player::setVelocityX(GLfloat x) {
  m_velocityX = x;
}

GLfloat Player::getVelocityY() {
  return m_velocityY;
}

void Player::setVelocityY(GLfloat y) {
  m_velocityY = y;
}

GLfloat Player::getLength() {
  return m_length * m_scale;
}

void Player::setLength(GLfloat length) {
  m_length = length;
}

GLfloat Player::getWidth() {
  return m_width * m_scale;
}

void Player::setWidth(GLfloat width) {
  m_width = width;
}

GLfloat Player::getScale() {
  return m_scale;
}

void Player::setScale(GLfloat scale) {
  m_scale = scale;
}
