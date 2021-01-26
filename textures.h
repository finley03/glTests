#ifndef TEXTURES_H
#define TEXTURES_H

#include <map>

class Texture {
public:
	//Texture(int& success);

	// create new texture from file. add variable name
	int newTexture(
		const char* fileName,
		const char* varName,
		int textureUnit,
		int textureWrap,
		int textureFilter
	);

	// select active texture unit
	void active(int textureUnit);

	// bind texture to current texture unit
	void bind(const char* varName);
	void bind(const char* varName, int textureUnit);


private:
	std::map<const char*, int> textures;

	int colorSpace(int nrChannels, int& variable);

};


#endif
