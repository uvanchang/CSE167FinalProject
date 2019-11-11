#include <algorithm> 
#include <fstream>
#include <iostream>

#include "Transform.h"

Transform::Transform(glm::mat4 M) 
{
	transformMatrix = M;
}

Transform::Transform() 
{
	transformMatrix = glm::mat4(1.0);
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

}