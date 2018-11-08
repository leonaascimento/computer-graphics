#include "model.h"

Model::Model(QOpenGLWidget* glWidget) {
  m_glWidget = glWidget;
}

Model::~Model() {
  destroy();
  m_glWidget = nullptr;
}

void Model::create(QVector<QVector4D> vertices, QVector<GLuint> indices) {
  initializeOpenGLFunctions();

  glGenVertexArrays(1, &m_vaoId);
  glBindVertexArray(m_vaoId);
  glGenBuffers(1, &m_vboId);
  glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * static_cast<GLint>(sizeof(QVector4D)),
               vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(0);
  glGenBuffers(1, &m_iboId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               indices.size() * static_cast<GLint>(sizeof(unsigned int)),
               indices.data(), GL_DYNAMIC_DRAW);
  glBindVertexArray(0);
}

void Model::destroy() {
  glDeleteBuffers(1, &m_iboId);
  glDeleteBuffers(1, &m_vboId);
  glDeleteVertexArrays(1, &m_vaoId);

  m_iboId = 0;
  m_vboId = 0;
  m_vaoId = 0;
}

void Model::beginDraw() {
  glBindVertexArray(m_vaoId);
}

void Model::draw() {
  glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);
}

void Model::endDraw() {
  glBindVertexArray(0);
}
