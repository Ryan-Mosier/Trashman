//
// Created by ryanm on 4/11/2023.
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "rendering/stb_image.h"
#include <cmath>

#include "rendering/VAO.h"
#include "rendering/VBO.h"
#include "rendering/shaderClass.h"
#include "rendering/EBO.h"

GLFWwindow *window;
GLuint  uniID;
VAO VAO1(1);
EBO EBO1;
VBO VBO1;
Shader shaderProgram;

void setup() {
    //start openGL
    glfwInit();

    //specify version of openGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    //define some verticies
    GLfloat verticies[] = {
            -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, 0.8f, 0.3f, 0.02f,       //lower left        0
            0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, 0.8f, 0.3f, 0.02f,       //lower right       1
            0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, 1.0f, 0.6f, 0.32f,       //top               2
            -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, 0.9f, 0.45f, 0.17f,       //inner left        3
            0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, 0.9f, 0.45f, 0.17f,       //inner right       4
            0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f, 0.8f, 0.3f, 0.02f,       //bottom middle     5
    };

    GLuint indicies[] = {
            0, 3, 5, // lower left triangle
            3, 2, 4, // lower right triangle
            5, 4, 1 // upper triangle
    };

    //create a window
    window = glfwCreateWindow(800, 800, "TrashMan", NULL, NULL);
    //failsafe
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    //set window to current context
    glfwMakeContextCurrent(window);

    //start glad
    gladLoadGL();


    //make the viewport from (0,0) to (800,800)
    glViewport(0, 0, 800, 800);


    shaderProgram = Shader("rendering/shaders/default.vert", "rendering/shaders/default.frag");
    //create reference containers for the vertex array object and the vertex buffer object
    VAO1 = VAO();
    VAO1.Bind();

    VBO1 = VBO(verticies, sizeof(verticies));
    EBO1 = EBO(indicies, sizeof(indicies));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void *) 0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    uniID = glGetUniformLocation(shaderProgram.ID,"scale");

    //specify color of background
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);


}

void shutdown() {
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();

}

void renderFrame() {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shaderProgram.Activate();
    glUniform1f(uniID,0.5f);
    VAO1.Bind();
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, nullptr);


    glfwSwapBuffers(window);
}

int main() {
    setup();

    while (!glfwWindowShouldClose(window)) {
        renderFrame();

        //take care of all GLFW events
        glfwPollEvents();
    }

    shutdown();
}