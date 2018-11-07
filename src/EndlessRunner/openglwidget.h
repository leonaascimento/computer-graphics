#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>
#include <QtOpenGL>

#include <obstacle.h>
#include <player.h>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
  Q_OBJECT

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
  void newGame();

 private:
  Player* player;
  Obstacle* obstacle;

  GLuint shaderProgramId;
  GLuint vaoId;
  GLuint vboId;
  GLuint iboId;

  QVector<QVector4D> vertices;
  QVector<unsigned int> indices;

  float gameScore;

  QTimer updateTimer;
  QTime stopwatch;

 signals:

 public slots:
  void update();
};
#endif
