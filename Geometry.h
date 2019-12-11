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

#include "Debug.h"
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
	Material material;
	glm::vec3 color;
	int useColor;
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textures;
	std::vector<unsigned int> indices;
	GLuint program;
	GLuint textureId = -1;
	GLuint vao, ebo;
	GLuint vbos [3];
	int toRender = 1;
	std::vector<glm::vec3> offsets;
public:
	Geometry(std::string objFilename, GLuint shader);
	~Geometry();

	void parseOBJ(std::string objFilename);

	void render();
	void draw(glm::mat4 C);
	void update();

	void setModelMatrix(glm::mat4 M) { this->model = M; };
	void setOffsets(std::vector<glm::vec3> offsets) { this->offsets = offsets; }
	void setMaterial(Material material) { this->material = material; }
	void setColor(glm::vec3 color) { this->color = color; }
	void useSetColor(int useColor) { this->useColor = useColor; }
	void toggleRender(int toRender) { this->toRender = toRender; }
	void setTextureSampler(GLuint textureId) { this->textureId = textureId; }

	glm::mat4 getModel() { return model; }
	Material getMaterial() { return material; }
};

#endif
