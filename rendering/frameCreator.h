//
// Created by ryanm on 4/13/2023.
//

#ifndef MYPROJECT_FRAMECREATOR_H
#define MYPROJECT_FRAMECREATOR_H

#include "EBO.h"
#include "VBO.h"
#include "VAO.h"
#include "shaderClass.h"
#include "GLFW/glfw3.h"
#include <glad/glad.h>

#include "fstream"
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

struct vertex {
    float x, y, z, R, G, B;

    vertex(float x, float y, float z, float r, float g, float b) : x(x), y(y), z(z), R(r), G(g), B(b) {}
};

class frameCreator {
public:
    GLFWwindow *window;
    GLuint uniID;
    VAO VAO1;
    EBO EBO1;
    VBO VBO1;
    Shader shaderProgram;

    int vxsize, vysize;
    GLfloat *verticies;
    std::vector<std::vector<vertex>> vertexVec;

    int isize;
    GLuint *indicies;

    frameCreator(GLFWwindow* win, VBO VBO1, VAO VAO1, EBO EBO1);

    ~frameCreator();

    void generateFrame();

    void renderFrame();

    void readData(std::string filename);

    void convertData();

    void generateIndicies();

};


#endif //MYPROJECT_FRAMECREATOR_H
