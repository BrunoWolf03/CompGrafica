#include "polygon.h"
#include <iostream>
#include <glad/glad.h>

PolygonPtr Polygon::Make(int n, float* xy, unsigned char* rgb, unsigned int* inc) {
	return PolygonPtr(new Polygon(n,xy,rgb,inc));
}

Polygon::Polygon(int n, float* xy, unsigned char* rgb, unsigned int* inc) {

    m_vao = 0;

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  GLuint id[3];
  glGenBuffers(3, id);

  glBindBuffer(GL_ARRAY_BUFFER, id[0]);
  glBufferData(GL_ARRAY_BUFFER, n * 2 * sizeof(float), xy, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);  // coord
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, id[1]);
  glBufferData(GL_ARRAY_BUFFER, n * 3 * sizeof(unsigned char), rgb, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);  // color
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, n*sizeof(GLuint), inc, GL_STATIC_DRAW);
}

Polygon::~Polygon() {
    glDeleteVertexArrays(1, &m_vao);
}

void Polygon::Draw() {
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLE_FAN, 5, GL_UNSIGNED_INT, 0);  
}
