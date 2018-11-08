#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>
#include <QtOpenGL>

#include <model.h>
#include <obstacle.h>
#include <player.h>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
  Q_OBJECT

 public:
  explicit OpenGLWidget(QWidget* parent = nullptr);
  ~OpenGLWidget();

  void createShaders();
  void createVBOs();
  void destroyShaders();

 protected:
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();

  void keyPressEvent(QKeyEvent* event);
  void newGame();

 private:
  Player* player;
  Obstacle* obstacle;
  Obstacle* cloud;

  Model* star;
  Model* square;

  GLuint shaderProgramId;

  float gameScore;

  QTimer updateTimer;
  QTime stopwatch;

 signals:

 public slots:
  void update();
};
#endif
