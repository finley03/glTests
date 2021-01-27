#version 460 core


struct Material {
	vec3 ambient; // glsl uses semicolons?
	vec3 diffuse;
	vec3 specular;
	float smoothness;
};

out vec4 FragColor;

in vec2 texCoord;
in vec3 Normal;
//in vec3 lightPos;
in vec3 FragPos;

//uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform Material material;

//uniform sampler2D texture0;

void main() {
	vec3 norm = normalize(Normal); // create normalized vertex normal
	vec3 lightDir = normalize(lightPos - FragPos); // create normalized light direction vector
	vec3 cameraDir = normalize(cameraPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	// the vector points from the fragment to the light source, because it will
	// be compared to the normalized vertex normal that points away from the vertex

	vec3 ambient = lightColor * material.ambient; // create ambient fragment color

	// create multiplier for diffuse scattering. Two vectors are dot producted together and
	// if the value < 0 it is discarded, you can't have a negative light value
	float diffMultiplier = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lightColor * diffMultiplier * material.diffuse;

	// create multiplier for specular reflections. compares the vector from the camer to the
	// fragment and the reflection vector like in diffuse, but also raises it to a power to
	// simulate the smoothness of the surface
	float specMultiplier = pow(max(dot(cameraDir, reflectDir), 0.0), material.smoothness);
	vec3 specular = lightColor * specMultiplier * material.specular;

//	vec3 outColor = objectColor * (ambient + diffuse + specular);
	vec3 outColor = ambient + diffuse + specular;

	FragColor = vec4(outColor, 1.0);



//	FragColor = texture(texture0, texCoord);
}