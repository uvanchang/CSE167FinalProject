#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>
#include <algorithm> 
#include <fstream>
#include <iostream>

#include "shader.h"
#include "texture.h"
#include "Debug.h"

class Skybox
{
protected:
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> textures;
	std::vector<glm::ivec3> indices;
	GLuint shaderId;
	GLuint textureId;
	GLuint vao, ebo;
	GLuint vbos[3];
public:
	Skybox(std::string textureFilePath, GLuint shaderId);
	~Skybox();

	void render();
	void update();

	GLuint getTextureId() { return textureId; }
};

#endif