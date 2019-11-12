#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GL/GLU.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <iostream>

class Debug
{
public:
	static void checkGLError(const char* msg) {
		const GLenum err = glGetError();
		if(err != GL_NO_ERROR) {
			const char* str= (const char*)gluErrorString(err);
			std::cerr<< "OpenGL error: " << msg << ", " << str << std::endl;
		}
	}

	static void printMatrix4(glm::mat4 M) {
		const float * flattened = (const float *)glm::value_ptr(M);
		std::cout << "[ " << std::endl;
		for (unsigned int i = 1; i <= 16; ++i) {
			if (i == 1) {
				std::cout << "  [ ";
			}
			std::cout << flattened[i-1] << " ";
			if (i % 4 == 0) {
				std::cout << "]";
				std::cout << std::endl;
				if (i != 16) {
					std::cout << "  [ ";
				}
			}
		}
		std::cout << "]" << std::endl;
	}

	static void printVector3(glm::vec3 V) {
		std::cout << "(" << V.x << ", " << V.y << ", " << V.z << ")" << std::endl;
	}

	static void printVector4(glm::vec4 V) {
		std::cout << "(" << V.x << ", " << V.y << ", " << V.z << ", " << V.w << ")" << std::endl;
	}
};

#endif