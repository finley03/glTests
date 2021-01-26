#ifndef OBJ_H
#define OBJ_H

#include <string>
#include <vector>

std::vector<float> genMeshFromFile(const char* filePath, int& success, int& size);

#endif