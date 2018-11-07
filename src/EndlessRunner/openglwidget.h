#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>
#include <QtOpenGL>

#include <player.h>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
  Q_OBJECT

  Player* player;

  GLuint vboVertices = 0;
  GLuint vboColors = 0;
  GLuint vboIndices = 0;

  GLuint vao = 0;

  std::unique_ptr<QVector4D[]> vertices = nullptr;
  std::unique_ptr<QVector4D[]> colors = nullptr;
  std::unique_ptr<unsigned int[]> indices = nullptr;

  GLuint shaderProgram;

  // float playerVelocityY;
  // float playerVelocityX;

  float gravity;

  // float playerPosY;
  float obstaclePosX;

  float score;

  QTimer timer;
  QTime time;

 public:
  explicit OpenGLWidget(QWidget* parent = nullptr);
  ~OpenGLWidget();

  void createVBOs();
  void createShaders();

  void destroyVBOs();
  void destroyShaders();

 protected:
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();

  void keyPressEvent(QKeyEvent* event);

 signals:

 public slots:
  void animate();
};
#endif
