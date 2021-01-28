#version 460 core

#define MAX_POINT_LIGHTS 16
#define NR_POINT_LIGHTS 2


struct Material {
//	vec3 ambient; // glsl uses semicolons?
	sampler2D diffuse;
	sampler2D specular;
	float smoothness;
};

struct Light {
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	vec3 position;

	float a, b, c;
};

struct DirLight {
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;

	vec3 direction;
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

uniform uint nrPointLights;
uniform uint nrDirLights;

uniform Material material;
uniform Light light[16];
uniform DirLight dirlight;

//uniform sampler2D texture0;

vec3 norm;
vec3 cameraDir;


// function declarations

vec3 calcPointLight(Light light);
vec3 calcDirLight(DirLight light);


void main() {
	norm = normalize(Normal); // create normalized vertex normal
//	vec3 lightDir = normalize(light.position - FragPos); // create normalized light direction vector
	cameraDir = normalize(cameraPos - FragPos);
//	vec3 reflectDir = reflect(-lightDir, norm);
//	// the vector points from the fragment to the light source, because it will
//	// be compared to the normalized vertex normal that points away from the vertex
//
//	// calculate light intensity
//	float dist = length(light.position - FragPos);
//	float intensity = 1.0 / (light.a * dist*dist + light.b * dist + light.c);
//
//
//	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb; // create ambient fragment color
//
//	// create multiplier for diffuse scattering. Two vectors are dot producted together and
//	// if the value < 0 it is discarded, you can't have a negative light value
//	float diffMultiplier = max(dot(norm, lightDir), 0.0);
//	vec3 diffuse = light.diffuse * diffMultiplier * texture(material.diffuse, texCoord).rgb;
//
//	// create multiplier for specular reflections. compares the vector from the camer to the
//	// fragment and the reflection vector like in diffuse, but also raises it to a power to
//	// simulate the smoothness of the surface
//	float specMultiplier = pow(max(dot(cameraDir, reflectDir), 0.0), material.smoothness);
//	vec3 specular = light.specular * specMultiplier * texture(material.specular, texCoord).rgb;
//
////	vec3 outColor = objectColor * (ambient + diffuse + specular);
//	vec3 outColor = (ambient + diffuse + specular) * intensity;
//
//	FragColor = vec4(outColor, 1.0);

	vec3 outColor;

	if (nrDirLights > 0)
		outColor += calcDirLight(dirlight);

	for (uint i = 0; i < nrPointLights; ++i)
		outColor += calcPointLight(light[i]);
	

	FragColor = vec4(outColor, 1.0);



//	FragColor = texture(material.diffuse, texCoord);
}



vec3 calcPointLight(Light light) {
	vec3 lightDir = normalize(light.position - FragPos); // create normalized light direction vector
	vec3 reflectDir = reflect(-lightDir, norm);

	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb; // create ambient fragment color
	
	// calculate light intensity
	float dist = length(light.position - FragPos);
	float intensity = 1.0 / (light.a * dist*dist + light.b * dist + light.c);

	// create multiplier for diffuse scattering. Two vectors are dot producted together and
	// if the value < 0 it is discarded, you can't have a negative light value
	float diffMultiplier = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diffMultiplier * texture(material.diffuse, texCoord).rgb;

	// create multiplier for specular reflections. compares the vector from the camer to the
	// fragment and the reflection vector like in diffuse, but also raises it to a power to
	// simulate the smoothness of the surface
	float specMultiplier = pow(max(dot(cameraDir, reflectDir), 0.0), material.smoothness);
	vec3 specular = light.specular * specMultiplier * texture(material.specular, texCoord).rgb;

//	vec3 outColor = objectColor * (ambient + diffuse + specular);
	vec3 outColor = (ambient + diffuse + specular) * intensity;

	return outColor;
}




vec3 calcDirLight(DirLight light) {
	vec3 lightDir = normalize(-light.direction); // create normalized light direction vector
	vec3 reflectDir = reflect(-lightDir, norm);

	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb; // create ambient fragment color

	// create multiplier for diffuse scattering. Two vectors are dot producted together and
	// if the value < 0 it is discarded, you can't have a negative light value
	float diffMultiplier = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diffMultiplier * texture(material.diffuse, texCoord).rgb;

	// create multiplier for specular reflections. compares the vector from the camer to the
	// fragment and the reflection vector like in diffuse, but also raises it to a power to
	// simulate the smoothness of the surface
	float specMultiplier = pow(max(dot(cameraDir, reflectDir), 0.0), material.smoothness);
	vec3 specular = light.specular * specMultiplier * texture(material.specular, texCoord).rgb;

//	vec3 outColor = objectColor * (ambient + diffuse + specular);
	vec3 outColor = ambient + diffuse + specular;

	return outColor;
}