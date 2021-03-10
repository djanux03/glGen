#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
void main()
{
    //FragColor = vec4(0.9f, 0.9f, 0.9f, 1.0f);
    FragColor = texture(texture1, texCoord);
    FragColor = texture(texture2, texCoord);
}