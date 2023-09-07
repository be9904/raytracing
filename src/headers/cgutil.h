#ifndef CGUTIL_H
#define CGUTIL_H

#define STB_IMAGE_IMPLEMENTATION

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "stb_image.h"

#include <vector>

GLuint create_program() {
	// Setup shaders
	const char* vs_path = "bin/shaders/rt.vert";
	const char* fs_path = "bin/shaders/rt.frag";

	unsigned int vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vs_path, NULL);
	glCompileShader(vertex);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fs_path, NULL);
	glCompileShader(fragment);

	GLuint program = glCreateProgram();
	
	// attach shaders
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);

	// link program
	glLinkProgram(program);

	// delete shaders after linking to program
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	// TODO: add validation check for program and shaders

	return program;
}

GLuint setup_texture() {
	float tex_coord[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	// load and create a texture 
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	unsigned char* data = stbi_load("../bin/output.png", &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return texture;
}

#endif