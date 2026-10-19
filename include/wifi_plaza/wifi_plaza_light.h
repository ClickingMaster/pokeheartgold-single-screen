#ifndef POKEHEARTGOLD_WIFI_PLAZA_LIGHT_H
#define POKEHEARTGOLD_WIFI_PLAZA_LIGHT_H

#include "global.h"

#include "heap.h"

// Light and material colours of the room, each faded over 60 frames to a new colour set.
// Colours are 5-bit r, g, b components (4th byte unused).
typedef struct WiFiPlazaLight {
    u8 lights[2][4];    // lights 0 and 1
    u8 materials[4][4]; // diffuse, ambient, specular, emission
    u8 lightsFrom[2][4];
    u8 materialsFrom[4][4];
    u8 lightsTo[2][4];
    u8 materialsTo[4][4];
    s32 lightTimers[2]; // frame of the fade, 60 = done
    s32 materialTimers[4];
} WiFiPlazaLight;

WiFiPlazaLight *WiFiPlazaLight_Create(enum HeapID heapID, u32 colorSet, u32 light1Set, u32 thirdSet);
void WiFiPlazaLight_UnpackColorSet(s8 *dest, const u16 *colors);
void WiFiPlazaLight_UnpackLight1(WiFiPlazaLight *light, const u16 *color);
void WiFiPlazaLight_Apply(WiFiPlazaLight *light);

#endif // POKEHEARTGOLD_WIFI_PLAZA_LIGHT_H
