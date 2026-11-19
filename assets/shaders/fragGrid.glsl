#version 450 core

in vec3 WorldPos;

out vec4 FragColor;

void main() {
        FragColor = vec4(WorldPos, 1.0);
}
