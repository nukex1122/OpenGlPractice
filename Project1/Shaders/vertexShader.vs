#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform float offset;

out vec3 ourColor;
//out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;


void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    vec3 FragPos = vec3(view * model * vec4(aPos, 1.0));
    vec3 Normal = mat3(transpose(inverse(view*model))) * aNormal;

    float ambientStrength = 0.1;

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - FragPos);
    float diffuseValue = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diffuseValue * lightColor;
    
    float specularStrength = 0.5;
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float specValue = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * specValue * lightColor;

    vec3 result = objectColor * (ambientStrength + diffuse + specular);
    ourColor = result;

}