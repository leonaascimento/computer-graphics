#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>
#include <QtOpenGL>

class ShaderProgram : protected QOpenGLExtraFunctions {
 public:
  explicit ShaderProgram(QOpenGLWidget* glWidget);
  ~ShaderProgram();

  void create(const QString& vertexShaderFile,
              const QString& fragmentShaderFile);
  void destroy();
  GLint getUniformLocation(const QString& name);
  void beginUse();
  void endUse();

 private:
  QOpenGLWidget* m_glWidget;
  GLuint m_shaderProgramId;
};

#endif  // SHADERPROGRAM_H
