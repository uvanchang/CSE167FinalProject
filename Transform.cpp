#include <algorithm> 
#include <fstream>
#include <iostream>

#include "Transform.h"

Transform::Transform(glm::mat4 M, int id)
{
	transformMatrix = M;
    this->id = id;
}

Transform::Transform() 
{
	transformMatrix = glm::mat4(1.0);
    id = 0;
}

Transform::~Transform()
{
	for (Node* child : children) {
		delete child;
	}
}

void Transform::addChild(Node* n) 
{
	children.push_back(n);
}

void Transform::draw(glm::mat4 C) 
{
	glm::mat4 M = C * transformMatrix; // convert to defined coordinate system
	for (Node* child : children) {
		child->draw(M);
	}
}

void Transform::update() 
{
    if (id == 1) {
        transformMatrix = glm::rotate(transformMatrix, 0.01f, glm::vec3(0.0f, 0.0f, 1.0f));
    }
    for (Node* child : children) {
        child->update();
    }
}
