#include <fstream>
#include <iostream>
#include <sstream>
#include "obj.h"

// Generates triangle mesh suitable for glDrawArrays ie there are no incices
// the return format is vertex[3], normal[3], texture[2] therefore each vertex is 8 floats

std::vector<float> genMeshFromFile(const char* filePath, int &success, int &size) {
	std::ifstream f(filePath); // generates file "f" from the filepath
    // check if file is open, if not, throw error
    if (!f.is_open()) {
        std::cout << "Error: Failed to find file \"" << filePath << "\"\n";
        success = false; // sets success variable to false to check later
    }

    std::vector<float> vertices; // define data types to add to
    std::vector<float> vertexNormals;
    std::vector<float> textureCoords;
    std::vector<int> vertexIndices;
    std::vector<int> normalIndices;
    std::vector<int> textureIndices;

    std::string line;

    while (std::getline(f, line)) { // iterate over lines in file
        if (line.empty()) continue;

        std::stringstream ss(line); // create stringstream from line data
        std::vector<std::string> linevec; // create string vector
        std::string tmp; // create temporary string for getline function
        while (std::getline(ss, tmp, ' ')) { // iterate over line, using getline with ' ' delimiter
            linevec.push_back(tmp); // add tmp to vector
        }

        if (linevec[0] == "v") { // case for vertex
            vertices.push_back(std::stof(linevec[1])); // convert string to float and push to vertex vector
            vertices.push_back(std::stof(linevec[2]));
            vertices.push_back(std::stof(linevec[3]));
            continue;
        }
        else if (linevec[0] == "vn") { // case for vertex normals
            vertexNormals.push_back(std::stof(linevec[1]));
            vertexNormals.push_back(std::stof(linevec[2]));
            vertexNormals.push_back(std::stof(linevec[3]));
            continue;
        }
        else if (linevec[0] == "vt") { // case for texture coordinates
            textureCoords.push_back(std::stof(linevec[1]));
            textureCoords.push_back(std::stof(linevec[2]));
            continue;
        }
        else if (linevec[0] == "f") { // case for TRIANGULAR face elements
            std::string triangle[3];
            for (unsigned int i = 1; i < linevec.size() - 2; ++i) { // creates number of shapes: n = sides - 2
                triangle[0] = linevec[1];
                triangle[1] = linevec[i + 1];
                triangle[2] = linevec[i + 2];

                for (unsigned int i = 0; i < 3; ++i) { // loop through indices of triangle
                    std::stringstream indexData(triangle[i]); // generate stringstream of current working data
                    std::vector<int> indexvec; // create vector of index data format [v, vt, vn]
                    std::string tmp;
                    while (std::getline(indexData, tmp, '/')) {
                        indexvec.push_back(std::stoi(tmp)); // push indices to vector of index data
                    }

                    vertexIndices.push_back(indexvec[0] - 1); // add indexes to main data structures
                    textureIndices.push_back(indexvec[1] - 1); // subtract 1 because .obj is 1 indexed
                    normalIndices.push_back(indexvec[2] - 1);
                }

            }
            continue;
        }
        else if (linevec[0] == "#") { // case for comment
            continue;
        }
        else if (linevec[0] == "mtllib") { // case for material properties file
            continue;
        }
        else if (linevec[0] == "usemtl") { // case for use material declaration
            continue;
        }
        else if (linevec[0] == "o") { // case for object name
            std::cout << "Loading object \"" << linevec[1] << "\" from file \"" << filePath << "\"" << std::endl;
            continue;
        }
        else if (linevec[0] == "g") { // case for group name
            continue;
        }
        else if (linevec[0] == "s") { // case for smooth shading
            continue;
        }
        else {
            std::cout << "File \"" << filePath << "\" contains unknown parameter \"" << linevec[0] << "\"" << std::endl;
        }
    }

    // Now main data structures have been populated from the file, the file is no longer needed

    f.close();

    // now the function will arrange the data into one std::vector, suitable for packing into
    // a single vertex buffer object.
    // the format given does not include indices, so it creates a format suitable for glDrawArrays

    std::vector<float> data; // final data structure

    for (unsigned int i = 0; i < vertexIndices.size(); ++i) { // loops through data, +=3 because there are
        // 3 vertex coordinates per vertex data element

        data.push_back(vertices[vertexIndices[i] * 3]); // add 3 vertex coordinates
        data.push_back(vertices[vertexIndices[i] * 3 + 1]);
        data.push_back(vertices[vertexIndices[i] * 3 + 2]);

        data.push_back(vertexNormals[normalIndices[i] * 3]); // add 3 verted normal values
        data.push_back(vertexNormals[normalIndices[i] * 3 + 1]);
        data.push_back(vertexNormals[normalIndices[i] * 3 + 2]);

        data.push_back(textureCoords[textureIndices[i] * 2]); // add 2 texture coordinates
        data.push_back(textureCoords[textureIndices[i] * 2 + 1]);

    }

    size = vertexIndices.size();
    std::cout << "Size: " << size << " vertices" << std::endl;

    //std::cout << "Vertices:" << std::endl;

    //for (unsigned int i = 0; i < vertices.size(); ++i) {
    //    std::cout << vertices[i] << std::endl;
    //}

    //std::cout << "Vertex Indexes:" << std::endl;

    //for (unsigned int i = 0; i < vertexIndices.size(); ++i) {
    //    std::cout << vertexIndices[i] << std::endl;
    //}

    //std::cout << "Data:" << std::endl;

    //for (unsigned int i = 0; i < data.size(); ++i) {
    //    std::cout << data[i] << std::endl;
    //}

    success = true;

    return data;


}