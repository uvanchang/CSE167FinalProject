#include "Window.h"

int Window::width;
int Window::height;
double Window::fov = 60.0f;

std::string Window::windowTitle = "GLFW Starter Project";

// Objects to display.
Skybox * Window::skybox;
Transform * Window::world;
Geometry * Window::star;
Terrain * Window::terrain;
std::vector<LSystem*> Window::trees;

double Window::oldTime;
double Window::distance;
unsigned int Window::curveIndex = 0;
unsigned int Window::pointIndex = 0;
bool Window::pauseTrack = false;

glm::mat4 Window::projection; // Projection matrix.

// camera
float Window::lastX =  640.0f / 2.0;
float Window::lastY =  480.0f / 2.0;
float Window::currX =  0.0f;
float Window::currY =  0.0f;
float Window::yaw   = 90.0f;  // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the right (to face +z axis). TODO: fix once facing -z
float Window::pitch =  0.0f;

glm::vec3 Window::eye(0, 10, 0); // Camera position.
glm::vec3 Window::center(0, 10, 1); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up);

// Shaders
GLuint Window::skyboxShader;
GLuint Window::objectShader;
GLuint Window::terrainShader;
GLuint Window::plantShader;

GLuint Window::skybox_projectionLoc; // Location of projection in shader.
GLuint Window::skybox_viewLoc; // Location of view in shader.

GLuint Window::object_projectionLoc; // Location of projection in shader.
GLuint Window::object_viewLoc; // Location of view in shader.
GLuint Window::object_cameraPosLoc;
GLuint Window::object_reflectionLoc;

GLuint Window::terrain_projectionLoc; // Location of projection in shader.
GLuint Window::terrain_viewLoc; // Location of view in shader.

GLuint Window::plant_projectionLoc;
GLuint Window::plant_viewLoc;

Light Window::light;

GLuint Window::lightAmbientLoc; // Location of light ambient in shader.
GLuint Window::lightDiffuseLoc; // Location of light diffuse in shader.
GLuint Window::lightSpecularLoc; // Location of light specular in shader.
GLuint Window::lightPosLoc; // Location of light position in shader.
GLuint Window::lightDirLoc; // Location of light direction in shader.
GLuint Window::lightLinearLoc; // Location of light attenuation linear factor in shader.

glm::vec3 Window::cursor(0, 0, 0); // 3-D position of cursor
glm::vec3 Window::pressedPos(0, 0, 0); // 3-D position of cursor at moment of mouse press
bool Window::pressed = false; // Flag to determine if mouse button is pressed

bool Window::initializeProgram()
{
	// Create a shader program with a vertex shader and a fragment shader.
	skyboxShader = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
	objectShader = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	terrainShader = LoadShaders("shaders/terrainShader.vert", "shaders/terrainShader.frag");
    plantShader = LoadShaders("shaders/plantShader.vert", "shaders/plantShader.frag");

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
	if (!terrainShader)
	{
		std::cerr << "Failed to initialize terrain shader" << std::endl;
		return false;
	}
    if (!plantShader)
    {
        std::cerr << "Failed to initialize terrain shader" << std::endl;
        return false;
    }

	// Get the locations of uniform variables.
	glUseProgram(skyboxShader);
	skybox_projectionLoc = glGetUniformLocation(skyboxShader, "projection");
	skybox_viewLoc = glGetUniformLocation(skyboxShader, "view");

	glUseProgram(objectShader);
	object_cameraPosLoc = glGetUniformLocation(objectShader, "cameraPos");
	object_projectionLoc = glGetUniformLocation(objectShader, "projection");
	object_viewLoc = glGetUniformLocation(objectShader, "view");
	object_reflectionLoc = glGetUniformLocation(objectShader, "reflection");

	glUseProgram(terrainShader);
	terrain_projectionLoc = glGetUniformLocation(terrainShader, "projection");
	terrain_viewLoc = glGetUniformLocation(terrainShader, "view");
    
    glUseProgram(plantShader);
    plant_projectionLoc = glGetUniformLocation(terrainShader, "projection");
    plant_viewLoc = glGetUniformLocation(terrainShader, "view");

	return true;
}

