#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>

#include "stb_image.h"

namespace fs = std::filesystem;

class Texture
{
public:
	static GLuint loadCubemap(std::string textureFilePath)
	{
		GLuint textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

		std::vector<std::string> textures_faces; // Path to all 6 sides (right, left, top, bot, back, front)
		for (const auto & file : fs::directory_iterator(textureFilePath)) {
			textures_faces.push_back(file.path().string());
		}

		int width, height, nrChannels;
		unsigned char *data;  
		// for(GLuint i = 0; i < textures_faces.size(); i++)
		// {
		//     data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
		//     if (data)
	 //        {
	 //        	std::cout << "Loading " << textures_faces[i] << "(" << width << ", " << height << ")" << std::endl;
	 //            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
	 //                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
	 //            );
	 //            stbi_image_free(data);
	 //        }
	 //        else
	 //        {
	 //            std::cout << "Cubemap texture failed to load at path: " << textures_faces[i] << std::endl;
	 //            stbi_image_free(data);
	 //        }
		// }

		data = stbi_load(textures_faces[0].c_str(), &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
        data = stbi_load(textures_faces[1].c_str(), &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);	
        data = stbi_load(textures_faces[2].c_str(), &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);	
        data = stbi_load(textures_faces[3].c_str(), &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);	
        data = stbi_load(textures_faces[5].c_str(), &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);	
        data = stbi_load(textures_faces[4].c_str(), &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);	

		// Make sure no bytes are padded:
	  	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		// Use bilinear interpolation:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// Use clamp to edge to hide skybox edges:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

		return textureId;
	}
};

#endif


// // Loads image as texture, returns ID of texture
// GLuint Texture::loadTexture(const char * textureFilePath)
// {
// 	GLuint textureId;
// 	glGenTextures(1, &textureId); // Get unique ID for texture
// 	glBindTexture(GL_TEXTURE_2D, textureId); // Tell OpenGL which texture to edit
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // set bi-linear interpolation
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // for both filtering modes
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture edge mode
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
// 	Image* image = loadJPG(textureFilePath); // load image from disk; uses third party Image library
// 	// Depending on the image library, the texture image may have to be flipped vertically
// 	// Load image into OpenGL texture in GPU memory:
// 	glTexImage2D(GL_TEXTURE_2D, // Always GL_TEXTURE_2D for image textures
// 	0, // 0 for now
// 	GL_RGB, // Format OpenGL uses for image without alpha channel
// 	image->width, image->height, // Width and height
// 	0, // The border of the image
// 	GL_RGB, // GL_RGB, because pixels are stored in RGB format
// 	GL_UNSIGNED_BYTE, // GL_UNSIGNED_BYTE, because pixels are stored as unsigned numbers
// 	image->pixels); // The actual RGB image data
// 	return textureId; // Return the ID of the texture
// }

