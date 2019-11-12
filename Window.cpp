#include "Window.h"

int Window::width;
int Window::height;
double Window::fov = 60.0f;

std::string Window::windowTitle = "GLFW Starter Project";

// Objects to display.
Skybox * Window::skybox;
Geometry * Window::sphere;
Track * Window::track;

glm::mat4 Window::projection; // Projection matrix.

// camera
float Window::lastX =  640.0f / 2.0;
float Window::lastY =  480.0f / 2.0;
float Window::currX =  0.0f;
float Window::currY =  0.0f;
float Window::yaw   = 90.0f;  // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the right (to face +z axis). TODO: fix once facing -z
float Window::pitch =  0.0f;

glm::vec3 Window::eye(0, 0, 0); // Camera position.
glm::vec3 Window::center(0, 0, 1); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up);

// Shaders
GLuint Window::skyboxShader;
GLuint Window::objectShader;

GLuint Window::skybox_projectionLoc; // Location of projection in shader.
GLuint Window::skybox_viewLoc; // Location of view in shader.
GLuint Window::skybox_normalColoringLoc;

GLuint Window::object_projectionLoc; // Location of projection in shader.
GLuint Window::object_viewLoc; // Location of view in shader.
GLuint Window::object_normalColoringLoc;
GLuint Window::object_cameraPosLoc;
GLuint Window::object_reflectionLoc;

int Window::normalColoring = 0;

glm::vec3 Window::cursor(0, 0, 0); // 3-D position of cursor
glm::vec3 Window::pressedPos(0, 0, 0); // 3-D position of cursor at moment of mouse press
bool Window::pressed = false; // Flag to determine if mouse button is pressed

bool Window::initializeProgram()
{
	// Create a shader program with a vertex shader and a fragment shader.
	skyboxShader = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
	objectShader = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	Debug::checkGLError("load shaders");

	// Check the shader program.
	if (!skyboxShader)
	{
		std::cerr << "Failed to initialize skybox shader" << std::endl;
		return false;
	}
	if (!objectShader)
	{
		std::cerr << "Failed to initialize object shader" << std::endl;
		return false;
	}

	// Get the locations of uniform variables.
	glUseProgram(skyboxShader);
	skybox_projectionLoc = glGetUniformLocation(skyboxShader, "projection");
	skybox_viewLoc = glGetUniformLocation(skyboxShader, "view");
	skybox_normalColoringLoc = glGetUniformLocation(skyboxShader, "normalColoring");

	glUseProgram(objectShader);
	object_cameraPosLoc = glGetUniformLocation(objectShader, "cameraPos");
	object_projectionLoc = glGetUniformLocation(objectShader, "projection");
	object_viewLoc = glGetUniformLocation(objectShader, "view");
	object_normalColoringLoc = glGetUniformLocation(objectShader, "normalColoring");
	object_reflectionLoc = glGetUniformLocation(objectShader, "reflection");

	Debug::checkGLError("initialize program");

	return true;
}

bool Window::initializeObjects()
{	
	//skybox = new Skybox(SKYBOX_DEMO, skyboxShader);
	//skybox = new Skybox(PALACE, skyboxShader);
	skybox = new Skybox(SKYBOX, skyboxShader);
	//skybox = new Skybox(COLORED_FACES, skyboxShader);
	sphere = new Geometry(SPHERE_OBJ, objectShader);
	sphere->setTextureSampler(skybox->getTextureId());
	sphere->setModelMatrix(glm::translate(glm::scale(sphere->getModel(), glm::vec3(1.5f, 1.5f, 1.5f)), glm::vec3(0.0f, 0.0f, 5.0f)));

	track = new Track(objectShader);

	Debug::checkGLError("initialize objects");

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete skybox;
	delete sphere;
	delete track;

	// Delete the shader program.
	glDeleteProgram(skyboxShader);
	glDeleteProgram(objectShader);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

glm::vec3 Window::trackBallMapping(glm::vec2 cursor2D)
{
	glm::vec3 cursor3D;
	GLfloat depth;

	cursor3D.x = (2.0f * cursor2D.x - width) / width;
	cursor3D.y = (height - 2.0f * cursor2D.y) / height;
	cursor3D.z = 0.0f;

	depth = glm::length(cursor3D);
	depth = depth < 1.0f ? depth : 1.0f; // limit depth to be less than or equal to 1

	cursor3D.z = std::sqrtf(1.001f - depth * depth); // calculate z coordinate based on z^2 + d^2 = 1^2

	cursor3D = glm::normalize(cursor3D);
	return cursor3D;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	double aspectRatio = (double)width / (double)height;
	double near = 1.0;
	double far = 1000.0;

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(fov), 
		aspectRatio, near, far);
}

