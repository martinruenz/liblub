#version 330 core

uniform sampler2D colorMap;

in vec2 uv;

out vec4 fragColor;

void main(void) {
    fragColor = texture(colorMap, uv);
}
