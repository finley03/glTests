//#define _CRTDBG_MAP_ALLOC

#include <glad/glad.h>

#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <ratio>
#include <iomanip>
#include "terminalColors.h""
#include "objects.h"
#include "obj.h"


Objects::Objects() {
	glGenVertexArrays(1, &VAO); // generate vertex array object
	glBindVertexArray(VAO); // bind vertex array
	// the program is currently only capable of working with
	// one vertex array object

	//int textureUnits;
	//glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
	//std::cout << "Max " << textureUnits << " texture units" << std::endl;

	//shaderProgram = shaderProgram;
}


int Objects::createFromFile(std::string filePath, std::string name) {
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	std::cout << std::endl;

	Data object; // create object struct

	object.name = name;

	glGenBuffers(1, &object.VBO); // generate VBO, store ID in variable in struct

	std::vector<float> mesh;

	int meshSuccess;
	mesh = genMeshFromFile(filePath, meshSuccess, object.size);
	if (!meshSuccess) std::cout << color::error << "Mesh for \"" << name << "\" could not be generated" << color::std << std::endl;


	std::vector<OBJmaterial> matData = getMatData(filePath, meshSuccess); // get material data

	for (int i = 0; i < matData.size(); ++i) {
		Material mat;
		mat.ambient[0] = matData[i].ambient[0];
		mat.ambient[1] = matData[i].ambient[1];
		mat.ambient[2] = matData[i].ambient[2];
		mat.diffuse[0] = matData[i].diffuse[0];
		mat.diffuse[1] = matData[i].diffuse[1];
		mat.diffuse[2] = matData[i].diffuse[2];
		mat.specular[0] = matData[i].specular[0];
		mat.specular[1] = matData[i].specular[1];
		mat.specular[2] = matData[i].specular[2];
		mat.smoothness = matData[i].smoothness;
		mat.texture = matData[i].texture;

		//object.texture = matData[i].texture;
		//std::cout << "Texture?" << matData[i].texture << std::endl;
		
		if (!matData[i].ambientFile.empty()) {
			textures->newTexture(matData[i].ambientFile.c_str(), matData[i].ambientFile, 0, GL_REPEAT, GL_NEAREST);
			mat.ambientTextureName = matData[i].ambientFile;
		}
		if (!matData[i].diffuseFile.empty()) {
			textures->newTexture(matData[i].diffuseFile.c_str(), matData[i].diffuseFile, 0, GL_REPEAT, GL_NEAREST);
			//std::cout << "Texture " << matData[i].diffuseFile.c_str() << std::endl;
			mat.diffuseTextureName = matData[i].diffuseFile;
			//std::cout << "Creating texture " << matData[i].diffuseFile << std::endl;
		}
		if (!matData[i].specularFile.empty()) {
			textures->newTexture(matData[i].specularFile.c_str(), matData[i].specularFile, 1, GL_REPEAT, GL_NEAREST);
			mat.specularTextureName = matData[i].specularFile;
			//std::cout << "Creating texture " << matData[i].specularFile << std::endl;
		}

		object.materials.push_back(mat);
	}

	std::cout << "Getting material indexes for " << color::file << "\"" << filePath << "\"" << color::std << std::endl;
	object.index = getMatIndexes(filePath, meshSuccess); // get material indexes

	std::cout << color::process << "Buffering data" << color::std << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, object.VBO); // bind VBO
	glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(float), &mesh[0], GL_STATIC_DRAW); // Buffer mesh in VBO


	shapeMap[name] = shapes.size(); // add object to shape map
	shapes.push_back(object); // add object to shapes struct

	// clean up memory

	mesh.clear();
	mesh.shrink_to_fit();

	std::cout << color::success << "Finished loading file " << color::file << "\"" << filePath << "\"" << color::std << std::endl;

	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> time_span = std::chrono::duration_cast<std::chrono::duration<float>>(t2 - t1);

	std::cout << "Time: " << color::value << std::fixed << std::setprecision(4) << time_span.count() << " seconds" << color::std << std::endl;


	return true;

}


void Objects::draw(std::string name) {

	//glBindBuffer(GL_ARRAY_BUFFER, shapes[shapeMap[name]].VBO); // bind vbo of shape

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // vertex attribute pointer
	//glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // vertex normal attribute pointer
	//glEnableVertexAttribArray(1);

	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texture attribute pointer
	//glEnableVertexAttribArray(2);

	//int totalVertices = 0;

	for (int i = 0; i < shapes[shapeMap[name]].index.size(); i += 2) { // there are two indexes for each material, [startIndex, materialIndex]
		int startVertex = shapes[shapeMap[name]].index[i] * 3; // indexes are for triangles, start index must be in vertexes
		
		int nrVertexes;
		if (i + 2 >= shapes[shapeMap[name]].index.size()) { // check if i is out of bounds
			nrVertexes = shapes[shapeMap[name]].size - shapes[shapeMap[name]].index[i] * 3;
		}
		else {
			nrVertexes = shapes[shapeMap[name]].index[i + 2] * 3 - shapes[shapeMap[name]].index[i] * 3;
		}

		// set uniforms

		if (!shapes[shapeMap[name]].materials[shapes[shapeMap[name]].index[i + 1]].texture) {
			objectShader->use();

			glBindBuffer(GL_ARRAY_BUFFER, shapes[shapeMap[name]].VBO); // bind vbo of shape

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // vertex attribute pointer
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // vertex normal attribute pointer
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texture attribute pointer
			glEnableVertexAttribArray(2);

			objectShader->vec3("material.ambient", shapes[shapeMap[name]].materials[shapes[shapeMap[name]].index[i + 1]].ambient);
			objectShader->vec3("material.diffuse", shapes[shapeMap[name]].materials[shapes[shapeMap[name]].index[i + 1]].diffuse);
			objectShader->vec3("material.specular", shapes[shapeMap[name]].materials[shapes[shapeMap[name]].index[i + 1]].specular);
			objectShader->setFloat("material.smoothness", shapes[shapeMap[name]].materials[shapes[shapeMap[name]].index[i + 1]].smoothness);

			//objectShader->setFloat("light.a", 0.1f);

		}

		else {
			//std::cout << "a";
			textureShader->use();

			glBindBuffer(GL_ARRAY_BUFFER, shapes[shapeMap[name]].VBO); // bind vbo of shape

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // vertex attribute pointer
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // vertex normal attribute pointer
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texture attribute pointer
			glEnableVertexAttribArray(2);

			textures->bind(shapes[shapeMap[name]].materials[shapes[shapeMap[name]].index[i + 1]].diffuseTextureName.c_str(), 0);
			////std::cout << "TexName " << shapes[shapeMap[name]].materials[shapes[shapeMap[name]].index[i + 1]].diffuseTextureName.c_str() << std::endl;
			textures->bind(shapes[shapeMap[name]].materials[shapes[shapeMap[name]].index[i + 1]].specularTextureName.c_str(), 1);
			//textures->bind("assets/container2.png", 0);
			textureShader->vec3("material.ambient", shapes[shapeMap[name]].materials[shapes[shapeMap[name]].index[i + 1]].ambient);
			textureShader->vec3("material.diffuse", shapes[shapeMap[name]].materials[shapes[shapeMap[name]].index[i + 1]].diffuse);
			textureShader->vec3("material.specular", shapes[shapeMap[name]].materials[shapes[shapeMap[name]].index[i + 1]].specular);
			textureShader->setFloat("material.smoothness", shapes[shapeMap[name]].materials[shapes[shapeMap[name]].index[i + 1]].smoothness);
			textureShader->setInt("material.diffusetex", 0);
			textureShader->setInt("material.speculartex", 1);



		}



		glDrawArrays(GL_TRIANGLES, startVertex, nrVertexes);

		//totalVertices += nrVertexes;
	}


	/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/

	//glDrawArrays(GL_TRIANGLES, 0, shapes[shapeMap[name]].size); // 36
}


void Objects::drawLight(std::string name) {


	glBindBuffer(GL_ARRAY_BUFFER, shapes[shapeMap[name]].VBO); // bind vbo of shape

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // vertex attribute pointer
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // vertex normal attribute pointer
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texture attribute pointer
	glEnableVertexAttribArray(2);


	glDrawArrays(GL_TRIANGLES, 0, shapes[shapeMap[name]].size); // 36
}


void Objects::destroy(std::string name) {
	glDeleteBuffers(1, &shapes[shapeMap[name]].VBO);
}


void Objects::terminate() {
	glDeleteBuffers(1, &VAO);
}
