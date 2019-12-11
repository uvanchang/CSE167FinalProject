#ifndef _LSYSTEM_H
#define _LSYSTEM_H

#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

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
    glm::vec3 position;
    glm::vec3 angle;
};

class LSystem
{
private:
    std::vector<glm::vec3> points;
    float angle, length;
    int depth;
    std::string rule;
    std::vector<Branch> stack;
    Branch currBranch;
    GLuint shader;
    GLuint vao, vbo;
    glm::mat4 model;
    Material material;
    float branchLength = 0.05f;
    
    std::vector<Branch> tree;
public:
    LSystem(glm::vec3 rootPos, float angle, int iterations, GLuint shader);
    LSystem(float angle);
    ~LSystem();
    void push(Branch b);
    void pop();
    void rotL();
    void rotR();
    void moveForward();
    void genRule();
    void display();
    void render();
};

#endif
