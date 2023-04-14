//
// Created by ryanm on 4/13/2023.
//

#include "frameCreator.h"

frameCreator::frameCreator(GLFWwindow *window, VBO VBO1, VAO VAO1, EBO EBO1) : window(window), VBO1(VBO1), VAO1(VAO1),
                                                                               EBO1(EBO1) {
    //make the viewport from (0,0) to (800,800)
    glViewport(0, 0, 1920, 1080);


    shaderProgram = Shader("rendering/shaders/default.vert", "rendering/shaders/default.frag");
    //create reference containers for the vertex array object and the vertex buffer object


    uniID = glGetUniformLocation(shaderProgram.ID, "scale");
}

void frameCreator::generateFrame() {
    convertData();
    generateIndicies();

    VAO1.Bind();


    VBO1.Data(verticies, sizeof(verticies));
    EBO1.Data(indicies, sizeof(indicies));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void *) 0); //positions
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void *) (3 * sizeof(float))); //colors
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();
}

frameCreator::~frameCreator() {
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();
    delete[]verticies;
}

void frameCreator::readData(std::string filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        exit(-10);
    }
    std::string line;
    getline(in, line);
    std::vector<int> size;
    std::string token;
    std::stringstream ss(line);
    while (std::getline(ss, token, ',')) {
        size.push_back(std::stoi(token));
    }
    vxsize = size[2];
    vysize = size[1];
    for (int frame = 0; frame < size[0]; frame++) {
        for (int y = 0; y < vysize; y++) {
            getline(in, line);
            ss = std::stringstream(line);
            std::vector<vertex> vector;
            for (int x = 0; x < vxsize; x++) {
                uint32_t pixel;
                std::getline(ss, token, ',');
                pixel = std::strtoul(token.c_str(), nullptr, 16);
                uint8_t r, g, b;
                r = (pixel & 0xff0000) >> 16;
                g = (pixel & 0x00ff00) >> 8;
                b = (pixel & 0x0000ff);
                float xPos, yPos, zPos = 0.0f;
                xPos = (float) x * 0.1f;
                yPos = (float) y * 0.1f;
                vector.push_back(vertex(xPos, yPos, zPos, (float) r / 255.0f, (float) g / 255.0f, (float) b / 255.0f));
            }
            vertexVec.push_back(vector);
        }
    }
}

void frameCreator::convertData() {
    verticies = new GLfloat[vxsize * vysize * 6];
    int idx = 0;
    for (auto vec: vertexVec) {
        for (auto v: vec) {
            verticies[idx] = v.x;
            verticies[idx + 1] = v.y;
            verticies[idx + 2] = v.z;
            verticies[idx + 3] = v.R;
            verticies[idx + 4] = v.G;
            verticies[idx + 5] = v.B;
            idx += 6;
        }
    }
}

void frameCreator::generateIndicies() {
    std::vector<unsigned int> ind;
    for (int i = 0; i < vysize - 1; i++) {
        for (int j = 0; j < vxsize - 1; j++) {
            int index = i * vxsize + j;
            ind.push_back(index);
            ind.push_back(index + vxsize + 1);
            ind.push_back(index + vxsize);

            ind.push_back(index);
            ind.push_back(index + 1);
            ind.push_back(index + vxsize + 1);
        }
    }
    isize = ind.size();
    indicies = new GLuint[isize];
    for (int i = 0; i < isize; i++) {
        indicies[i] = ind[i];
    }
}

void frameCreator::renderFrame() {


    VBO1.Bind();
    VAO1.Bind();
//    EBO1.Bind(); implicit in Data()
    EBO1.Data(indicies, isize);

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void *) 0); //positions
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void *) (3 * sizeof(float))); //colors
    VBO1.Unbind();


    shaderProgram.Activate();
    glUniform1f(uniID, 2.0f);
    VAO1.Bind();
    glDrawElements(GL_TRIANGLES, isize, GL_UNSIGNED_INT, 0);


    glfwSwapBuffers(window);
}
