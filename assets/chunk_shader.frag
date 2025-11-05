
#version 330 core

uniform sampler2D albedo;

in float fLight;
in vec2 vTexCord;

out vec4 FragColor;

void main()
{
    vec3 light = mix(vec3(0.1, 0.1, 0.2), vec3(1), fLight);
    FragColor.rgb = texture(albedo, vTexCord).rgb * light;
    FragColor.a = 1.0;
}
