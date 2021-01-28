#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
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

    success = true;

    return data;


}



std::string getMTLfile(const char* filePath, int& success) {
    success = false;

    std::ifstream f(filePath); // generates file "f" from the filepath
    // check if file is open, if not, throw error
    if (!f.is_open()) {
        std::cout << "Error: Failed to find file to find mtl \"" << filePath << "\"\n";
        success = false; // sets success variable to false to check later
    }

    std::string line;

    while (std::getline(f, line)) { // iterate over lines in file
        if (line.empty()) continue;

        std::stringstream ss(line); // create stringstream from line data
        std::vector<std::string> linevec; // create string vector
        std::string tmp; // create temporary string for getline function
        while (std::getline(ss, tmp, ' ')) { // iterate over line, using getline with ' ' delimiter
            linevec.push_back(tmp); // add tmp to vector
        }

        if (linevec[0] == "mtllib") {
            success = true;
            std::cout << "MTL file name: " << linevec[1] << std::endl;
            return linevec[1].c_str();
        }
        else if (linevec[0] == "usemtl") {
            std::cout << "Couldn't find file for material \"" << linevec[1] << "\"" << std::endl;
            return "";
        }

    }

    std::cout << "No mtl file found, using default parameters" << std::endl;
    return "";
}



std::vector<OBJmaterial> getMatData(const char* filePath, int& success) {
    int mtlsuccess;
    std::string mtl = getMTLfile(filePath, mtlsuccess);
    if (!success) {
        // use default values
    }


    std::vector<OBJmaterial> materials;

    std::string mtlFilePath = "assets/" + mtl;


    std::ifstream m(mtlFilePath); // generates mtl file "m" from the filepath
    // check if file is open, if not, throw error
    if (!m.is_open()) {
        std::cout << "Error: Failed to find mtl file for data \"" << mtlFilePath << "\"\n";
        success = false; // sets success variable to false to check later
        materials.push_back(OBJmaterial());
        return materials;
    }
    


    int materialIndex = -1; // material index starts at -1 because it is incremented before the first element is added


    std::string line;

    while (std::getline(m, line)) { // iterate over lines in file
        if (line.empty()) continue;

        std::stringstream ss(line); // create stringstream from line data
        std::vector<std::string> linevec; // create string vector
        std::string tmp; // create temporary string for getline function
        while (std::getline(ss, tmp, ' ')) { // iterate over line, using getline with ' ' delimiter
            linevec.push_back(tmp); // add tmp to vector
        }
        
        if (linevec[0] == "newmtl") {
            ++materialIndex;
            materials.push_back(OBJmaterial());
            continue;
        }
        else if (linevec[0] == "Ka") {
            materials[materialIndex].ambient[0] = std::stof(linevec[1]);
            materials[materialIndex].ambient[1] = std::stof(linevec[2]);
            materials[materialIndex].ambient[2] = std::stof(linevec[3]);
            continue;
        }
        else if (linevec[0] == "Kd") {
            materials[materialIndex].diffuse[0] = std::stof(linevec[1]);
            materials[materialIndex].diffuse[1] = std::stof(linevec[2]);
            materials[materialIndex].diffuse[2] = std::stof(linevec[3]);
            continue;
        }
        else if (linevec[0] == "Ks") {
            materials[materialIndex].specular[0] = std::stof(linevec[1]);
            materials[materialIndex].specular[1] = std::stof(linevec[2]);
            materials[materialIndex].specular[2] = std::stof(linevec[3]);
            continue;
        }
        else if (linevec[0] == "Ns") {
            materials[materialIndex].smoothness = std::stof(linevec[1]);
        }
        else if (linevec[0] == "map_Ka") {
            materials[materialIndex].ambientFile = ("assets/" + linevec[1]).c_str();
            materials[materialIndex].texture = true;
        }
        else if (linevec[0] == "map_Kd") {
            materials[materialIndex].diffuseFile = ("assets/" + linevec[1]).c_str();
            //std::cout << "Diffuse map " << linevec[1] << "  " << linevec[1].c_str() << std::endl;
            materials[materialIndex].texture = true;
        }
        else if (linevec[0] == "map_Ks") {
            materials[materialIndex].specularFile = ("assets/" + linevec[1]).c_str();
            materials[materialIndex].texture = true;
        }
        else if (linevec[0] == "#") {
            continue;
        }
        else {
            continue;
        }
    }




    //std::ifstream f(filePath); // generates file "f" from the filepath
    //// check if file is open, if not, throw error
    //if (!f.is_open()) {
    //    std::cout << "Error: Failed to find file \"" << filePath << "\"\n";
    //    success = false; // sets success variable to false to check later
    //}



    ////std::string line;

    //while (std::getline(f, line)) { // iterate over lines in file
    //    if (line.empty()) continue;

    //    std::stringstream ss(line); // create stringstream from line data
    //    std::vector<std::string> linevec; // create string vector
    //    std::string tmp; // create temporary string for getline function
    //    while (std::getline(ss, tmp, ' ')) { // iterate over line, using getline with ' ' delimiter
    //        linevec.push_back(tmp); // add tmp to vector
    //    }



    //}

    for (int i = 0; i < materials.size(); ++i) {
        std::cout << "Texture? " << materials[i].texture << std::endl;
    }

    return materials;
}



