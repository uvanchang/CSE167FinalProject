#ifndef _WINSCREEN_H
#define _WINSCREEN_H

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <math.h>

#include "Debug.h"
#include "shader.h"
#include "texture.h"

class WinScreen
{
protected:
    glm::mat4 model;
    glm::vec3 color;
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textures;
    std::vector<unsigned int> indices;
    GLuint shader;
    GLuint textureId;
    GLuint vao;
    GLuint vbos[2];
public:
    WinScreen(GLuint shader);
    ~WinScreen();
    
    void render();
    
    glm::mat4 getModel() { return model; }
    void setModel(glm::mat4 model) { this->model = model; }
};

#endif
