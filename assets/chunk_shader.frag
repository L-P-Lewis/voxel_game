
#version 330 core

uniform sampler2D albedo;

in vec2 vTexCord;

out vec4 FragColor;

void main()
{
    FragColor = texture(albedo, vTexCord);
}