std::vector<unsigned int> getMatIndexes(const char* filePath, int& success) {
    int mtlsuccess;
    std::string mtl = getMTLfile(filePath, mtlsuccess);
    if (!success) {
        // use default values
    }

    std::string mtlFilePath = "assets/" + mtl;

    //std::vector<std::string> materials;
    std::map<std::string, unsigned int> materials;

    std::ifstream m(mtlFilePath); // generates mtl file "m" from the filepath
    // check if file is open, if not, throw error
    if (!m.is_open()) {
        std::cout << "Error: Failed to find mtl file \"" << mtlFilePath << "\"\n";
        success = false; // sets success variable to false to check later
    }


    // add all material names in order to std::vector

    std::string line;

    while (std::getline(m, line)) { // iterate over lines in file
        if (line.empty()) continue;

        std::stringstream ss(line); // create stringstream from line data
        std::vector<std::string> linevec; // create string vector
        std::string tmp; // create temporary string for getline function
        while (std::getline(ss, tmp, ' ')) { // iterate over line, using getline with ' ' delimiter
            linevec.push_back(tmp); // add tmp to vector
        }

        if (linevec[0] == "newmtl") {
            //materials.push_back(linevec[1]);
            materials[linevec[1]] = materials.size();
        }
    }




    std::ifstream f(filePath); // generates file "f" from the filepath
    // check if file is open, if not, throw error
    if (!f.is_open()) {
        std::cout << "Error: Failed to find obj file \"" << filePath << "\"\n";
        success = false; // sets success variable to false to check later
    }




    std::vector<unsigned int> indexes; // material index array, format <start index, material index>

    unsigned int faceIndex = 0;

    while (std::getline(f, line)) { // iterate over lines in file
        if (line.empty()) continue;

        std::stringstream ss(line); // create stringstream from line data
        std::vector<std::string> linevec; // create string vector
        std::string tmp; // create temporary string for getline function
        while (std::getline(ss, tmp, ' ')) { // iterate over line, using getline with ' ' delimiter
            linevec.push_back(tmp); // add tmp to vector
        }

        if (linevec[0] == "f") {
            if (linevec.size() >= 4) {
                faceIndex += (linevec.size() - 3); // increment faceIndex by number of triangles 
            }
            else {
                std::cout << "File \"" << filePath << "\" contains invalid polygons" << std::endl;
            }
        }
        else if (linevec[0] == "usemtl") {
            indexes.push_back(faceIndex); // push starting index
            
            indexes.push_back(materials[linevec[1]]); // push index of material
        }
    }

    std::cout << "Indexes" << std::endl;
    for (int i = 0; i < indexes.size(); ++i) {
        std::cout << indexes[i] << std::endl;
    }


    return indexes;
}