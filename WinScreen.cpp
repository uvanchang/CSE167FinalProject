#include "WinScreen.h"

WinScreen::WinScreen(GLuint shader)
: shader(shader)
{
    textureId = Texture::loadTexture("textures/win.jpg");
    
    float screenVertices[] = {
        -250.0f,  50.0f, 0.0f,
        -250.0f, -50.0f, 0.0f,
        250.0f, -50.0f, 0.0f,
        -250.0f, 50.0f, 0.0f,
        250.0f,  -50.0f, 0.0f,
        250.0f,  50.0f, 0.0f
    };
    
    float textures[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };
    
    // Set the model matrix to an identity matrix.
    model = glm::scale(glm::vec3(0.1)) * glm::translate(glm::vec3(0, 300, 0));
    
    // Generate a vertex array (VAO) and a vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbos);
    
    // Bind to the VAO.
    glBindVertexArray(vao);
    
    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), &screenVertices, GL_STATIC_DRAW);
    // Enable vertex attribute 0. (Connect to shader)
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(textures), &textures, GL_STATIC_DRAW);
    // Enable vertex attribute 1. (Connect to shader)
    // We will be able to access normals through it.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    
    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
    
    std::cout << "Win Screen initialized" << std::endl;
}

WinScreen::~WinScreen() {
    // Delete the VBO and the VAO.
    glDeleteBuffers(2, vbos);
    glDeleteVertexArrays(1, &vao);
}

void WinScreen::render() {
    glUseProgram(shader);
    
    GLuint modelLoc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    // Bind to the VAO.
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // Draw points
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // Unbind from the VAO.
    glBindVertexArray(0);
    
    glUseProgram(shader);
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::translate(glm::vec3(0.0f, 0.0f, -0.01f)) * glm::rotate(model, (float)M_PI, glm::vec3(0, 1, 0))));
    
    // Bind to the VAO.
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // Draw points
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // Unbind from the VAO.
    glBindVertexArray(0);
}
