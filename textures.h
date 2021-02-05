#ifndef TEXTURES_H
#define TEXTURES_H

#include <map>
#include <string>

class Texture {
public:
	//Texture(int& success);

	// create new texture from file. add variable name
	int newTexture(
		const char* fileName,
		//std::string fileName,
		std::string varName,
		int textureUnit,
		int textureWrap,
		int textureFilter
	);

	// select active texture unit
	void active(int textureUnit);

	// bind texture to current texture unit
	void bind(std::string varName);
	void bind(std::string varName, int textureUnit);


private:
	std::map<std::string, int> textures;

	int colorSpace(int nrChannels, int& variable);

};


#endif