void Window::idleCallback()
{
	sphere->update();
	track->update();

	Debug::checkGLError("update objects");
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Render objects (sphere, bezier track)
	glUseProgram(objectShader);
	glUniformMatrix4fv(object_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(object_viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniform3fv(object_cameraPosLoc, 1, glm::value_ptr(eye));
	glUniform1i(object_normalColoringLoc, normalColoring);
	glUniform1i(object_reflectionLoc, 1); // Enable environment mapping for sphere

	//sphere->render();
	
	glUniform1i(object_reflectionLoc, 0); // Disable environment mapping for bezier track
	
	track->render();

	// Render the skybox.
	glUseProgram(skyboxShader);
	glUniformMatrix4fv(skybox_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(skybox_viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniform1i(skybox_normalColoringLoc, normalColoring);

	skybox->render();

	Debug::checkGLError("render objects");

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}


void Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) 
{
	cursor = trackBallMapping(glm::vec2(xpos, ypos));
	currX = xpos;
	currY = ypos;

	if (pressed) {
	    float xoffset = xpos - lastX;
	    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	    lastX = xpos;
	    lastY = ypos;

	    float sensitivity = 0.1f; // change this value to your liking
	    xoffset *= sensitivity;
	    yoffset *= sensitivity;

	    yaw -= xoffset;
	    pitch -= yoffset;

	    // make sure that when pitch is out of bounds, screen doesn't get flipped
	    if (pitch > 89.0f)
	        pitch = 89.0f;
	    if (pitch < -89.0f)
	        pitch = -89.0f;

	    glm::vec3 front;
	    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	    front.y = sin(glm::radians(pitch));
	    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	    view = glm::lookAt(eye, eye + front, up);
	}
}


void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		pressedPos = cursor;
		lastX = currX;
		lastY = currY;
		pressed = true;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		pressed = false;
	}
}

void Window::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if(fov >= 1.0f && fov <= 90.0f)
		fov += yoffset;
	if(fov <= 1.0f)
		fov = 1.0f;
	if(fov >= 90.0f)
		fov = 90.0f;
	// Update window params
	resizeCallback(window, width, height);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{	
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;
		case GLFW_KEY_N:
			normalColoring = !normalColoring;
			break;
		default:
			break;
		}
	}

	// Adjust camera (for testing)
	switch (key)
	{
	case GLFW_KEY_UP:
		eye = eye + glm::vec3(0.0f, 1.0f, 0.0f);
		view = glm::lookAt(eye, center, up);
		break;
	case GLFW_KEY_DOWN:
		eye = eye + glm::vec3(0.0f, -1.0f, 0.0f);
		view = glm::lookAt(eye, center, up);
		break;
	case GLFW_KEY_LEFT:
		eye = eye + glm::vec3(-1.0f, 0.0f, 0.0f);
		view = glm::lookAt(eye, center, up);
		break;
	case GLFW_KEY_RIGHT:
		eye = eye + glm::vec3(1.0f, 0.0f, 0.0f);
		view = glm::lookAt(eye, center, up);
		break;
	case GLFW_KEY_P:
		// Adjust fov
		if (mods == GLFW_MOD_SHIFT) {
			// Zoom in
			fov += 1.0;
		}
		else {
			// Zoom out
			fov -= 1.0;
		}
		resizeCallback(window, width, height);
		break;
	default:
		break;
	}
}