#version 450 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

uniform float deltaT;
uniform float time;
uniform float speed;
uniform vec4  winSize;

const float PI  = 3.14159265358979323846;
const float PI2 = 6.28318530717958647693;

struct Slime {
        vec2 pos;
        float angle;
        float padding;
};

layout (std430, binding=0) buffer slimeBuf {
        Slime slimes[];
};

layout(rgba32f, binding = 0) uniform image2D img;

// Hash function www.cs.ubc.ca/~rbridson/docs/schechter-sca08-turbulence.pdf
uint hash(uint state) {
        state ^= 2747636419u;
        state *= 2654435769u;
        state ^= state >> 16;
        state *= 2654435769u;
        state ^= state >> 16;
        state *= 2654435769u;
        return state;
}

float hashScale(uint state) {
        return float(state) / float(0xffffffffu);
}

void main() {
        uvec3 size = gl_NumWorkGroups * gl_WorkGroupSize;
        uvec3 id = gl_GlobalInvocationID;
        uint idx = id.x + (id.y * size.x) + (id.z * size.x * size.y);

        vec2 p = slimes[idx].pos;
        float angle = slimes[idx].angle;

        uint random = hash(uint(idx + uint(time) * 100000));
        random = hash(uint(p.y * (winSize.y - winSize.x) + p.x + random));

        vec2 dir = vec2(cos(angle), sin(angle));
        p += dir * deltaT * speed;

        // If hit boundary, pick new random direction and clamp pos to boundary
        if (p.x < winSize.x || p.x > winSize.y || p.y < winSize.z || p.y > winSize.w) {
                p.x = max(winSize.x, min(winSize.y, p.x));
                p.y = max(winSize.z, min(winSize.w, p.y));
                angle = hashScale(hash(random)) * PI2;
        }

        slimes[idx].pos = p;
        slimes[idx].angle = angle;

        ivec2 texCoord = imageSize(img);
        texCoord.x = int((p.x - winSize.x) / (winSize.y - winSize.x) * texCoord.x);
        texCoord.y = int((p.y - winSize.z) / (winSize.w - winSize.z) * texCoord.y);
        vec4 value = vec4(1.0, 1.0, 1.0, 1.0);
        imageStore(img, texCoord, value);
}

