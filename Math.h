#ifndef _MATH_H_
#define _MATH_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

class Math
{
public:
	static float euclideanDistance(glm::vec3 point)
	{
		return sqrt(pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2));
	}

	static bool doubleEquals(double a, double b, double epsilon = 0.001) {
		return std::abs(a - b) < epsilon;
	}

	/*
	 * Calculates point on a tangent line at a given distance away from an initial point on a circle
	 *
	 * Chooses point in the clock-wise direction (unless otherwise specified)
	 */
	static glm::vec2 pointOnTangent(double angle, double radius, double distance, bool reverseDirection=false) {
		double x0 = cos(glm::radians(angle)) * radius;
		double y0 = sin(glm::radians(angle)) * radius;
		double m = -1 / tan(glm::radians(angle));
		double b = m * (-x0) + y0; // Equation: m = (b - y0)/(0 - x0)

		double x1;
		double y1;

		if (angle > 180.0 && angle < 360.0) { // Reverse x direction on slope when below circle
			x1 = x0 - 1;
		} else {
			x1 = x0 + 1;
		}
		y1 = m * x1 + b;

		if (Math::doubleEquals(angle, 0.0) || Math::doubleEquals(angle, 360.0)) { // Manually define for infinite slope
			x1 = x0;
			y1 = y0 - distance;	
		} else if (Math::doubleEquals(angle, 180.0)) {
			x1 = x0;
			y1 = y0 + distance;
		}

		glm::vec2 tangentVector = glm::normalize(glm::vec2(x1 - x0, y1 - y0));
		tangentVector = glm::vec2(tangentVector.x * distance, tangentVector.y * distance);

		if (reverseDirection)
			return glm::vec2(x0, y0) - tangentVector;
		else
			return glm::vec2(x0, y0) + tangentVector;
	}

    // Checks if boundary of one sphere is within the boundary of another
	static bool isInsideSphere(glm::vec3 point, double radius, glm::vec3 origin) {
		return euclideanDistance(point - origin) < 2 * radius;
	}
};

#endif
