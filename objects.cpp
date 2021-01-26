#include <glad/glad.h>

#include <iostream>
#include "objects.h"
#include "obj.h"


Objects::Objects() {
	glGenVertexArrays(1, &VAO); // generate vertex array object
	glBindVertexArray(VAO); // bind vertex array
	// the program is currently only capable of working with
	// one vertex array object


	//shaderProgram = shaderProgram;
}


int Objects::createFromFile(const char* filePath, std::string name) {
	Data object; // create object struct

	object.name = name;

	glGenBuffers(1, &object.VBO); // generate VBO, store ID in variable in struct

	std::vector<float> mesh;

	int meshSuccess;
	mesh = genMeshFromFile(filePath, meshSuccess, object.size);
	if (!meshSuccess) std::cout << "Mesh for \"" << name << "\" could not be generated";

	glBindBuffer(GL_ARRAY_BUFFER, object.VBO); // bind VBO
	glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(float), &mesh[0], GL_STATIC_DRAW); // Buffer mesh in VBO

	shapeMap[name] = shapes.size(); // add object to shape map
	shapes.push_back(object); // add object to shapes struct

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // vertex attribute pointer
	//glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // vertex normal attribute pointer
	//glEnableVertexAttribArray(1);

	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texture attribute pointer
	//glEnableVertexAttribArray(2);

	return true;

}


void Objects::draw(std::string name) {
	glBindBuffer(GL_ARRAY_BUFFER, shapes[shapeMap[name]].VBO); // bind vbo of shape

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // vertex attribute pointer
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // vertex normal attribute pointer
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texture attribute pointer
	glEnableVertexAttribArray(2);

	/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/

	glDrawArrays(GL_TRIANGLES, 0, shapes[shapeMap[name]].size); // 36
}


void Objects::destroy(std::string name) {
	glDeleteBuffers(1, &shapes[shapeMap[name]].VBO);
}


void Objects::terminate() {
	glDeleteBuffers(1, &VAO);
}
