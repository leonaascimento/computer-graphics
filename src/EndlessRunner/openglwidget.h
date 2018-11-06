#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>
#include <QtOpenGL>

#include <memory>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
  Q_OBJECT

  GLuint vboVertices = 0;
  GLuint vboColors = 0;
  GLuint vboIndices = 0;

  GLuint vao = 0;

  std::unique_ptr<QVector4D[]> vertices = nullptr;
  std::unique_ptr<QVector4D[]> colors = nullptr;
  std::unique_ptr<unsigned int[]> indices = nullptr;

  GLuint shaderProgram;

  float playerPosYOffset;  // Player displacement along Y axis
  float playerPosY;        // Current player Y position

  float targetPosYOffset;  // Target displacement along Y axis
  float targetPosY;        // Current target Y position

  bool shooting;         // Whether the projectile is being animated
  float projectilePosX;  // Projectile X position
  float projectilePosY;  // Projectile Y position
  int numHits;           // Number of hits

  QTimer timer;
  QTime time;

 public:
  explicit OpenGLWidget(QWidget* parent = 0);

  void createVBOs();
  void createShaders();

  void destroyVBOs();
  void destroyShaders();

 protected:
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();

  void keyPressEvent(QKeyEvent* event);
  void keyReleaseEvent(QKeyEvent* event);

 signals:
  void updateHitsLabel(QString);

 public slots:
  void animate();
};
#endif
