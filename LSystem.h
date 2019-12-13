#ifndef _LSYSTEM_H
#define _LSYSTEM_H

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
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

struct Rule
{
    std::string lhs;
    std::string rhs;
};

class LSystem
{
private:
    std::vector<glm::vec3> points;
    std::vector<float> angles;
    float length;
    std::vector<Rule> ruleset;
    std::string generatedRule;
    std::vector<Branch> stack;
    Branch currBranch;
    GLuint shader;
    GLuint vao, vbo;
    glm::mat4 model;
    float branchLength = 0.05f;
    
    std::vector<Branch> tree;
public:
    LSystem(glm::vec3 rootPos, std::vector<float> angles, std::string axiom, std::vector<Rule> ruleset, int iterations, GLuint shader);
    LSystem(float angle);
    ~LSystem();
    void push(Branch b);
    void pop();
    void rotateXL();
    void rotateXR();
    void rotateYL();
    void rotateYR();
    void rotateZL();
    void rotateZR();
    void moveForward();
    void genRule();
    void display();
    void render();
};

#endif
