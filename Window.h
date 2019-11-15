#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>

#include <stdlib.h>

#include "shader.h"
#include "texture.h"

#include "Constants.h"
#include "Debug.h"
#include "Geometry.h"
#include "Math.h"
#include "Node.h"
#include "Skybox.h"
#include "Track.h"
#include "Transform.h"

class Window
{
public:
	static int width;
	static int height;
	static double fov;
	static double oldTime, distance;
	static unsigned int curveIndex, pointIndex;
	static bool pauseTrack;
	static float currX, currY, lastX, lastY, pitch, yaw;
	static std::string windowTitle;
	static Skybox* skybox;
	static Geometry* sphere;
	static Track* track;
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eye, center, up;
	static GLuint skyboxShader, skybox_projectionLoc, skybox_viewLoc, skybox_normalColoringLoc;
	static GLuint objectShader, object_projectionLoc, object_viewLoc, object_normalColoringLoc, object_cameraPosLoc, object_reflectionLoc;
	static int normalColoring;
	static glm::vec3 cursor;
	static glm::vec3 pressedPos;
	static bool pressed;
	static int mode;

	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();
	static GLFWwindow* createWindow(int width, int height);
	static glm::vec3 trackBallMapping(glm::vec2 point);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif