#ifndef _BEZIER_CURVE_H_
#define _BEZIER_CURVE_H_

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

#include "Geometry.h"
#include "Constants.h"
#include "Debug.h"

class BezierCurve
{
protected:;
	glm::mat4 Gbez; // geometry matrix (control points) 
 	glm::mat4 Bbez; // geometry basis
 	glm::mat4 C; // Gbez * Bbez

	glm::vec3 points[150];
	std::vector<unsigned int> indices;
	GLuint program;
	GLuint vao, vbo;
public:
	BezierCurve(glm::vec3 controlPoints[4], GLuint shader);
	~BezierCurve();

	void setControlPoints(glm::vec3 controlPoints[4]);
	void setBasisVectors();

	glm::vec3 parametricCurve(float t);
	glm::vec3 getPoint(float t);

	void render();
	void update();
};

#endif