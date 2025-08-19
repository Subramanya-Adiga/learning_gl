#version 460 core
layout(location = 0) in vec3 apos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCord;

out vec4 oColor;
out vec2 oTexCord;

void main() {
    gl_Position = vec4(apos, 1.0f);
    oColor = aColor;
    oTexCord = aTexCord;
}
