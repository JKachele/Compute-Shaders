#version 450 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

layout (std430, binding=0) buffer pos {
        vec4 position[];
};

void main() {
        uvec3 size = gl_NumWorkGroups * gl_WorkGroupSize;
        uvec3 id = gl_GlobalInvocationID;
        uint idx = id.x + (id.y * size.x) + (id.z * size.x * size.y);

        vec3 p = position[idx].xyz;

        p += vec3(0.01);

        position[idx].xyz = p;
}


