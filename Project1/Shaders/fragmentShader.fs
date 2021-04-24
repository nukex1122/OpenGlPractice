#version 330 core
out vec4 FragColor;

//in vec3 ourColor;
//in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

//uniform sampler2D texture1;
//uniform sampler2D texture2;
uniform vec3 lightPos;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    //FragColor = mix(texture(texture1, TexCoords), texture(texture2, TexCoords), 0.2);
    //FragColor = texture(ourTexture, TexCoords) * vec4(ourColor, 1.0f);
    //FragColor = vec4(ourColor, 1.0f);
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
    FragColor = vec4(result, 1.0);
}