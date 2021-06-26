#version 330 core

out vec4 FragColor;

//in vec3 ourColor;

in vec2 TexCoords;


uniform sampler2D texture1;
//uniform sampler2D texture2;


void main()
{
	vec4 texColor = texture(texture1, TexCoords);;
	if(texColor.a<0.1)
		discard;
	FragColor = texColor;
	
}