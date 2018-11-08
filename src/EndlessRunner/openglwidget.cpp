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

  stars.resize(10);
  for (int i = 0; i < stars.size(); i++) {
    float x = static_cast<float>(std::rand() % 18001 - 9000.0f) / 10000.0f;
    float y = static_cast<float>(std::rand() % 18001 - 9000.0f) / 10000.0f;

    Star star;
    star.positionX = x;
    star.positionY = y + 0.2f;
    star.velocityX = -0.1f;
    star.scale = 0.05f;
    stars[i] = star;
  }

  gameScore = 0;
}

OpenGLWidget::~OpenGLWidget() {}

void OpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();

  qDebug("OpenGL version: %s", glGetString(GL_VERSION));
  qDebug("GLSL %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

  glClearColor(0, 0, 0, 1);

  createShaders();
  createModels();
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

  GLint scalingId = defaultShaderProgram->getUniformLocation("scaling");
  GLint translationId = defaultShaderProgram->getUniformLocation("translation");

  defaultShaderProgram->beginUse();

  squareModel->beginDraw();
  // Draw player square
  glUniform2f(translationId, player->positionX, player->positionY);
  glUniform1f(scalingId, player->scale);
  squareModel->draw();

  // Draw obstacle square
  glUniform2f(translationId, obstacle->positionX, obstacle->positionY);
  glUniform1f(scalingId, obstacle->scale);
  squareModel->draw();
  squareModel->endDraw();

  starModel->beginDraw();
  for (int i = 0; i < stars.size(); i++) {
    glUniform2f(translationId, stars[i].positionX, stars[i].positionY);
    glUniform1f(scalingId, stars[i].scale);
    starModel->draw();
  }

  starModel->endDraw();

  defaultShaderProgram->endUse();

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

void OpenGLWidget::update() {
  float deltaTime = stopwatch.restart() / 1000.0f;

  if (player->state != Player::Dead) {
    player->update(deltaTime);
    obstacle->update(deltaTime);
    obstacle->velocityX -= 0.1f * deltaTime;

    for (int i = 0; i < stars.size(); i++) {
      stars[i].update(deltaTime);
      stars[i].velocityX -= 0.1f * deltaTime;
    }

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

void OpenGLWidget::createShaders() {
  defaultShaderProgram = new ShaderProgram(this);
  defaultShaderProgram->create(":/shaders/vshader.glsl",
                               ":/shaders/fshader.glsl");
}

void OpenGLWidget::createModels() {
  starModel = new Model(this);
  squareModel = new Model(this);

  QVector<QVector4D> starVertices = {QVector4D(0, sqrtf(3) / 3, 0, 1),
                                     QVector4D(0.5, -sqrtf(3) / 6, 0, 1),
                                     QVector4D(-0.5, -sqrtf(3) / 6, 0, 1),
                                     QVector4D(0, -sqrtf(3) / 3, 0, 1),
                                     QVector4D(0.5, sqrtf(3) / 6, 0, 1),
                                     QVector4D(-0.5, sqrtf(3) / 6, 0, 1)};
  QVector<GLuint> starIndices = {0, 1, 2, 3, 4, 5};

  QVector<QVector4D> squareVertices = {
      QVector4D(-1, -1, 0, 1), QVector4D(1, -1, 0, 1), QVector4D(1, 1, 0, 1),
      QVector4D(-1, 1, 0, 1)};
  QVector<GLuint> squareIndices = {0, 1, 2, 2, 3, 0};

  starModel->create(starVertices, starIndices);
  squareModel->create(squareVertices, squareIndices);
}
