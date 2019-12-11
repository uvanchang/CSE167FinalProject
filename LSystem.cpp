#include "LSystem.h"

LSystem::LSystem(float angle, int iterations, GLuint shader)
    : angle(angle), shader(shader)
{
    trees = new std::vector<std::string>();
    depth = 0;
    length = 5;
    rule = "X";
    
    for(int i = 0; i <= iterations; i++){
        genRule();
    }
    
    model = glm::mat4(1);
    material.ambient = glm::vec3(1.0f, 0.5f, 0.31f);
    material.diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
    material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    material.shininess = 32.0f;
    
    std::string ch = "";
    std::string LSystem = trees->at(depth);
    for (int i = 0; i < LSystem.length(); i++){
        ch = LSystem.at(i);
        
        if (ch.compare("F") == 0 || ch.compare("X") == 0){
            moveForward();
        } else if (ch.compare("[") == 0){
            push();
        } else if (ch.compare("]") == 0){
            pop();
        } else if (ch.compare("+") == 0){
            rotR();
        } else if (ch.compare("-") == 0){
            rotL();
        }
    }
}

LSystem::~LSystem() {
    delete trees;
}

void LSystem::push(){
    glPushMatrix();
}

void LSystem::pop(){
    glPopMatrix();
}

void LSystem::rotL(){
    glRotatef(angle, 1, 0, 0);
    glRotatef(angle * 4, 0, 1, 0);
    glRotatef(angle, 0, 0, 1);
}
void LSystem::rotR(){
    glRotatef(-angle, 1, 0, 0);
    glRotatef(angle * 4, 0, 1, 0);
    glRotatef(-angle, 0, 0, 1);
}

void LSystem::moveForward(){
    glBegin(GL_LINES);
    
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(length, 0, 0);
    glEnd();
    
    glTranslatef(0, length, 0);
}

void LSystem::genRule()
{
    std::string ch = "";
    
    for (int i = 0; i < rule.length(); i++){
        ch = rule.at(i);
        
        if (ch.compare("F") == 0){
            rule.replace(i, 1, "FF");
            i = i + 1;
        } else if (ch.compare("X") == 0){
            rule.replace(i, 1, "F[-X]F[+X]-X");
            i = i + 11;
        }
        
    }
    trees->push_back(rule);
}

void LSystem::render()
{
    glUseProgram(shader);
    
    
    
    glm::mat4 model(1.0f);
    glm::vec3 color(1.0f, 0.0f, 0.0f); // Yellow
    GLuint modelLoc = glGetUniformLocation(shader, "model");
    GLuint colorLoc = glGetUniformLocation(shader, "color");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(colorLoc, 1, glm::value_ptr(color));
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glBindVertexArray(0);
    
    draw();
    
    
}
