#ifndef OBJECTS_H
#define OBJECTS_H

#include <vector>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Objects {
public:
	unsigned int VAO;

	Objects();

	int createFromFile(const char* filePath, std::string name);

	void draw(std::string name);

	void destroy(std::string name);

	void terminate();

private:
	struct Data {
		//float position[3] = {0.0f, 0.0f, 0.0f}; // object location
		unsigned int VBO; // ID of the VBO containing mesh data
		std::string name; // object name
		bool display = true; // if true object will draw
		glm::mat4 model = glm::mat4(1.0f); // model matrix
		float scaleFactor = 1.0f;
		int size;
	};

	std::map<std::string, int> shapeMap;
	std::vector<Data> shapes;
};

#endif
