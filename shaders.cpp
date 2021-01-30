#include <glad/glad.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "shaders.h"
#include "terminalColors.h"

#define INFOLOG_LENGTH 512


int readFileToString(const char fileName[], std::string &data) {
    std::ifstream f(fileName); // create filename insta
    if (!f.is_open()) {
        std::cout << color::error << "Error: Failed to find file \"" << fileName << "\"" << color::std << std::endl;
        return false;
    }
    std::stringstream buffer; // create stringstream buffer instance
    buffer << f.rdbuf(); // read file to buffer
    f.close(); // close file
    //return buffer.str();
    data = buffer.str();
    return true;
}


Shader::Shader(const char* vsName, const char* fsName, int &success) {
    // read source of vertex shader and fragment shader source into std::string
    //std::string vsSource = readFileToString(vsName);
    //std::string fsSource = readFileToString(fsName);
    std::string vsSource, fsSource;
    int vsrSuccess, fsrSuccess; // []shaderReadSuccess
    vsrSuccess = readFileToString(vsName, vsSource);
    fsrSuccess = readFileToString(fsName, fsSource);

    if (!vsrSuccess || !fsrSuccess) { // check both files read correctly
        success = false;
        return;
    }

    std::cout << "Shader: ";

    // convert to const char*
    const char* vertexShaderSource = vsSource.c_str();
    const char* fragmentShaderSource = fsSource.c_str();

    // create shader instances in OpenGL
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // set source code for shaders
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    // compile shaders
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
    
    // check for errors
    int vscSuccess; // []shaderCompileSuccess
    int fscSuccess;
    char vsInfoLog[INFOLOG_LENGTH];
    char fsInfoLog[INFOLOG_LENGTH];

    // get shader compile status
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vscSuccess);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fscSuccess);

    // if errors tell the user
    if (!vscSuccess) {
        glGetShaderInfoLog(vertexShader, INFOLOG_LENGTH, NULL, vsInfoLog);
        std::cout << color::error << "\nError: vertex shader compilation failed:\n" << vsInfoLog << color::std << std::endl;
        success = false;
    }
    if (!fscSuccess) {
        glGetShaderInfoLog(fragmentShader, INFOLOG_LENGTH, NULL, fsInfoLog);
        std::cout << color::error << "\nError: fragment shader compilation failed:\n" << fsInfoLog << color::std << std::endl;
        success = false;
    }

    if (!success) return; // if compilation failed, return AFTER error messages

    std::cout << color::process << "compiled..." << std::flush;


    // Create Shader Program


    shaderProgram = glCreateProgram();

    // add and link programs
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking success
    int programSuccess;
    char programInfoLog[INFOLOG_LENGTH];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programSuccess);

    if (!programSuccess) {
        glGetProgramInfoLog(shaderProgram, INFOLOG_LENGTH, NULL, programInfoLog);
        std::cout << color::error << "\nError: Shader Program linking failed:\n" << programInfoLog << color::std << std::endl;
        success = false;
        return;
    }
    else {
        std::cout << color::success << "linked!" << color::std << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    success = true;
}


void Shader::use() {
    glUseProgram(shaderProgram);
}


void Shader::close() {
    glDeleteProgram(shaderProgram);
}


void Shader::setFloat(const char* name, float value) {
    glUniform1f(glGetUniformLocation(shaderProgram, name), value);
}

void Shader::vec2(const char* name, float value[2]) {
    glUniform2fv(glGetUniformLocation(shaderProgram, name), 1, value);
}

void Shader::vec3(const char* name, float value[3]) {
    glUniform3fv(glGetUniformLocation(shaderProgram, name), 1, value);
}

void Shader::vec4(const char* name, float value[4]) {
    glUniform4fv(glGetUniformLocation(shaderProgram, name), 1, value);
}


void Shader::setInt(const char* name, int value) {
    glUniform1i(glGetUniformLocation(shaderProgram, name), value);
}

void Shader::ivec2(const char* name, int value[2]) {
    glUniform2iv(glGetUniformLocation(shaderProgram, name), 1, value);
}

void Shader::ivec3(const char* name, int value[3]) {
    glUniform3iv(glGetUniformLocation(shaderProgram, name), 1, value);
}

void Shader::ivec4(const char* name, int value[4]) {
    glUniform4iv(glGetUniformLocation(shaderProgram, name), 1, value);
}


void Shader::setUint(const char* name, unsigned int value) {
    glUniform1ui(glGetUniformLocation(shaderProgram, name), value);
}

void Shader::uvec2(const char* name, unsigned int value[2]) {
    glUniform2uiv(glGetUniformLocation(shaderProgram, name), 1, value);
}

void Shader::uvec3(const char* name, unsigned int value[3]) {
    glUniform3uiv(glGetUniformLocation(shaderProgram, name), 1, value);
}

void Shader::uvec4(const char* name, unsigned int value[4]) {
    glUniform4uiv(glGetUniformLocation(shaderProgram, name), 1, value);
}

void Shader::mat2(const char* name, float value[4]) { // requires matrix in column major
    glUniformMatrix2fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, value);
}

void Shader::mat3(const char* name, float value[9]) {
    glUniformMatrix3fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, value);
}

void Shader::mat4(const char* name, float value[16]) {
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, value);
}