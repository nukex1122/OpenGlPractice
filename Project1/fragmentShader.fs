#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, TexCoords), texture(texture2, vec2(1.-TexCoords.x, TexCoords.y)), 0.2);
    //FragColor = texture(ourTexture, TexCoords) * vec4(ourColor, 1.0f);
    //FragColor = vec4(ourColor, 1.0f);
}