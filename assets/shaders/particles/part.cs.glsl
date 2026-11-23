#version 450 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

uniform vec3 blackHolePos1;
uniform vec3 blackHolePos2;
uniform float blackHoleGrav1;
uniform float blackHoleGrav2;

uniform float deltaT;

const float PartMass = 0.1f;
const float PartInvMass = 1.0 / PartMass;
const float MaxDist = 45.0f;

layout (std430, binding=0) buffer pos {
        vec4 position[];
};

layout (std430, binding=1) buffer vel {
        vec4 velocity[];
};

void main() {
        uvec3 size = gl_NumWorkGroups * gl_WorkGroupSize;
        uvec3 id = gl_GlobalInvocationID;
        uint idx = id.x + (id.y * size.x) + (id.z * size.x * size.y);

        vec3 p = position[idx].xyz;
        vec3 v = velocity[idx].xyz;

        // Force from black hole 1
        vec3 d = blackHolePos1 - p;
        float dist = length(d);
        vec3 force = (blackHoleGrav1 / dist) * normalize(d);

        // Add force from black hole 2
        d = blackHolePos2 - p;
        dist = length(d);
        force += (blackHoleGrav2 / dist) * normalize(d);

        // Use kinematics equations
        // F = ma -> a = f * (1/m)
        // x = (1/2)at^2 + vt + x0
        // v = at + v0
        vec3 acc = force * PartInvMass;
        position[idx] = vec4((0.5 * acc * deltaT * deltaT) + (v * deltaT) + p, 1.0);
        velocity[idx] = vec4((acc * deltaT) + v, 0.0);

        // Reset particle if gets too far
        if (length(p) > MaxDist) {
                position[idx] = vec4(0, 0, 0, 1);
                velocity[idx] = vec4(0, 0, 0, 0);
        }
}


