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
#include <math.h>

#include "Debug.h"
#include "shader.h"
#include "texture.h"

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
    GLuint textureId;
	GLuint vao;
	GLuint vbos[2];
	std::vector<std::vector<int>> terrain;
    int scale = 5;
public:
	Terrain(int n, GLuint shader);
	~Terrain();
    void diamondSquare(int n, std::vector<std::pair<int, int>> cornerPoints);
    float avgPlusRandom(std::vector<std::pair<int, int>> cornerPoints, int n);

	void render();
	void update();

	glm::mat4 getModel() { return model; }
	glm::vec3 getColor() { return color; }
    float getTerrainHeight(float x, float z);
};

#endif
