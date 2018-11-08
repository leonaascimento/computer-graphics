#include "shaderprogram.h"

ShaderProgram::ShaderProgram(QOpenGLWidget* glWidget) {
  m_glWidget = glWidget;
}

ShaderProgram::~ShaderProgram() {
  destroy();
  m_glWidget = nullptr;
}

void ShaderProgram::create(const QString& vertexShaderFile,
                           const QString& fragmentShaderFile) {
  initializeOpenGLFunctions();

  QFile vs(vertexShaderFile);
  QFile fs(fragmentShaderFile);

  vs.open(QFile::ReadOnly | QFile::Text);
  fs.open(QFile::ReadOnly | QFile::Text);

  QTextStream streamVs(&vs), streamFs(&fs);

  QString qtStringVs = streamVs.readAll();
  QString qtStringFs = streamFs.readAll();

  std::string stdStringVs = qtStringVs.toStdString();
  std::string stdStringFs = qtStringFs.toStdString();

  // Create an empty vertex shader handle
  GLuint vertexShader = 0;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  // Send the vertex shader source code to GL
  const GLchar* source = stdStringVs.c_str();

  glShaderSource(vertexShader, 1, &source, nullptr);

  // Compile the vertex shader
  glCompileShader(vertexShader);

  GLint isCompiled = 0;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
    // The maxLength includes the NULL character
    std::vector<GLchar> infoLog(static_cast<GLuint>(maxLength));
    glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
    qDebug("%s", &infoLog[0]);

    glDeleteShader(vertexShader);
    return;
  }

  // Create an empty fragment shader handle
  GLuint fragmentShader = 0;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  // Send the fragment shader source code to GL
  // Note that std::string's .c_str is NULL character terminated.
  source = stdStringFs.c_str();
  glShaderSource(fragmentShader, 1, &source, nullptr);

  // Compile the fragment shader
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<GLchar> infoLog(static_cast<GLuint>(maxLength));
    glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
    qDebug("%s", &infoLog[0]);

    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    return;
  }

  // Vertex and fragment shaders are successfully compiled.
  // Now time to link them together into a program.
  // Get a program object.
  m_shaderProgramId = glCreateProgram();

  // Attach our shaders to our program
  glAttachShader(m_shaderProgramId, vertexShader);
  glAttachShader(m_shaderProgramId, fragmentShader);

  // Link our program
  glLinkProgram(m_shaderProgramId);

  // Note the different functions here: glGetProgram* instead of glGetShader*.
  GLint isLinked = 0;
  glGetProgramiv(m_shaderProgramId, GL_LINK_STATUS, &isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(m_shaderProgramId, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> infoLog(static_cast<GLuint>(maxLength));
    glGetProgramInfoLog(m_shaderProgramId, maxLength, &maxLength, &infoLog[0]);
    qDebug("%s", &infoLog[0]);

    glDeleteProgram(m_shaderProgramId);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return;
  }

  glDetachShader(m_shaderProgramId, vertexShader);
  glDetachShader(m_shaderProgramId, fragmentShader);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  vs.close();
  fs.close();
}

void ShaderProgram::destroy() {
  glDeleteProgram(m_shaderProgramId);
  m_shaderProgramId = 0;
}

void ShaderProgram::beginUse() {
  m_glWidget->makeCurrent();

  glUseProgram(m_shaderProgramId);
}

void ShaderProgram::endUse() {
  glUseProgram(0);
}

GLint ShaderProgram::getUniformLocation(const QString& name) {
  return glGetUniformLocation(m_shaderProgramId, name.toStdString().c_str());
}
