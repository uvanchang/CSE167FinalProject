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

struct ControlPoint {
	glm::vec3 coordinates;
	glm::vec3 og_color;
	Geometry* pointObject;
	ControlPoint* left;
	ControlPoint* right;

	ControlPoint(glm::vec3 point, std::string objFilepath, GLuint shaderId, glm::vec3 color)
		: coordinates(point), og_color(color)
	{
		pointObject = new Geometry(objFilepath, shaderId);
		pointObject->setModelMatrix(glm::translate(pointObject->getModel(), coordinates));
		pointObject->setModelMatrix(glm::scale(pointObject->getModel(), glm::vec3(0.1f, 0.1f, 0.1f)));
		pointObject->setColor(color);
	}
	~ControlPoint() {
		delete pointObject;
		delete left;
		delete right;
	}
	glm::vec3 translateCoordinates(glm::vec3 xyz) { 
		coordinates = glm::vec3(coordinates.x + xyz.x, coordinates.y + xyz.y, coordinates.z + xyz.z);

		pointObject->setModelMatrix(glm::scale(pointObject->getModel(), glm::vec3(10.0f, 10.0f, 10.0f)));
		pointObject->setModelMatrix(glm::translate(pointObject->getModel(), xyz));
		pointObject->setModelMatrix(glm::scale(pointObject->getModel(), glm::vec3(0.1f, 0.1f, 0.1f)));

		return coordinates;
	}
	glm::vec3 getCoordinates() { return coordinates; }
	Geometry* getObject() { return pointObject; }
	ControlPoint* getRight() { return right; }
	ControlPoint* getLeft() { return left; }
	void setColor(glm::vec3 color) { pointObject->setColor(color); }
	void resetColor() { pointObject->setColor(og_color); }
	void setLeft(ControlPoint* L) { left = L; }
	void setRight(ControlPoint* R) { right = R; }
	void update() { pointObject->update(); }
	void render() { pointObject->render(); }
};

class Track
{
protected:
	std::vector<BezierCurve*> curves;
	std::vector<ControlPoint*> allControlPoints;
	std::vector<ControlPoint*> anchorPoints;
	std::vector<ControlPoint*> tangentPoints;
	ControlPoint* currentControlPoint;
	unsigned int currentCurveIndex;
	unsigned int currentCurvePointIndex;
	GLuint program;
	GLuint vao, vbo;
public:
	Track(GLuint shader, glm::vec3 center, float radius);
	~Track();

	void changeCurrentControlPoint(bool right);
	void shiftCurrentControlPoint(glm::vec3 translation);

	ControlPoint* getCurrentControlPoint() { return currentControlPoint; }
	BezierCurve* getCurve(int curveIndex) { return curves[curveIndex]; }

	void render();
	void update();
};

#endif