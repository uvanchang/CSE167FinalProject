#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

#include "Node.h"
#include "shader.h"

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

class Geometry : public Node
{
protected:
	glm::mat4 model;
	glm::vec3 color;
	Material material;
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textures;
	std::vector<unsigned int> indices;
	GLuint program;
	GLuint textureId = -1;
	GLuint vao, ebo;
	GLuint vbos [2];
	int toRender = 1;
	std::vector<glm::vec3> offsets;
public:
	Geometry(std::string objFilename, GLuint shader);
	~Geometry();

	void parseOBJ(std::string objFilename);

	void setModelMatrix(glm::mat4 M);
	void render();
	void draw(glm::mat4 C);
	void update();

	void setOffsets(std::vector<glm::vec3> offsets) { this->offsets = offsets; }
	void setMaterial(Material material) { this->material = material; }
	void toggleRender(int toRender) { this->toRender = toRender; }
	void setTextureSampler(GLuint textureId) { this->textureId = textureId; }

	glm::mat4 getModel() { return model; }
	glm::vec3 getColor() { return color; }
	Material getMaterial() { return material; }
};

#endif