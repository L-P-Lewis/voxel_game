#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 vTexCord;
out float fLight;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    vTexCord = uv;
    vec3 light_dir = normalize(vec3(1, 2.5, 0.2));
    fLight = dot(normal, light_dir);
    fLight = (fLight + 1.0) / 2.0;
};
