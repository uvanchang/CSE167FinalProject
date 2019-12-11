#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#define _USE_MATH_DEFINES
#include <cmath>
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

#include "Debug.h"
#include "Geometry.h"
#include "Math.h"
#include "Node.h"
#include "Skybox.h"
#include "Transform.h"
#include "Terrain.h"
#include "texture.h"
// #include "LSystem.h"

struct Light {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    
    float linear;
};

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
    static Transform* world;
	// static Geometry* star;
	static std::vector<Geometry*> stars;
	static std::vector<Geometry*> spheres;
	static Terrain* terrain;
    // static LSystem* trees;
    static std::vector<float> radiuses;
	static std::vector<glm::vec3> origins;
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eye, center, up;
	static GLuint skyboxShader, skybox_projectionLoc, skybox_viewLoc;
	static GLuint objectShader, object_projectionLoc, object_viewLoc, object_cameraPosLoc, object_reflectionLoc;
    static GLuint terrainShader, terrain_projectionLoc, terrain_viewLoc;
    static GLuint lightAmbientLoc, lightDiffuseLoc, lightSpecularLoc, lightPosLoc, lightDirLoc, lightLinearLoc;
    static GLuint plantShader;
    static int renderSpheres;
	static glm::vec3 cursor;
	static glm::vec3 pressedPos;
	static bool pressed;
	static int mode;
    static Light light;

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
