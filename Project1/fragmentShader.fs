#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixV;

void main()
{
    FragColor = mix(texture(texture1, TexCoords), texture(texture2, TexCoords), mixV);
    //FragColor = texture(ourTexture, TexCoords) * vec4(ourColor, 1.0f);
    //FragColor = vec4(ourColor, 1.0f);
}