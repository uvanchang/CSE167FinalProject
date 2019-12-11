#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>

#include "Node.h"

class Transform : public Node
{
protected:
    int id;
	glm::mat4 transformMatrix;
	std::vector<Node*> children;
public:
	Transform(glm::mat4 M, int id);
	Transform();
	~Transform();

	void addChild(Node* n);
	void draw(glm::mat4 C);
	void update();

	void setTransformation(glm::mat4 TM) { transformMatrix = TM; }
	
	glm::mat4 getTransformation() { return transformMatrix; }
	std::vector<Node*> getChildren() { return children; }
};

#endif
