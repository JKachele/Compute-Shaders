#version 450 core

in vec3 WorldPos;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform float gridSize = 100.0f;
uniform float gridMinPixelsBetweenCells = 2.0f;
uniform float gridCellSize = 0.1f;
uniform vec4 gridColorThin  = vec4(0.5f, 0.5f, 0.5f, 1.0f);
uniform vec4 gridColorThick = vec4(0.0f, 0.0f, 0.0f, 1.0f);

float log10(float x) {
        float f = log(x) / log(10.0);
        return f;
}

float satf(float x) {
        float f = clamp(x, 0.0, 1.0);
        return f;
}

vec2 satv(vec2 x) {
        vec2 v = clamp(x, vec2(0.0), vec2(1.0));
        return v;
}

float max2(vec2 v) {
        float f = max(v.x, v.y);
        return f;
}

void main() {
        vec2 dvx = vec2(dFdx(WorldPos.x), dFdy(WorldPos.x));
        vec2 dvy = vec2(dFdx(WorldPos.z), dFdy(WorldPos.z));

        float lx = length(dvx);
        float ly = length(dvy);

        vec2 dudv = vec2(lx, ly);

        float l = length(dudv);

        float lod = max(0.0, log10(l * gridMinPixelsBetweenCells / gridCellSize) + 1.0);

        float gridCellSizeLod0 = gridCellSize * pow(10.0, floor(lod));
        float gridCellSizeLod1 = gridCellSizeLod0 * 10.0f;
        float gridCellSizeLod2 = gridCellSizeLod1 * 10.0f;

        dudv *= 4.0f;

        vec2 modDivDudv = mod(WorldPos.xz, gridCellSizeLod0) / dudv;
        float lod0a = max2(vec2(1.0f) - abs(satv(modDivDudv) * 2.0f - vec2(1.0f)));

        modDivDudv = mod(WorldPos.xz, gridCellSizeLod1) / dudv;
        float lod1a = max2(vec2(1.0f) - abs(satv(modDivDudv) * 2.0f - vec2(1.0f)));

        modDivDudv = mod(WorldPos.xz, gridCellSizeLod2) / dudv;
        float lod2a = max2(vec2(1.0f) - abs(satv(modDivDudv) * 2.0f - vec2(1.0f)));

        float lodFade = fract(lod);

        vec4 color;
        if (lod2a > 0.0f) {
                color = gridColorThick;
                color.a *= lod2a;
        } else if (lod1a > 0.0f) {
                color = mix(gridColorThick, gridColorThin, lodFade);
                color.a *= lod1a;
        } else {
                color = gridColorThin;
                color.a *= (lod0a * (1.0f - lodFade));
        }

        float opacityFalloff = (1.0f - satf(length(WorldPos.xz - cameraPos.xz) / gridSize));
        color.a *= opacityFalloff;

        FragColor = color;
}

