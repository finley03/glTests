// Texture vertex shader

#version 460 core

layout (location = 0) in vec3 aPos; // location = 0 ; vertex attribute location
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;
out vec3 Normal;
out vec3 FragPos;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0); // vec3 input for vec4 variable
	texCoord = aTexCoord;
	Normal = mat3(model) * aNormal; // vertex normal must be transformed to new value
//	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0));
	//vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
}