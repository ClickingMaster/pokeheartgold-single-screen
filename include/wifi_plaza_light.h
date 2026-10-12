#ifndef POKEHEARTGOLD_WIFI_PLAZA_LIGHT_H
#define POKEHEARTGOLD_WIFI_PLAZA_LIGHT_H

#include "global.h"

#include "heap.h"

// Light and material colours of the room, each faded over 60 frames to a new colour set.
// Colours are 5-bit r, g, b components (4th byte unused).
typedef struct WiFiPlazaLight {
    u8 lights[2][4];        // 0x00 lights 0 and 1
    u8 materials[4][4];     // 0x08 diffuse, ambient, specular, emission
    u8 lightsFrom[2][4];    // 0x18
    u8 materialsFrom[4][4]; // 0x20
    u8 lightsTo[2][4];      // 0x30
    u8 materialsTo[4][4];   // 0x38
    s32 lightTimers[2];     // 0x48 frame of the fade, 60 = done
    s32 materialTimers[4];  // 0x50
} WiFiPlazaLight;

WiFiPlazaLight *WiFiPlazaLight_Create(enum HeapID heapID, u32 colorSet, u32 light1Set, u32 thirdSet);
void WiFiPlazaLight_Delete(WiFiPlazaLight *light);
void WiFiPlazaLight_Update(WiFiPlazaLight *light);
void WiFiPlazaLight_FadeToColorSet(WiFiPlazaLight *light, int set);
void WiFiPlazaLight_FadeLight1(WiFiPlazaLight *light, int set);
void ov49_02268620(WiFiPlazaLight *light, int set);
void WiFiPlazaLight_PackColor(const s8 *rgb, u16 *color);
void WiFiPlazaLight_UnpackColor(s8 *rgb, const u16 *color);
void WiFiPlazaLight_LerpColor(const s8 *start, const s8 *end, s8 *dest, s32 step, s32 total);
void WiFiPlazaLight_UnpackColorSet(s8 *dest, const u16 *colors);
void WiFiPlazaLight_UnpackLight1(WiFiPlazaLight *light, const u16 *color);
void WiFiPlazaLight_Apply(WiFiPlazaLight *light);

#endif // POKEHEARTGOLD_WIFI_PLAZA_LIGHT_H
