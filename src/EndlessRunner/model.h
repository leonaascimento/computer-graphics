#ifndef MODEL_H
#define MODEL_H

#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>
#include <QtOpenGL>

class Model : protected QOpenGLExtraFunctions {
 public:
  explicit Model(QOpenGLWidget* glWidget);
  ~Model();

  void create(QVector<QVector4D> vertices, QVector<GLuint> indices);
  void destroy();
  void beginDraw();
  void draw();
  void endDraw();

 private:
  QOpenGLWidget* m_glWidget;
  GLuint m_vaoId;
  GLuint m_vboId;
  GLuint m_iboId;
};

#endif  // MODEL_H
