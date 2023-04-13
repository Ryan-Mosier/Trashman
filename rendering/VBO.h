//
// Created by ryanm on 4/13/2023.
//

#ifndef MYPROJECT_VBO_H
#define MYPROJECT_VBO_H

#include <glad/glad.h>

class VBO {
public:
    VBO();

    GLuint ID;
    VBO(GLfloat* vertices, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
};


#endif //MYPROJECT_VBO_H
