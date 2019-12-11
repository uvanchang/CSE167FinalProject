#include "LSystem.h"

LSystem::LSystem(glm::vec3 rootPos, float angle, int iterations, GLuint shader)
    : angle(angle), shader(shader)
{
    currBranch.angle = glm::vec3(0, 1, 0);
    currBranch.position = rootPos;
    depth = 0;
    length = 5;
    rule = "X";
    
    for(int i = 0; i <= iterations; i++){
        genRule();
    }
    
    model = glm::mat4(1);
    
    std::string ch = "";
    for (int i = 0; i < rule.length(); i++){
        ch = rule.at(i);
        
        if (ch.compare("F") == 0){
            moveForward();
            branchLength *= 0.999999999;
        } else if (ch.compare("[") == 0) {
            push(currBranch);
        } else if (ch.compare("]") == 0) {
            pop();
        } else if (ch.compare("+") == 0) {
            rotR();
        } else if (ch.compare("-") == 0) {
            rotL();
        }
    }
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

LSystem::~LSystem()
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void LSystem::push(Branch b)
{
    stack.push_back(b);
}

void LSystem::pop()
{
    currBranch = stack.back();
    stack.pop_back();
}

void LSystem::rotL()
{
    currBranch.angle = glm::rotateX(currBranch.angle, angle);
    currBranch.angle = glm::rotateZ(currBranch.angle, (std::rand() % 20 - 10.0f) * (float)(M_PI / 180.0f));
}

void LSystem::rotR()
{
    currBranch.angle = glm::rotateX(currBranch.angle, -angle);
    currBranch.angle = glm::rotateZ(currBranch.angle, (std::rand() % 20 - 10.0f) * (float)(M_PI / 180.0f));
}

void LSystem::moveForward()
{
    points.push_back(currBranch.position);
    currBranch.position += currBranch.angle * branchLength;
    points.push_back(currBranch.position);
}

void LSystem::genRule()
{
    std::string ch = "";
    
    for (int i = 0; i < rule.length(); i++){
        ch = rule.at(i);
        
        if (ch.compare("F") == 0){
            std::string rule1 = "FF";
            rule.replace(i, 1, rule1);
            i += rule1.length() - 1;
        }
        else if (ch.compare("X") == 0){
            std::string rule2 = "F-[[X]+X]+F[+FX]-X";
            rule.replace(i, 1, rule2);
            i += rule2.length() - 1;
        }
        
    }
}

void LSystem::render()
{
    glUseProgram(shader);
    
    glm::vec3 color(0.0f, 1.0f, 0.0f);
    GLuint modelLoc = glGetUniformLocation(shader, "model");
    GLuint colorLoc = glGetUniformLocation(shader, "color");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(colorLoc, 1, glm::value_ptr(color));
    
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, points.size());
    glBindVertexArray(0);
}
