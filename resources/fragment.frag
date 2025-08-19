#version 460 core
out vec4 FragColor;

in vec4 oColor;
in vec2 oTexCord;

uniform sampler2D OutTexture;

void main() {
    FragColor = texture(OutTexture, oTexCord);
}
