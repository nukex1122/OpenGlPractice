#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
}; 
  
uniform Material material;

out vec4 FragColor;

//in vec3 ourColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

//uniform sampler2D texture1;
//uniform sampler2D texture2;
//uniform vec3 lightPos;
uniform vec3 viewPos;

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform float time;
uniform Light light;  

void main()
{
    //FragColor = mix(texture(texture1, TexCoords), texture(texture2, TexCoords), 0.2);
    //FragColor = texture(ourTexture, TexCoords) * vec4(ourColor, 1.0f);
    //FragColor = vec4(ourColor, 1.0f);
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(light.position - FragPos);
    float diffuseValue = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diffuseValue * vec3(texture(material.diffuse, TexCoords));
    
    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float specValue = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * specValue * vec3(texture(material.specular, TexCoords));
    
    vec3 emission = vec3(texture(material.emission, TexCoords+vec2(0.0f, time/2))) * (sin(3*time)+1)/2;
    if(texture(material.specular, TexCoords).rgb != vec3(0.0f))
        emission = vec3(0.0f);
    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}