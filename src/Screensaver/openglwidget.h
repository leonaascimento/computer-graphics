#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>
#include <QtOpenGL>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
  Q_OBJECT
 public:
  explicit OpenGLWidget(QWidget* parent = nullptr);

 protected:
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();

 private:
  GLuint vboVertices, vboIndices, vao, program;

  std::unique_ptr<QVector4D[]> vertices;
  std::unique_ptr<GLuint[]> indices;

  void createSquare();
  void bindSquare();
  void drawSquare();
  void animateSquare();
  void rotateSquare();
  void updateTranslation(QTime* elapsedTime,
                         float* translationX,
                         float* translationY,
                         float* directionX,
                         float* directionY);
  void updateSquare(float translationX, float translationY);

  void compileShaders(const GLchar* vertexShaderCode,
                      const GLchar* fragmentShaderCode);

  GLint addShader(GLuint program, const GLchar* shaderCode, GLenum shaderType);
  GLint linkProgram(GLuint program);
  GLint validateProgram(GLuint program);

  const GLchar* getShaderCode(const GLchar* shaderPath);

 signals:

 public slots:
};

#endif  // OPENGLWIDGET_H
