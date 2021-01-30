#ifndef OBJECTS_H
#define OBJECTS_H

#include <vector>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaders.h"
#include "textures.h"


class Objects {
public:
	unsigned int VAO;

	Shader* objectShader;
	Shader* textureShader;
	Texture* textures;



	Objects();

	int createFromFile(std::string filePath, std::string name);

	void draw(std::string name);

	void drawLight(std::string name);

	void destroy(std::string name);

	void terminate();

private:
	struct Material {
		bool texture = false;

		float ambient[3];
		float diffuse[3];
		float specular[3];
		float smoothness;

		std::string ambientTextureName;
		std::string diffuseTextureName;
		std::string specularTextureName;
	};




	struct Data {
		//float position[3] = {0.0f, 0.0f, 0.0f}; // object location
		unsigned int VBO; // ID of the VBO containing mesh data
		
		std::vector<Material> materials;

		std::vector<unsigned int> index;

		std::string name; // object name
		bool display = true; // if true object will draw
		glm::mat4 model = glm::mat4(1.0f); // model matrix
		float scaleFactor = 1.0f;
		int size; // number of vertices

		bool texture = false;
	};

	std::map<std::string, int> shapeMap;
	std::vector<Data> shapes;
};

#endif
