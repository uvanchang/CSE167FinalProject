#include "Terrain.h"

Terrain::Terrain(int n, GLuint shader)
	: shader(shader)
{
    textureId = Texture::loadTexture("textures/tough_grass.jpg");
    
    int length = pow(2, n) + 1;
    terrain = std::vector<std::vector<int>>(length, std::vector<int> (length, 0));
	std::vector<std::pair<int, int>> cornerPoints = {
		std::make_pair(0, 0), 
		std::make_pair(0, length-1), 
		std::make_pair(length-1, 0), 
		std::make_pair(length-1, length-1)
	};
    
	// Initialize corner values
	terrain[0][0] = 3;
	terrain[0][length-1] = 3;
	terrain[length-1][0] = 2;
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
            textures.push_back(glm::vec2(0.0f, 1.0f));
            textures.push_back(glm::vec2(0.0f, 0.0f));
            textures.push_back(glm::vec2(1.0f, 1.0f));
            glm::vec3 v1 = glm::vec3(x, terrain[y+1][x], y+1) - glm::vec3(x, terrain[y][x], y);
            glm::vec3 v2 = glm::vec3(x+1, terrain[y][x+1], y) - glm::vec3(x, terrain[y][x], y);
            normals.push_back(glm::cross(v1, v2));

			// Triangle 2 (counter clockwise)
			points.push_back(glm::vec3(x+1, terrain[y][x+1], y));
			points.push_back(glm::vec3(x, terrain[y+1][x], y+1));
			points.push_back(glm::vec3(x+1, terrain[y+1][x+1], y+1));
            textures.push_back(glm::vec2(1.0f, 1.0f));
            textures.push_back(glm::vec2(0.0f, 0.0f));
            textures.push_back(glm::vec2(1.0f, 0.0f));
            v1 = glm::vec3(x, terrain[y+1][x], y+1) - glm::vec3(x+1, terrain[y][x+1], y);
            v2 = glm::vec3(x+1, terrain[y+1][x+1], y+1) - glm::vec3(x+1, terrain[y][x+1], y);
            normals.push_back(glm::cross(v1, v2));
		}
	}
    
    float offset = terrain.size() / 2.0f;
    
    // Shift the entire model to the center of the coordinate system
    for (unsigned int i = 0; i < points.size(); ++i) {
        points[i].x -= offset;
        points[i].z -= offset;
        points[i].x *= scale;
        points[i].z *= scale;
    }

	// Set the model matrix to an identity matrix. 
    model = glm::mat4(1);//glm::translate(glm::vec3(terrain.size() / 2, 0, terrain.size() / 2));
	// Set the color. 
	color = glm::vec3(1, 0, 0);

	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbos);

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
    
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * textures.size(), textures.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 1. (Connect to shader)
    // We will be able to access textures through it.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
    
    std::cout << "Terrain initialized" << std::endl;
}

Terrain::~Terrain() {
    // Delete the VBO and the VAO.
    glDeleteBuffers(3, vbos);
    glDeleteVertexArrays(1, &vao);
}

float Terrain::avgPlusRandom(std::vector<std::pair<int, int>> cornerPoints, int n) {

	float sum = 0;
	for (std::pair<int, int> p : cornerPoints) {
		sum += terrain[p.first][p.second];
	}
	float randomNumber = n + (std::rand() % 10) / 10.0f;
	//randomNumber = std::rand() % 2 ? -randomNumber : randomNumber; // negative if odd
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
	terrain[y][x] = avgPlusRandom(cornerPoints, n);

	std::vector<std::pair<int, int>> diamondMidpoints = { // Clock-wise from top
		std::make_pair(y - distance, x),
		std::make_pair(y, x + distance),
		std::make_pair(y + distance, x),
		std::make_pair(y, x - distance)
	};

	// Square step
	for (std::pair<int, int> p : diamondMidpoints) {
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
		terrain[p.first][p.second] = avgPlusRandom(diamondCorners, n);
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
	glUseProgram(shader);

	GLuint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Bind to the VAO.
	glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
	// Draw points
	glDrawArrays(GL_TRIANGLES, 0, points.size() * 3);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

float Terrain::getTerrainHeight(float x, float z) {
    
    float terrainX = x + scale * terrain.size() / 2.0f; // Center terrain coordinates around (0,0)
    float terrainZ = z + scale * terrain.size() / 2.0f;
    
    float scaleX = terrainX / scale; // Scale back down
    float scaleZ = terrainZ / scale;
    
    if (floor(scaleX) == ceil(scaleX)) { // Offsets to avoid zero determinant (when floor == ceil)
        scaleX = scaleX == 0 ? scaleX + 0.5 : scaleX - 0.5;
    }
    if (floor(scaleZ) == ceil(scaleZ)) {
        scaleZ = scaleZ == 0 ? scaleZ + 0.5 : scaleZ - 0.5;
    }
    
    float row = ceil(scaleZ) -  scaleZ;
    float col = scaleX - floor(scaleX);
    
    glm::vec3 v1, v2, v3;
    if (col < row) { // upper triangle
        v1 = glm::vec3(floor(scaleX), terrain[floor(scaleZ)][floor(scaleX)], floor(scaleZ));
    } else {         // lower triangle
        v1 = glm::vec3(ceil(scaleX), terrain[ceil(scaleZ)][ceil(scaleX)], ceil(scaleZ));
    }
    v2 = glm::vec3(ceil(scaleX), terrain[floor(scaleZ)][ceil(scaleX)], floor(scaleZ));
    v3 = glm::vec3(floor(scaleX), terrain[ceil(scaleZ)][floor(scaleX)], ceil(scaleZ));
    
    float det = (v2.z - v3.z) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.z - v3.z);
    
    float l1 = ((v2.z - v3.z) * (scaleX - v3.x) + (v3.x - v2.x) * (scaleZ - v3.z)) / det;
    float l2 = ((v3.z - v1.z) * (scaleX - v3.x) + (v1.x - v3.x) * (scaleZ - v3.z)) / det;
    float l3 = 1.0f - l1 - l2;
    
    return l1 * v1.y + l2 * v2.y + l3 * v3.y;
    
}
