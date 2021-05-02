#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
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

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutoff;
    float outerCutoff;
  
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


#define NR_POINTS_LIGHTS 4

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINTS_LIGHTS];
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    //FragColor = mix(texture(texture1, TexCoords), texture(texture2, TexCoords), 0.2);
    //FragColor = texture(ourTexture, TexCoords) * vec4(ourColor, 1.0f);
    //FragColor = vec4(ourColor, 1.0f);
    vec3 norm = normalize(Normal);
    
    

    vec3 viewDir = normalize(viewPos-FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    
    for(int i=0;i<NR_POINTS_LIGHTS;i++){
        result+=CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    result+=CalcSpotLight(spotLight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);

}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 lightDirection = normalize(-light.direction);
    
    float diffuseValue = max(dot(normal, lightDirection), 0.0);
    
    vec3 reflectDir = reflect(-lightDirection, normal);
    float specValue = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diffuseValue * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * specValue * vec3(texture(material.specular, TexCoords));
    
    return vec3(ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDirection = normalize(light.position - fragPos);
    
    float diffuseValue = max(dot(normal, lightDirection), 0.0);
    
    vec3 reflectDir = reflect(-lightDirection, normal);
    float specValue = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diffuseValue * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * specValue * vec3(texture(material.specular, TexCoords));
    
    return (ambient + diffuse + specular) * attenuation;
}


vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDirection = normalize(light.position - fragPos);
    
    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff)/epsilon, 0.0, 1.0);
    
    float diffuseValue = max(dot(normal, lightDirection), 0.0);
    
    vec3 reflectDir = reflect(-lightDirection, normal);
    float specValue = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diffuseValue * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * specValue * vec3(texture(material.specular, TexCoords));
    
    return (ambient + diffuse + specular) * attenuation * intensity;
}