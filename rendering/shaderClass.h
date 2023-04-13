//
// Created by ryanm on 4/13/2023.
//

#ifndef MYPROJECT_SHADERCLASS_H
#define MYPROJECT_SHADERCLASS_H

#include <glad//glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string getFileConstants(const char* filename);

class Shader{
public:
    Shader();

    GLuint ID;
    Shader(const char* vertexFile, const char* fragmnetFile);

    void Activate();
    void Delete();
};

#endif //MYPROJECT_SHADERCLASS_H
