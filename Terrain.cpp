#include "Terrain.h"

void Terrain::Terrain(int n, GLuint shader)
	: shader(shader)
{
	int length = 2 * n + 1;
	terrain = std::vector<std::vector<int>>(length, vector<int> (length, 0));
	std::vector<std::pair<int, int> cornerPoints> = {
		std::make_pair(0, 0), 
		std::make_pair(0, length-1), 
		std::make_pair(length-1, 0), 
		std::make_pair(length-1, length-1)
	};

	// Initialize corner values
	terrain[0][0] = 5;
	terrain[0][length-1] = 5;
	terrain[length-1][0] = 5;
	terrain[length-1][length-1] = 5;

	// Initialize terrain values
	diamondSquare(n, cornerPoints);

	// Initialize gl points
	for (int y = 0; y < terrain.size() - 1; ++y) {
		for (int x = 0; x < terrain[0].size() - 1; ++x) {
			// Triangle 1 (counter clockwise)
			points.push_back(glm::vec3(x, terrain[y][x], y));
			points.push_back(glm::vec3(x, terrain[y+1][x], y+1));
			points.push_back(glm::vec3(x+1, terrain[y][x+1], y));

			// Triangle 2 (counter clockwise)
			points.push_back(glm::vec3(x+1, terrain[y][x+1], y));
			points.push_back(glm::vec3(x, terrain[y+1][x], y+1));
			points.push_back(glm::vec3(x+1, terrain[y+1][x+1], y+1));
		}
	}

	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);
	// Set the color. 
	color = glm::vec3(1, 0, 0);

	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. (Connect to shader) 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

float avgPlusRandom(std::vector<std::pair<int, int>> cornerPoints) {

	float sum = 0;
	for (std::pair p : cornerPoints) {
		sum += terrain[p.first][p.second];
	}
	float randomNumber = (std::rand() % 100) / 100;
	randomNumber = std::rand() % 2 ? -randomNumber : randomNumber; // negative if odd
	return sum / cornerPoints.size() + randomNumber;

}

void Terrain::diamondSquare(int n, std::vector<std::pair<int, int>> cornerPoints)
{
	// Base case
	if (n == 0) {
		return;
	}

	// Diamond step
	int distance = (cornerPoints[1].second - cornerPoints[0].second) / 2;
	int x = distance + cornerPoints[0].second;
	int y = distance + cornerPoints[0].first;
	terrain[y][x] = avgPlusRandom(cornerPoints);

	std::vector<std::pair<int, int>> diamondMidpoints = { // Clock-wise from top
		std::make_pair(y - distance, x),
		std::make_pair(y, x + distance),
		std::make_pair(y + distance, x),
		std::make_pair(y, x - distance)
	};

	// Square step
	for (std::pair p : diamondMidpoints) {
		std::vector<std::pair<int, int>> diamondCorners;
		if (p.first - distance >= 0) {
			diamondCorners.push_back(std::make_pair(p.first - distance, p.second));
		}
		if (p.second + distance < terrain.size()) {
			diamondCorners.push_back(std::make_pair(p.first, p.second + distance));
		}
		if (p.first + distance < terrain.size()) {
			diamondCorners.push_back(std::make_pair(p.first + distance, p.second));
		}
		if (p.second - distance >= 0) {
			diamondCorners.push_back(std::make_pair(p.first, p.second - distance));
		}
		terrain[p.first][p.second] = avgPlusRandom(diamondCorners);
	}

	// Recursive step
	for (int x = cornerPoints[0].second; x < cornerPoints[1].second; x += distance) {

		for (int y = cornerPoints[0].first; y < cornerPoints[3].first; y += distance) {

			std::vector<std::pair<int, int>> newSquarePoints = {
				std::make_pair(y, x),
				std::make_pair(y, x + distance),
				std::make_pair(y + distance, x),
				std::make_pair(y + distance, x + distance)
			};
			diamondSquare(n - 1, newSquarePoints);

		}

	}
}

void Terrain::render() {
	glUseProgram(program);

	GLuint modelLoc = glGetUniformLocation(program, "model");
	GLuint colorLoc = glGetUniformLocation(program, "color");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));

	// Bind to the VAO.
	glBindVertexArray(vao);
	if (textureId) {
		// Bind the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	}
	// Draw points 
	glDrawArrays(GL_TRIANGLES, 0, points.size() * 3);
	// Unbind from the VAO.
	glBindVertexArray(0);
}
