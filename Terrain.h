#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

#include "shader.h"

class Terrain
{
protected:
	glm::mat4 model;
	glm::vec3 color;
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textures;
	std::vector<unsigned int> indices;
	GLuint shader;
	GLuint vao;
	GLuint vbo;
	std::vector<std::vector<int>> terrain;
public:
	Terrain(int n, GLuint shader);
	~Terrain();

	void render();
	void update();

	glm::mat4 getModel() { return model; }
	glm::vec3 getColor() { return color; }
};

#endif