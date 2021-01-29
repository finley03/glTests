#ifndef OBJ_H
#define OBJ_H

#include <string>
#include <vector>

struct OBJmaterial {
	bool texture = false;

	//float ambient[3];
	//float ambient[3] = { 0.2f, 0.2f, 0.2f };
	//float diffuse[3] = { 0.8f, 0.8f, 0.8f };
	//float specular[3] = { 1.0f, 1.0f, 1.0f };
	float ambient[3] = { 1.0f, 1.0f, 1.0f };
	float diffuse[3] = { 1.0f, 1.0f, 1.0f };
	float specular[3] = { 1.0f, 1.0f, 1.0f };
	float smoothness = 32.0f;

	std::string ambientFile;
	std::string diffuseFile;
	std::string specularFile;
};

//struct DefaultColorValues {
//	float ambient[3] = { 0.2f, 0.2f, 0.2f };
//	float diffuse[3] = { 0.8f, 0.8f, 0.8f };
//	float specular[3] = { 1.0f, 1.0f, 1.0f };
//};

//DefaultColorValues defaultcolorvalues;

std::vector<float> genMeshFromFile(const char* filePath, int& success, int& size);

std::vector<OBJmaterial> getMatData(const char* filePath, int& success);

std::vector<unsigned int> getMatIndexes(const char* filePath, int& success);

#endif