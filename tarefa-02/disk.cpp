#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include "disk.h"

Disk::Disk(int nslice) : m_nslice(nslice) {
    // Gerar os vértices do círculo com cores
    std::vector<float> vertices;
    float angle_step = 2.0f * 3.14159 / m_nslice;
    vertices.push_back(0.0f); // x
    vertices.push_back(0.0f); // y
   
    for (int i = 0; i <= m_nslice; ++i) {
        float angle = i * angle_step;
        float x = cos(angle);
        float y = sin(angle);
        vertices.push_back(x);
        vertices.push_back(y);
    }

    // Gerar VAO e VBO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Atribuir os atributos dos vértices (posição)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);  // Desvincular VAO
}

Disk::~Disk() {
    glDeleteVertexArrays(1, &m_vao);
}

DiskPtr Disk::Make(int nslice) {
    return DiskPtr(new Disk(nslice));
}

void Disk::Draw() {
    // Vincula o VAO do disco e desenha
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, m_nslice + 2);
    glBindVertexArray(0);
}