bool Window::initializeObjects()
{
    // Set the light parameters.
    light.position = glm::vec3(0.0f, 20.0f, 0.0f);
    light.direction = glm::vec3(1.0f, 1.0f, 0.0f);
    light.ambient = glm::vec3(1.0f, 1.0f, 0.8f);
    light.diffuse = glm::vec3(1.0f, 1.0f, 0.8f);
    light.specular = glm::vec3(1, 1, 1);
    light.linear = 0.09f;
    
    lightAmbientLoc = glGetUniformLocation(objectShader, "light.ambient");
    lightDiffuseLoc = glGetUniformLocation(objectShader, "light.diffuse");
    lightSpecularLoc = glGetUniformLocation(objectShader, "light.specular");
    lightPosLoc = glGetUniformLocation(objectShader, "light.position");
    lightDirLoc = glGetUniformLocation(objectShader, "light.direction");
    lightLinearLoc = glGetUniformLocation(objectShader, "light.linear");
    
	skybox = new Skybox({"skybox/right.jpg", "skybox/left.jpg", "skybox/top.jpg", "skybox/bottom.jpg", "skybox/front.jpg", "skybox/back.jpg"}, skyboxShader);
    
    world = new Transform();

    star = new Geometry("objs/star.obj", objectShader);
    
	terrain = new Terrain(3, terrainShader);
    eye.y = terrain->getTerrainHeight(0, 0) + 2;
    center.y = eye.y;
    view = glm::lookAt(eye, center, up);
    
    for(unsigned int i = 0; i < 5; i++) {
        float randX = std::rand() % (terrain->scale * terrain->terrain.size()) - (terrain->scale * terrain->terrain.size() / 2.0f);
        float randZ = std::rand() % (terrain->scale * terrain->terrain.size()) - (terrain->scale * terrain->terrain.size() / 2.0f);
        
        LSystem* tree = new LSystem(glm::vec3(randX, terrain->getTerrainHeight(randX, randZ), randZ), 20.0f * (float)(M_PI / 180.0f), 5, plantShader);
        trees.push_back(tree);
    }
    
    Transform* scale = new Transform(glm::scale(glm::vec3(0.1f, 0.4f, 0.2f)), 1);
    Transform* rotate = new Transform(glm::rotate((float)(M_PI / 2), glm::vec3(1, 0, 0)), 0);
    scale->addChild(star);
    rotate->addChild(scale);
    
    // create starting positions for the stars
    for(unsigned int i = 0; i < 5; i++) {
        float randX = std::rand() % (terrain->scale * terrain->terrain.size()) - (terrain->scale * terrain->terrain.size() / 2.0f);
        float randZ = std::rand() % (terrain->scale * terrain->terrain.size()) - (terrain->scale * terrain->terrain.size() / 2.0f);
        
        Transform* randomTransform = new Transform(glm::translate(glm::vec3(randX,
                                                                           terrain->getTerrainHeight(randX, randZ) + 3,
                                                                           randZ)), 0);
        
        randomTransform->addChild(rotate);
        world->addChild(randomTransform);
    }

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete skybox;
    delete world;
	delete star;
	delete terrain;

	// Delete the shader program.
	glDeleteProgram(skyboxShader);
	glDeleteProgram(objectShader);
	glDeleteProgram(terrainShader);
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
    world->update();
}

void Window::displayCallback(GLFWwindow* window)
{
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render the star
    glUseProgram(objectShader);
    glUniformMatrix4fv(object_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(object_viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    
    glUniform3fv(lightAmbientLoc, 1, glm::value_ptr(light.ambient));
    glUniform3fv(lightDiffuseLoc, 1, glm::value_ptr(light.diffuse));
    glUniform3fv(lightSpecularLoc, 1, glm::value_ptr(light.specular));
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(light.position));
    glUniform3fv(lightDirLoc, 1, glm::value_ptr(light.direction));
    glUniform1f(lightLinearLoc, light.linear);
    
    world->draw(glm::mat4(1.0));

	// Render the terrain.
	glUseProgram(terrainShader);
	glUniformMatrix4fv(terrain_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(terrain_viewLoc, 1, GL_FALSE, glm::value_ptr(view));	

	terrain->render();
    
    glUseProgram(plantShader);
    glUniformMatrix4fv(plant_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(plant_viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    
    for (LSystem* tree : trees) {
        tree->render();
    }

	// Render the skybox.
    glDepthFunc(GL_LEQUAL);
	glUseProgram(skyboxShader);
	glUniformMatrix4fv(skybox_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glm::mat4 view = glm::mat4(glm::mat3(Window::view)); // remove translation from the view matrix
    glUniformMatrix4fv(skybox_viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    
    skybox->render();
    glDepthFunc(GL_LESS);
    
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
        center = eye + front;
        view = glm::lookAt(eye, center, up);
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
		default:
			break;
		}
	}
    
    glm::vec3 forward = center - eye;
    glm::vec3 front = glm::normalize(glm::vec3(forward.x, 0, forward.z));
	// Adjust camera (for testing)
	switch (key)
	{
        case GLFW_KEY_W:
            eye += front;
            center += front;
            break;
        case GLFW_KEY_A:
            eye += glm::rotateY(front, (float)(M_PI / 2));
            center += glm::rotateY(front, (float)(M_PI / 2));
            break;
        case GLFW_KEY_S:
            eye += -front;
            center += -front;
            break;
        case GLFW_KEY_D:
            eye += glm::rotateY(front, -(float)(M_PI / 2));
            center += glm::rotateY(front, -(float)(M_PI / 2));
            break;
        default:
            break;
	}
    float oldEyeY = eye.y;
    eye.y = terrain->getTerrainHeight(center.x, center.z) + 2;
    center.y += eye.y - oldEyeY;
    view = glm::lookAt(eye, center, up);
}
