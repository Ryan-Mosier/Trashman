//
// Created by ryanm on 4/13/2023.
//

#ifndef MYPROJECT_EBO_H
#define MYPROJECT_EBO_H

#include <glad/glad.h>

class EBO {
public:
    EBO();
    EBO(int){}

    GLuint ID;

    void Data(GLuint *indices, GLsizeiptr size);
    void Bind();

    void Unbind();

    void Delete();
};


#endif //MYPROJECT_EBO_H
