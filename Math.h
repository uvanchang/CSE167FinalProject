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
};

#endif