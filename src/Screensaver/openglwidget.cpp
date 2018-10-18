#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent) : QOpenGLWidget(parent) {
  createSquare();
}

void OpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();

  qDebug("OpenGL version %s", glGetString(GL_VERSION));
  qDebug("GLSL %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

  const GLchar* vshader = getShaderCode(":/shaders/vshader.vsh");
  const GLchar* fshader = getShaderCode(":/shaders/fshader.frag");

  compileShaders(vshader, fshader);
  bindSquare();
  animateSquare();
}

void OpenGLWidget::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
}

void OpenGLWidget::paintGL() {
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  drawSquare();
}

template <typename T>
int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

void OpenGLWidget::createSquare() {
  // Four vertices to define a square
  vertices = std::make_unique<QVector4D[]>(4);
  vertices[0] = QVector4D(-0.1f, -0.1f, 0, 1);
  vertices[1] = QVector4D(+0.1f, -0.1f, 0, 1);
  vertices[2] = QVector4D(+0.1f, +0.1f, 0, 1);
  vertices[3] = QVector4D(-0.1f, +0.1f, 0, 1);

  // Two triangles to define the mesh topology of the square
  indices = std::make_unique<GLuint[]>(2 * 3);
  indices[0] = 0;
  indices[1] = 1;
  indices[2] = 2;

  indices[3] = 2;
  indices[4] = 3;
  indices[5] = 0;
}

void OpenGLWidget::bindSquare() {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vboVertices);
  glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
  glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(QVector4D), vertices.get(),
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, sizeof(vertices), GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &vboIndices);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(GLuint), indices.get(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void OpenGLWidget::animateSquare() {
  QTimer* timer = new QTimer(this);
  QTime* elapsedTime = new QTime();

  // Square is initially going to upper right
  float* directionX = new float(1.0f);
  float* directionY = new float(1.0f);

  // Square should start in different locations
  std::srand(static_cast<uint>(std::time(nullptr)));
  float* translationX =
      new float(static_cast<float>(std::rand() % 18001 - 9000.0f) / 10000.0f);
  float* translationY =
      new float(static_cast<float>(std::rand() % 18001 - 9000.0f) / 10000.0f);

  connect(timer, &QTimer::timeout,
          [this, elapsedTime, translationX, translationY, directionX,
           directionY]() {
            rotateSquare();
            updateTranslation(elapsedTime, translationX, translationY,
                              directionX, directionY);
            updateSquare(*translationX, *translationY);
          });

  timer->start(0);
  elapsedTime->start();
}

void OpenGLWidget::rotateSquare() {
  float c = std::cos(1.0f * 3.14159265f / 180.0f);
  float s = std::sin(1.0f * 3.14159265f / 180.0f);

  for (int i = 0; i < 4; ++i) {
    float xt = vertices.get()[i].x() - 0.0f;
    float yt = vertices.get()[i].y() - 0.0f;

    float xr = xt * c - yt * s;
    float yr = xt * s + yt * c;

    vertices.get()[i].setX(xr);
    vertices.get()[i].setY(yr);
  }
}

void OpenGLWidget::updateTranslation(QTime* elapsedTime,
                                     float* translationX,
                                     float* translationY,
                                     float* directionX,
                                     float* directionY) {
  float speed = 0.5f;
  float deltaTime = elapsedTime->restart() / 1500.0f;

  if (std::abs(*translationX) >= 0.9f &&
      sgn(*directionX) == sgn(*translationX)) {
    *directionX *= -1;
  }

  if (std::abs(*translationY) >= 0.9f &&
      sgn(*directionY) == sgn(*translationY)) {
    *directionY *= -1;
  }

  *translationX += speed * deltaTime * *directionX;
  *translationY += speed * deltaTime * *directionY;
}

void OpenGLWidget::updateSquare(float translationX, float translationY) {
  makeCurrent();

  glBindBuffer(GL_ARRAY_BUFFER, vboVertices);

  QVector4D* position = static_cast<QVector4D*>(glMapBufferRange(
      GL_ARRAY_BUFFER, 0, 4 * sizeof(QVector4D), GL_MAP_WRITE_BIT));

  for (int i = 0; i < 4; ++i) {
    position[i].setX(vertices.get()[i].x() + translationX);
    position[i].setY(vertices.get()[i].y() + translationY);
  }

  glUnmapBuffer(GL_ARRAY_BUFFER);
  update();
}

void OpenGLWidget::drawSquare() {
  glUseProgram(program);

  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);

  glUseProgram(0);
}

void OpenGLWidget::compileShaders(const GLchar* vertexShaderCode,
                                  const GLchar* fragmentShaderCode) {
  if (!(program = glCreateProgram())) {
    qDebug("Failed to create program");
    return;
  }
  if (!addShader(program, vertexShaderCode, GL_VERTEX_SHADER)) {
    qDebug("Failed to add vertex shader");
    return;
  }
  if (!addShader(program, fragmentShaderCode, GL_FRAGMENT_SHADER)) {
    qDebug("Failed to add fragment shader");
    return;
  }
  if (!linkProgram(program)) {
    qDebug("Failed to link program");
    return;
  }
  if (!validateProgram(program)) {
    qDebug("Failed to validate program");
    return;
  }
}

GLint OpenGLWidget::addShader(GLuint program,
                              const GLchar* shaderCode,
                              GLenum shaderType) {
  GLint status = 0;
  GLuint shader = glCreateShader(shaderType);
  GLint codeLength = static_cast<GLint>(strlen(shaderCode));

  glShaderSource(shader, 1, &shaderCode, &codeLength);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

  if (status == GL_FALSE) {
    GLint logLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

    GLchar* errorMessage = new GLchar[static_cast<GLuint>(logLength)];
    glGetShaderInfoLog(shader, logLength, &logLength, errorMessage);

    qDebug("Error compiling the %d shader:\r\n%s", shaderType, errorMessage);

    glDeleteShader(shader);

    return status;
  }

  glAttachShader(program, shader);
  return status;
}

GLint OpenGLWidget::linkProgram(GLuint program) {
  GLint status = 0;

  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &status);

  if (status == GL_FALSE) {
    GLsizei logLength = 0;
    glGetShaderiv(program, GL_INFO_LOG_LENGTH, &logLength);

    GLchar* errorMessage = new GLchar[static_cast<GLuint>(logLength)];
    glGetProgramInfoLog(program, logLength, &logLength, errorMessage);

    qDebug("Error linking program:\r\n%s", errorMessage);

    glDeleteProgram(program);

    return status;
  }

  return status;
}

GLint OpenGLWidget::validateProgram(GLuint program) {
  GLint status = 0;

  glValidateProgram(program);
  glGetProgramiv(program, GL_VALIDATE_STATUS, &status);

  if (status == GL_FALSE) {
    GLsizei logLength = 0;
    glGetShaderiv(program, GL_INFO_LOG_LENGTH, &logLength);

    GLchar* errorMessage = new GLchar[static_cast<GLuint>(logLength)];
    glGetProgramInfoLog(program, logLength, &logLength, errorMessage);

    qDebug("Error validating program:\r\n%s", errorMessage);

    glDeleteProgram(program);

    return status;
  }

  return status;
}

const GLchar* OpenGLWidget::getShaderCode(const GLchar* shaderPath) {
  QFile file(shaderPath);
  file.open(QFile::ReadOnly | QFile::Text);
  QTextStream textStream(&file);
  return textStream.readAll().toStdString().c_str();
}
