#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>
#include <QtOpenGL>

#include <obstacle.h>
#include <player.h>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
  Q_OBJECT

  Player* player;
  Obstacle* obstacle;

  GLuint vboVertices = 0;
  GLuint vboColors = 0;
  GLuint vboIndices = 0;

  GLuint vao = 0;

  std::unique_ptr<QVector4D[]> vertices = nullptr;
  std::unique_ptr<QVector4D[]> colors = nullptr;
  std::unique_ptr<unsigned int[]> indices = nullptr;

  GLuint shaderProgram;

  float score;

  QTimer updateTimer;
  QTime stopwatch;

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
  void reset();

 signals:

 public slots:
  void update();
};
#endif
