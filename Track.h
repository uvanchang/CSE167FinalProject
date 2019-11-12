#ifndef _TRACK_H_
#define _TRACK_H_

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

#include "BezierCurve.h"
#include "Debug.h"
#include "Geometry.h"
#include "Math.h"

class Track
{
protected:
	std::vector<BezierCurve*> curves;
	std::vector<glm::vec3> allControlPoints;
	std::vector<glm::vec3> anchorPoints;
	std::vector<Geometry*> anchors;
	std::vector<glm::vec3> pullPoints;
	std::vector<Geometry*> pulls;
	GLuint program;
	GLuint vao, vbo;
public:
	Track(GLuint shader);
	~Track();

	void render();
	void update();
};

#endif