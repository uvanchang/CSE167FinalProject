#include "Track.h"

Track::Track(GLuint shader)
	: program(shader)
{
	float offset = 360.0f / (8 * 3);
	for (unsigned int i = 0; i < 8; ++i) {
		float angle = i * offset * 3;
		float radius = 10; 
		glm::vec3 p0 = glm::vec3(radius * cos(glm::radians(angle)), 0.0f, radius * sin(glm::radians(angle)));
		glm::vec3 p3 = glm::vec3(radius * cos(glm::radians(angle + 3 * offset)), 0.0f, radius * sin(glm::radians(angle + 3 * offset)));
		glm::vec2 pointOnP0Tangent = Math::pointOnTangent(angle, radius, 2, true);
		glm::vec2 pointOnP3Tangent = Math::pointOnTangent(angle + 3 * offset, radius, 2, false);
		glm::vec3 p1 = glm::vec3(pointOnP0Tangent.x, 0.0f, pointOnP0Tangent.y);
		glm::vec3 p2 = glm::vec3(pointOnP3Tangent.x, 0.0f, pointOnP3Tangent.y);
		glm::vec3 controlPoints[4] = {p0, p1, p2, p3};
			
		anchorPoints.push_back(controlPoints[0]);
		pullPoints.push_back(controlPoints[1]);
		pullPoints.push_back(controlPoints[2]);
		for (glm::vec3 controlPoint : controlPoints) {
			allControlPoints.push_back(controlPoint);
		}
		BezierCurve * curve = new BezierCurve(controlPoints, shader);
		curves.push_back(curve);
	}

	// Initialize anchor point spheres
	for (glm::vec3 anchorPoint : anchorPoints) {
		Geometry* anchor = new Geometry(SPHERE_OBJ, shader);
		anchor->setModelMatrix(glm::translate(anchor->getModel(), anchorPoint));
		anchor->setModelMatrix(glm::scale(anchor->getModel(), glm::vec3(0.1f, 0.1f, 0.1f)));
		anchor->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
		anchors.push_back(anchor);
	}

	// Initialize pull point spheres
	for (glm::vec3 pullPoint : pullPoints) {
		Geometry* pull = new Geometry(SPHERE_OBJ, shader);
		pull->setModelMatrix(glm::translate(pull->getModel(), pullPoint));
		pull->setModelMatrix(glm::scale(pull->getModel(), glm::vec3(0.1f, 0.1f, 0.1f)));
		pull->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
		pulls.push_back(pull);
	}
}

Track::~Track()
{
	for (BezierCurve * curve : curves) {
		delete curve;
	}
}

void Track::update()
{
	for (BezierCurve * curve : curves) {
		curve->update();
	}

	for (Geometry * anchor : anchors) {
		anchor->update();
	}

	for (Geometry * pull : pulls) {
		pull->update();
	}
}

void Track::render()
{
	// Draw curve segments
	for (BezierCurve * curve : curves) {
		curve->render();
	}

	// Draw anchor spheres
	for (Geometry * anchor : anchors) {
		anchor->render();
	}

	// Draw pull spheres
	for (Geometry * pull : pulls) {
		pull->render();
	}

	// Draw handles
	glUseProgram(program);
	glm::mat4 model(1.0f);
	GLuint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBegin(GL_LINES);
	for (unsigned int i = 0; i < allControlPoints.size(); ++i) {
		glVertex3f(allControlPoints[i].x, allControlPoints[i].y, allControlPoints[i].z);
	}
	glEnd();
	glBindVertexArray(0);
}