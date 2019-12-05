#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

class Skybox
{
private:
    GLuint vao;
    GLuint vbo;
    GLuint shader;
public:
    Skybox(std::vector<std::string> facesFilenames, GLuint shader);
    ~Skybox();
    void render();
    void update();
    static GLuint cubemapTexture;
};

#endif
