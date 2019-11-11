#include <algorithm> 
#include <fstream>
#include <iostream>

#include "Geometry.h"
#include "Math.h"

#pragma warning(disable : 4996)

void Geometry::parseOBJ(std::string objFilename) {
	FILE* file = fopen(objFilename.c_str(), "r");
	if (file == NULL) {
		printf("Cannot open %s\n", objFilename.c_str());
		exit(EXIT_FAILURE);
	}

	std::vector<glm::vec3> input_vertices;
	std::vector<glm::vec3> input_normals;
	std::vector<glm::vec2> input_textures;
	std::vector<unsigned int> vertex_indices;
	std::vector<unsigned int> normal_indices;
	std::vector<unsigned int> texture_indices;
	while (true) {
		char startOfLine[128];
		if (fscanf(file, "%s", startOfLine) == EOF) {
			break;
		}

		if (strcmp(startOfLine, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			input_vertices.push_back(vertex);
		}
		else if (strcmp(startOfLine, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			input_normals.push_back(normal);
		}
		else if (strcmp(startOfLine, "vt") == 0) {
			glm::vec3 texture;
			fscanf(file, "%f %f\n", &texture.x, &texture.y);
			input_textures.push_back(texture);
		}
		else if (strcmp(startOfLine, "f") == 0) {
			glm::ivec3 vertexInd;
			glm::ivec3 normalInd;
			glm::ivec3 textureInd;
			fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
				&vertexInd.x, &textureInd.x, &normalInd.x, 
				&vertexInd.y, &textureInd.y, &normalInd.y, 
				&vertexInd.z, &textureInd.z, &normalInd.z);
			vertex_indices.push_back(--vertexInd.x);
			vertex_indices.push_back(--vertexInd.y);
			vertex_indices.push_back(--vertexInd.z);
			normal_indices.push_back(--normalInd.x);
			normal_indices.push_back(--normalInd.y);
			normal_indices.push_back(--normalInd.z);
			texture_indices.push_back(--textureInd.x);
			texture_indices.push_back(--textureInd.y);
			texture_indices.push_back(--textureInd.z);
		}
		else {
			// Ignore other cases
		}
	}

	// Align indices
	for (unsigned int i = 0; i < vertex_indices.size(); ++i) {
		points.push_back(input_vertices[vertex_indices[i]]);
		normals.push_back(input_normals[normal_indices[i]]);
		if (!input_textures.empty())
			textures.push_back(input_textures[texture_indices[i]]);
		indices.push_back(i);
	}

	fclose(file);
}

Geometry::Geometry(std::string objFilename, GLuint shader)
	: program(shader)
{
	parseOBJ(objFilename);

	// Calculate the maximum coordinate values along each (x,y,z) axis
	float maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;
	float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
	for (glm::vec3 point : points) {
		maxX = point.x > maxX ? point.x : maxX;
		maxY = point.y > maxY ? point.y : maxY;
		maxZ = point.z > maxZ ? point.z : maxZ;
		minX = point.x < minX ? point.x : minX;
		minY = point.y < minY ? point.y : minY;
		minZ = point.z < minZ ? point.z : minZ;
	}

	// Determine the center point of the model
	float offsetX = (maxX + minX) / 2.0f;
	float offsetY = (maxY + minY) / 2.0f;
	float offsetZ = (maxZ + minZ) / 2.0f;
	
	// Shift the entire model to the center of the coordinate system
	for (unsigned int i = 0; i < points.size(); ++i) {
		points[i].x -= offsetX;
		points[i].y -= offsetY;
		points[i].z -= offsetZ;
	}

	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);
	// Set the color. 
	color = glm::vec3(1, 1, 1);
	// Set the default material parameters.
	material.ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	material.diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
	material.shininess = 32.0f;
}

Geometry::~Geometry() {
	// Delete the VBOs, EBO, and the VAO.
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(3, vbos);
	glDeleteVertexArrays(1, &vao);
}

void Geometry::setModelMatrix(glm::mat4 M) {
	model = M;
}

void Geometry::render() {
	glUseProgram(program);

	GLuint modelLoc = glGetUniformLocation(program, "model");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Bind to the VAO.
	glBindVertexArray(vao);
	if (textureId) {
		// Bind the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	}
	// Draw points 
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Geometry::draw(glm::mat4 C) {
	if (toRender) {
		setModelMatrix(C);
		render();
	}
}

void Geometry::update() {
	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbos);

	// Bind to the VAO.
	glBindVertexArray(vao);

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

	// Generate EBO, bind the EBO to the bound VAO and send the data
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}