//
// Created by ryanm on 4/13/2023.
//

#include "EBO.h"


void EBO::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() {
    glDeleteBuffers(1, &ID);
}

void EBO::Data(GLuint *indices, GLsizeiptr size) {
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

EBO::EBO() {
    glGenBuffers(1, &ID);
}
