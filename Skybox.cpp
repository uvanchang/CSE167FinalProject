#include <algorithm> 
#include <fstream>
#include <iostream>

#include "Skybox.h"
#include "Math.h"

Skybox::Skybox(std::string textureFilePath, GLuint shaderId)
	: shaderId(shaderId)
{
	textureId = Texture::loadCubemap(textureFilePath);
	/*
	 * Skybox indices used below.
	 *    4----7
     *   /|   /|
     *  0-+--3 |
     *  | 5--+-6
     *  |/   |/
     *  1----2
	 *
	 */

	// The 8 vertices of a Skybox.
	points = std::vector<glm::vec3> 
	{
		glm::vec3(-1, 1, -1),
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, -1),
		glm::vec3(-1, 1, 1),
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
	}; 

	normals = std::vector<glm::vec3>
	{
		glm::vec3(-1, 1, -1),
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, -1),
		glm::vec3(-1, 1, 1),
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
	};

	// Each ivec3(v1, v2, v3) define a triangle consists of vertices v1, v2 
	// and v3 in counter-clockwise order.
	indices = std::vector<glm::ivec3> 
	{
		// Front face.
		glm::ivec3(0, 1, 2),
		glm::ivec3(2, 3, 0),
		// Back face.
		glm::ivec3(6, 5, 4),
		glm::ivec3(4, 7, 6),
		// Right face.
		glm::ivec3(3, 2, 6),
		glm::ivec3(6, 7, 3),
		// Left face.
		glm::ivec3(5, 1, 0),
		glm::ivec3(0, 4, 5),
		// Top face.
		glm::ivec3(4, 0, 3),
		glm::ivec3(3, 7, 4),
		// Bottom face.
		glm::ivec3(2, 1, 5),
		glm::ivec3(5, 6, 2),
	}; 

	float scaleFactor = 500.0f;
	for (unsigned int i = 0; i < points.size(); ++i) {
		points[i].x *= scaleFactor;
		points[i].y *= scaleFactor;
		points[i].z *= scaleFactor;
	}

	// Generate a vertex array (VAO) and 2 vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbos);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// // Bind the texture
	// glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. (Connect to shader) 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO. We will use it to store the normals.
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 1. (Connect to shader) 
	// We will be able to access normals through it.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO. We will use it to store the textures.
	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * textures.size(), textures.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 1. (Connect to shader) 
	// We will be able to access normals through it.
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO and send the data
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind from the VAO.
	glBindVertexArray(0);
}

Skybox::~Skybox()
{
	// Delete the VBOs, EBO, and the VAO.
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(3, vbos);
	glDeleteVertexArrays(1, &vao);
}

void Skybox::render() {
	glUseProgram(shaderId);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glDepthMask(GL_FALSE);
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Bind the texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	// Draw points 
	glDrawElements(GL_TRIANGLES, 3 * indices.size(), GL_UNSIGNED_INT, 0);
	glDepthMask(GL_TRUE);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Skybox::update()
{

}
