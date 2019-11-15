#include "Track.h"

Track::Track(GLuint shader, glm::vec3 center, float radius)
	: program(shader)
{
	float offset = 360.0f / (8 * 3);
	for (unsigned int i = 0; i < 8; ++i) {
		float angle = i * offset * 3;
		// Compute locations of 4 control points for the bezier curve segment (3rd order)

		glm::vec3 p0 = glm::vec3(radius * cos(glm::radians(angle)) + center.x, 2.0f + center.y, radius * sin(glm::radians(angle)) + center.z);
		glm::vec3 p3 = glm::vec3(radius * cos(glm::radians(angle + 3 * offset)) + center.x, 2.0f + center.y, radius * sin(glm::radians(angle + 3 * offset)) + center.z);
		glm::vec2 pointOnP0Tangent = Math::pointOnTangent(angle, radius, 2, true);
		glm::vec2 pointOnP3Tangent = Math::pointOnTangent(angle + 3 * offset, radius, 2, false);
		glm::vec3 p1 = glm::vec3(pointOnP0Tangent.x + center.x, 0.0f + center.y, pointOnP0Tangent.y + center.z);
		glm::vec3 p2 = glm::vec3(pointOnP3Tangent.x + center.x, 4.0f + center.y, pointOnP3Tangent.y + center.z);
		glm::vec3 controlPoints[4] = {p0, p1, p2, p3};

		// Create curve
		BezierCurve * curve = new BezierCurve(controlPoints, shader);
		curves.push_back(curve);
			
		// Initialize anchor points and tangent points
		glm::vec3 red(1.0f, 0.0f, 0.0f);
		glm::vec3 green(0.0f, 1.0f, 0.0f);
		anchorPoints.push_back(new ControlPoint(controlPoints[0], SPHERE_OBJ, shader, red));
		tangentPoints.push_back(new ControlPoint(controlPoints[1], SPHERE_OBJ, shader, green));
		tangentPoints.push_back(new ControlPoint(controlPoints[2], SPHERE_OBJ, shader, green));
		allControlPoints.push_back(anchorPoints[anchorPoints.size() - 1]);
		allControlPoints.push_back(tangentPoints[tangentPoints.size() - 2]);
		allControlPoints.push_back(tangentPoints[tangentPoints.size() - 1]);

		// Connect curve segments
		for (unsigned int i = 0; i < allControlPoints.size(); ++i) {
			// Set left pointers
			if (i == 0) {
				allControlPoints[i]->setLeft(allControlPoints[allControlPoints.size() - 1]);
			} else {
				allControlPoints[i]->setLeft(allControlPoints[i - 1]);
			}
			// Set right pointers
			if (i == allControlPoints.size() - 1) {
				allControlPoints[i]->setRight(allControlPoints[0]);
			} else {
				allControlPoints[i]->setRight(allControlPoints[i + 1]);
			}
		}
	}

	// Set initial current control point
	currentCurveIndex = 0;
	currentCurvePointIndex = 0;
	currentControlPoint = allControlPoints[0];
	currentControlPoint->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
}

Track::~Track()
{
	for (BezierCurve * curve : curves) {
		delete curve;
	}

	for (ControlPoint * controlPoint : allControlPoints) {
		delete controlPoint;
	}
}

void Track::changeCurrentControlPoint(bool right)
{
	// Update pointer to current control point
	currentControlPoint->resetColor();
	currentControlPoint = right ? currentControlPoint->getRight() : currentControlPoint->getLeft();
	currentControlPoint->setColor(glm::vec3(1.0f, 1.0f, 1.0f));

	// Update current curve index and control point index on curve
	if (right) {	
		if (currentCurveIndex == curves.size() - 1)
			currentCurveIndex = currentCurvePointIndex == 2 ? 0 : currentCurveIndex;
		else
			currentCurveIndex = currentCurvePointIndex == 2 ? currentCurveIndex + 1 : currentCurveIndex;
		currentCurvePointIndex = currentCurvePointIndex == 2 ? 0 : currentCurvePointIndex + 1; 
	} else {
		if (currentCurveIndex == 0)
			currentCurveIndex = currentCurvePointIndex == 0 ? curves.size() - 1 : currentCurveIndex;
		else
			currentCurveIndex = currentCurvePointIndex == 0 ? currentCurveIndex - 1 : currentCurveIndex;
		currentCurvePointIndex = currentCurvePointIndex == 0 ? 2 : currentCurvePointIndex - 1; 
	}
}

