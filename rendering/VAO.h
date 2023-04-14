//
// Created by ryanm on 4/13/2023.
//

#ifndef MYPROJECT_VAO_H
#define MYPROJECT_VAO_H


#include <glad/glad.h>
#include "VBO.h"

class VAO {
public:
    GLuint ID;
    VAO();
    VAO(int){}

    void LinkAttrib(VBO VBO, GLuint layout,GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
    void Bind();
    void Unbind();
    void Delete();
};


#endif //MYPROJECT_VAO_H
