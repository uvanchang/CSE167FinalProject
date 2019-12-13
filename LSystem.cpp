#include "LSystem.h"

LSystem::LSystem(glm::vec3 rootPos, std::vector<float> angles, std::string axiom, std::vector<Rule> ruleset, int iterations, GLuint shader)
    : angles(angles), generatedRule(axiom), ruleset(ruleset), shader(shader)
{
    currBranch.angle = glm::vec3(0, 1, 0);
    currBranch.position = rootPos;
    length = 5;
    model = glm::mat4(1);
    
    for(int i = 0; i <= iterations; i++){
        genRule();
    }
    
    std::string ch = "";
    for (int i = 0; i < generatedRule.length(); i++){
        ch = generatedRule.at(i);
        
        if (ch.compare("F") == 0){
            moveForward();
        } else if(ch.compare("[") == 0) {
            push(currBranch);
        } else if(ch.compare("]") == 0) {
            pop();
        } else if(ch.compare("\\") == 0) {
            rotateXL();
        } else if(ch.compare("/") == 0) {
            rotateXR();
        } else if(ch.compare("<") == 0) {
            rotateYL();
        } else if(ch.compare(">") == 0) {
            rotateYR();
        } else if(ch.compare("-") == 0) {
            rotateZL();
        } else if(ch.compare("+") == 0) {
            rotateZR();
        }
    }
    
    float randRotation = std::rand() % 360 * (float)(M_PI / 180.0f);
    for (int i = 0; i < points.size(); i++) {
        points[i] = glm::rotateY(points[i], randRotation);
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

void LSystem::rotateXL()
{
    currBranch.angle = glm::rotateX(currBranch.angle,
                                    angles[0] + (std::rand() % 100 / 100.0f) * (float)(M_PI / 180.0f) - 0.5f * (float)(M_PI / 180.0f));
}

void LSystem::rotateXR()
{
    currBranch.angle = glm::rotateX(currBranch.angle,
                                    -angles[0] + (std::rand() % 100 / 100.0f) * (float)(M_PI / 180.0f) - 0.5f * (float)(M_PI / 180.0f));
}

void LSystem::rotateYL()
{
    currBranch.angle = glm::rotateY(currBranch.angle,
                                    angles[1] + (std::rand() % 100 / 100.0f) * (float)(M_PI / 180.0f) - 0.5f * (float)(M_PI / 180.0f));
}

void LSystem::rotateYR()
{
    currBranch.angle = glm::rotateY(currBranch.angle,
                                    -angles[1] + (std::rand() % 100 / 100.0f) * (float)(M_PI / 180.0f) - 0.5f * (float)(M_PI / 180.0f));
}

void LSystem::rotateZL()
{
    currBranch.angle = glm::rotateZ(currBranch.angle,
                                    angles[2] + (std::rand() % 100 / 100.0f) * (float)(M_PI / 180.0f) - 0.5f * (float)(M_PI / 180.0f));
}

void LSystem::rotateZR()
{
    currBranch.angle = glm::rotateZ(currBranch.angle,
                                    -angles[2] + (std::rand() % 100 / 100.0f) * (float)(M_PI / 180.0f) - 0.5f * (float)(M_PI / 180.0f));
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
    
    for (int i = 0; i < generatedRule.length(); i++) {
        ch = generatedRule.at(i);
        
        for (int j = 0; j < ruleset.size(); j++) {
            if (ch.compare(ruleset[j].lhs) == 0){
                generatedRule.replace(i, 1, ruleset[j].rhs);
                i += ruleset[j].rhs.length() - 1;
            }
        }
    }
}

void LSystem::render()
{
    glUseProgram(shader);
    
    glm::vec3 color(0.0f, 100.0f / 255.0f, 0.0f);
    GLuint modelLoc = glGetUniformLocation(shader, "model");
    GLuint colorLoc = glGetUniformLocation(shader, "color");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(colorLoc, 1, glm::value_ptr(color));
    
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, points.size());
    glBindVertexArray(0);
}