/*
 *** Example diagram for reference
 *
 * (p0)---p1---p2---(p3)---p4---p5---(p6)
 *
 * Curve 1: p0, p1, p2, p3
 * Curve 2: p3, p4, p5, p6
 * Right tangent points: p1, p4
 * Left tangent points: p2, p5
 * Anchor points: p0, p3, p6
 */
void Track::shiftCurrentControlPoint(glm::vec3 translation)
{
	// Update current control point (on current curve)
	glm::vec3 newCoords = currentControlPoint->translateCoordinates(translation);
	curves[currentCurveIndex]->updateControlPoint(currentCurvePointIndex, newCoords);

	int prevCurveIndex = currentCurveIndex == 0 ? curves.size() - 1 : currentCurveIndex - 1;
	int nextCurveIndex = currentCurveIndex == curves.size() - 1 ? 0 : currentCurveIndex + 1;

	// Right tangent point
	if (currentCurvePointIndex == 1) {
		// Update anchor point to the left (on both current and previous curve)
		glm::vec3 newLeftCoords1 = currentControlPoint->getLeft()->translateCoordinates(translation * 0.5f);
		curves[currentCurveIndex]->updateControlPoint(0, newLeftCoords1);
		curves[prevCurveIndex]->updateControlPoint(3, newLeftCoords1);
		// glm::vec3 newLeftCoords2 = currentControlPoint->getLeft()->getLeft()->translateCoordinates(translation * -1.0f);
		// curves[prevCurveIndex]->updateControlPoint(2, newLeftCoords1);
	}
	// Left tangent point 
	else if (currentCurvePointIndex == 2) {
		// Update anchor point to the right (on both current and following curve)
		glm::vec3 newRightCoords1 = currentControlPoint->getRight()->translateCoordinates(translation * 0.5f);
		curves[currentCurveIndex]->updateControlPoint(3, newRightCoords1);
		curves[nextCurveIndex]->updateControlPoint(0, newRightCoords1);
		// glm::vec3 newRightCoords2 = currentControlPoint->getRight()->getRight()->translateCoordinates(translation * -1.0f);
		// curves[nextCurveIndex]->updateControlPoint(1, newRightCoords1);
	}
	// Anchor point
	else {
		// Update tangent points to the right and left (on both current and previous curve)
		glm::vec3 newRightCoords = currentControlPoint->getRight()->translateCoordinates(translation);
		glm::vec3 newLeftCoords = currentControlPoint->getLeft()->translateCoordinates(translation);
		curves[currentCurveIndex]->updateControlPoint(1, newRightCoords); // right tangent point on current curve
		curves[prevCurveIndex]->updateControlPoint(2, newLeftCoords); // left tangent point on previous curve
		curves[prevCurveIndex]->updateControlPoint(3, newCoords); // current control point on previous curve
	}
}

void Track::update()
{
	for (BezierCurve * curve : curves) {
		curve->update();
	}

	for (ControlPoint * controlPoint : allControlPoints) {
		controlPoint->update();
	}
}

void Track::render()
{
	// Draw curve segments
	for (BezierCurve * curve : curves) {
		curve->render();
	}

	// Draw control point spheres
	for (ControlPoint * controlPoint : allControlPoints) {
		controlPoint->render();
	}

	// Draw handles
	glUseProgram(program);
	glm::mat4 model(1.0f);
	glm::vec3 color(1.0f, 1.0f, 0.0f); // Yellow
	GLuint modelLoc = glGetUniformLocation(program, "model");
	GLuint colorLoc = glGetUniformLocation(program, "color");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBegin(GL_LINES);
	for (ControlPoint* anchorPoint : anchorPoints) {
		glm::vec3 center = anchorPoint->getCoordinates();
		glm::vec3 left = anchorPoint->getLeft()->getCoordinates();
		glm::vec3 right = anchorPoint->getRight()->getCoordinates();
		glVertex3f(left.x, left.y, left.z);
		glVertex3f(center.x, center.y, center.z);
		glVertex3f(center.x, center.y, center.z);
		glVertex3f(right.x, right.y, right.z);
	}
	glEnd();
	glBindVertexArray(0);
}