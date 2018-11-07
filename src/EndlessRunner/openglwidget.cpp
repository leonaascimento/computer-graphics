#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent) : QOpenGLWidget(parent) {}

void OpenGLWidget::newGame() {
  player = new Player;
  player->positionX = -0.8f;
  player->positionY = -0.8f;
  player->velocityY = 0;
  player->scale = 0.1f;
  player->colliderHeight = 0.2f;
  player->colliderWidth = 0.2f;

  obstacle = new Obstacle;
  obstacle->positionX = 1.1f;
  obstacle->positionY = -0.8f;
  obstacle->velocityX = -1.5f;
  obstacle->scale = 0.1f;
  obstacle->colliderHeight = 0.2f;
  obstacle->colliderWidth = 0.2f;

  cloud = new Obstacle;
  cloud->positionX = 0.5f;
  cloud->positionY = 0.3f;
  cloud->velocityX = -0.1f;
  cloud->scale = 0.05f;
  cloud->colliderHeight = 0;
  cloud->colliderWidth = 0;

  gameScore = 0;
}

OpenGLWidget::~OpenGLWidget() {}

void OpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();

  qDebug("OpenGL version: %s", glGetString(GL_VERSION));
  qDebug("GLSL %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

  glClearColor(0, 0, 0, 1);

  createShaders();
  createVBOs();
  newGame();

  connect(&updateTimer, SIGNAL(timeout()), this, SLOT(update()));
  updateTimer.start(0);
  stopwatch.start();
}

void OpenGLWidget::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
}

void OpenGLWidget::paintGL() {
  QPainter painter(this);
  painter.beginNativePainting();
  glClear(GL_COLOR_BUFFER_BIT);

  GLint scalingId = glGetUniformLocation(shaderProgramId, "scaling");
  GLint translationId = glGetUniformLocation(shaderProgramId, "translation");

  glUseProgram(shaderProgramId);
  glBindVertexArray(vaoId);

  // Draw Player
  glUniform2f(translationId, player->positionX, player->positionY);
  glUniform1f(scalingId, player->scale);
  glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);

  // Draw Obstacle
  glUniform2f(translationId, obstacle->positionX, obstacle->positionY);
  glUniform1f(scalingId, obstacle->scale);
  glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);

  // Draw Cloud
  glUniform2f(translationId, cloud->positionX, cloud->positionY);
  glUniform1f(scalingId, cloud->scale);
  glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);
  glUseProgram(0);

  painter.endNativePainting();

  painter.setPen(Qt::white);
  painter.setFont(QFont("Arial", 12));
  painter.drawText(rect(), Qt::AlignRight,
                   QString("%1").arg(QString::number(
                       static_cast<int>(std::floor(gameScore)))));

  if (player->state == Player::Dead) {
    painter.setPen(Qt::red);
    painter.setFont(QFont("Arial", 36));
    painter.drawText(rect(), Qt::AlignCenter, "GAME OVER");
  }
}

void OpenGLWidget::createShaders() {
  // makeCurrent();
  destroyShaders();

  QFile vs(":/shaders/vshader.glsl");
  QFile fs(":/shaders/fshader.glsl");

  vs.open(QFile::ReadOnly | QFile::Text);
  fs.open(QFile::ReadOnly | QFile::Text);

  QTextStream streamVs(&vs), streamFs(&fs);

  QString qtStringVs = streamVs.readAll();
  QString qtStringFs = streamFs.readAll();

  std::string stdStringVs = qtStringVs.toStdString();
  std::string stdStringFs = qtStringFs.toStdString();

  // qDebug("%s", stdStringVs.c_str());
  // qDebug("length: %i, size: %i, strlen:%i", stdStringVs.length(),
  // stdStringVs.size(), strlen(stdStringVs.c_str()));

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
  shaderProgramId = glCreateProgram();

  // Attach our shaders to our program
  glAttachShader(shaderProgramId, vertexShader);
  glAttachShader(shaderProgramId, fragmentShader);

  // Link our program
  glLinkProgram(shaderProgramId);

  // Note the different functions here: glGetProgram* instead of glGetShader*.
  GLint isLinked = 0;
  glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(shaderProgramId, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> infoLog(static_cast<GLuint>(maxLength));
    glGetProgramInfoLog(shaderProgramId, maxLength, &maxLength, &infoLog[0]);
    qDebug("%s", &infoLog[0]);

    glDeleteProgram(shaderProgramId);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return;
  }

  glDetachShader(shaderProgramId, vertexShader);
  glDetachShader(shaderProgramId, fragmentShader);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  vs.close();
  fs.close();
}

void OpenGLWidget::destroyShaders() {
  makeCurrent();
  glDeleteProgram(shaderProgramId);
}

void OpenGLWidget::createVBOs() {
  makeCurrent();
  destroyVBOs();

  vertices = {QVector4D(-1, -1, 0, 1), QVector4D(1, -1, 0, 1),
              QVector4D(1, 1, 0, 1), QVector4D(-1, 1, 0, 1)};
  indices = {0, 1, 2, 2, 3, 0};

  glGenVertexArrays(1, &vaoId);
  glBindVertexArray(vaoId);

  glGenBuffers(1, &vboId);
  glBindBuffer(GL_ARRAY_BUFFER, vboId);
  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * static_cast<GLint>(sizeof(QVector4D)),
               vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &iboId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               indices.size() * static_cast<GLint>(sizeof(unsigned int)),
               indices.data(), GL_DYNAMIC_DRAW);

  glBindVertexArray(0);
}

void OpenGLWidget::destroyVBOs() {
  glDeleteBuffers(1, &iboId);
  glDeleteBuffers(1, &vboId);
  glDeleteVertexArrays(1, &vaoId);

  iboId = 0;
  vboId = 0;
  vaoId = 0;
}

void OpenGLWidget::update() {
  float deltaTime = stopwatch.restart() / 1000.0f;

  if (player->state != Player::Dead) {
    obstacle->update(deltaTime);
    player->update(deltaTime);
    cloud->update(deltaTime);

    obstacle->velocityX -= 0.1f * deltaTime;
    cloud->velocityX -= 0.1f * deltaTime;

    // Collision
    if (std::fabs(obstacle->positionX - player->positionX) <=
        std::max(obstacle->colliderWidth, player->colliderWidth)) {
      if (std::fabs(player->positionY - obstacle->positionY) <=
          std::max(obstacle->colliderHeight, player->colliderWidth)) {
        player->state = Player::Dead;
      }
    }

    gameScore += 10 * deltaTime;
  }

  QOpenGLWidget::update();
}

// QOpenGLWidget focusPolicy property must be set to StrongFocus
void OpenGLWidget::keyPressEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Space)
    player->jump();
  else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    newGame();
  else if (event->key() == Qt::Key_Escape)
    QApplication::quit();
}
