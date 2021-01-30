#include <glad/glad.h>

#include <iostream>
#include "textures.h"
#include "terminalColors.h"


#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"





int Texture::colorSpace(int nrChannels, int& variable) {
    switch (nrChannels) {
    case 3:
        variable = GL_RGB;
        return true;

    case 4:
        variable = GL_RGBA;
        return true;

    default:
        std::cout << color::error << "Error: Texture uses unsupported colorspace code " << nrChannels << color::std << std::endl;
        return false;
    }
}


int Texture::newTexture(const char* fileName, std::string varName, int textureUnit, int textureWrap, int textureFilter) {
    std::cout << "Loading texture " << color::file << "\"" << fileName << "\"" << color::std << std::endl;

    glActiveTexture(GL_TEXTURE0 + textureUnit); // set active texture unit

    unsigned int texture; // create texture variable

    glGenTextures(1, &texture); // generate texture object
    glBindTexture(GL_TEXTURE_2D, texture); // bind texture to current texture unit

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrap); // set texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilter); // set pixel filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilter);

    //stbi_set_flip_vertically_on_load(true);
    int texWidth, texHeight, nrChannels;
    unsigned char* data = stbi_load(fileName, &texWidth, &texHeight, &nrChannels, 0);

    int cSpace;
    if (!colorSpace(nrChannels, cSpace)) return false;

    if (data) { // add data to texture
        glTexImage2D(GL_TEXTURE_2D, 0, cSpace, texWidth, texHeight, 0, cSpace, GL_UNSIGNED_BYTE, data); //generate textures
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << color::error << "Texture failed to load" << color::std << std::endl;
    }
    stbi_image_free(data); // free memory

    textures[varName] = texture; // create entry of variable name in map with texture id

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Texture::active(int textureUnit) {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
}

void Texture::bind(std::string varName) {
    glBindTexture(GL_TEXTURE_2D, textures[varName]);
}

void Texture::bind(std::string varName, int textureUnit) {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, textures[varName]);
}
