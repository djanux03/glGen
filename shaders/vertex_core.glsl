#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextureCoord;
layout (location = 3) in vec2 secondTexCoord;
out vec3 ourColor;
out vec2 texCoord;
out vec2 secondTexCoord;
uniform mat4 transform; // set in code

void main()
{
	//gl_Position = transform * vec4(aPos, 1.0);
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;

	texCoord = aTextureCoord;
	secondTexCoord = secondTexCoord;
}