#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;

uniform float offset;

out vec3 ourColor;
out vec2 TexCoords;

uniform mat4 transform;

void main()
{
    //gl_Position = vec4(offset + aPos.x, aPos.y, aPos.z, 1.0);;
    //ourColor = aColor;
    gl_Position = transform * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}