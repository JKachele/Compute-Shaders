/*************************************************
 *File----------src/util.h
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 11, 2026 13:09:50 EST
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#define PI  3.14159265358979323846
#define PI2 6.28318530717958647693

typedef unsigned int  uint;
typedef unsigned long ulong;

typedef int8_t   int8;
typedef uint8_t  uint8;
typedef uint8_t  u8;
typedef int16_t  int16;
typedef uint16_t uint16;
typedef uint16_t u16;
typedef int32_t  int32;
typedef uint32_t uint32;
typedef uint32_t u32;
typedef int64_t  int64;
typedef uint64_t uint64;
typedef uint64_t u64;

typedef enum direction {
        FRONT,
        RIGHT,
        BACK,
        LEFT,
        UP,
        DOWN
} direction;

#endif

