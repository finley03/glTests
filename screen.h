#ifndef SCREEN_H
#define SCREEN_H

#include "shaders.h"

class Screen {
public:
	Screen(unsigned int width, unsigned int height, int &success);

	void clear();

	void swap();

	float clearColor[3] = { 0.1f, 0.1f, 0.1f };

private:
	unsigned int FBO;
	unsigned int RBO;
	unsigned int SCRTEX;

	unsigned int VBO;

	float vertices[24] = {
		1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f
	};

	const char* vertexFile = "s.vert";
	const char* fragmentFile = "s.frag";

	Shader* scrShader;


};

#endif