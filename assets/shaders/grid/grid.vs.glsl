#version 450 core

uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;
uniform float gridSize = 100.0f;

out vec3 WorldPos;

const vec3 Pos[4] = vec3[4](
                vec3(-1.0, -10.0, -1.0),
                vec3( 1.0, -10.0, -1.0),
                vec3( 1.0, -10.0,  1.0),
                vec3(-1.0, -10.0,  1.0));

const int Indices[6] = int[6](0, 2, 1, 2, 0, 3);

void main() {
        int Index = Indices[gl_VertexID];
        vec3 vPos3 = Pos[Index];
        vPos3.x = (vPos3.x * gridSize) + cameraPos.x;
        vPos3.z = (vPos3.z * gridSize) + cameraPos.z;

        vec4 vPos4 = vec4(vPos3, 1.0);
        gl_Position = projection * view * vPos4;

        WorldPos = vPos3;
}

