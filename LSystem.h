#ifndef _LSYSTEM_H
#define _LSYSTEM_H

#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <GLUT/GLUT.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>

#include "Geometry.h"

struct Branch
{
    float position;
    float angle;
};

class LSystem
{
private:
    float angle, length;
    int depth;
    std::string rule;
    std::vector<std::string>* trees;
    std::vector<Branch> stack;
    GLuint shader;
    GLuint vao;
    glm::mat4 model;
    Material material;
    
    std::vector<Branch> tree;
public:
    LSystem(float angle, int iterations, GLuint shader);
    LSystem(float angle);
    ~LSystem();
    void push();
    void pop();
    void rotL();
    void rotR();
    void moveForward();
    void genRule();
    void draw();
    void display();
    void render();
};

#endif
