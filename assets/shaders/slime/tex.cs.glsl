#version 450 core

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

uniform float deltaT;
uniform float diffuseRate = 5;
uniform float decayRate = 0.1;
uniform ivec2 direction;

layout(rgba32f, binding = 0) uniform image2D inputImg;
layout(rgba32f, binding = 1) uniform image2D outputImg;

void main() {
        ivec2 size = imageSize(outputImg);
        ivec2 imgMax = size - ivec2(1);
        ivec2 id = ivec2(gl_GlobalInvocationID.xy);
        if (id.x < 0 || id.x >= size.x || id.y < 0 || id.y >= size.y) {
                return;
        }

        vec4 origColor = imageLoad(inputImg, id);
        vec4 avg = vec4(0);
        for (int i = -1; i <= 1; i++) {
                ivec2 offset = direction * i;
                avg += imageLoad(inputImg, clamp(id + offset, ivec2(0), imgMax));
        }
        avg /= 3;

        float diffuseWeight = clamp(diffuseRate * deltaT, 0.0, 1.0);
        vec4 blurCol = (origColor * (1 - diffuseWeight)) + (avg * diffuseWeight);
        blurCol = max(vec4(0), blurCol - (decayRate * deltaT));

        imageStore(outputImg, id, blurCol);
}

