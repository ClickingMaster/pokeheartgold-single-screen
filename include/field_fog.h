#ifndef POKEHEARTGOLD_FIELD_FOG_H
#define POKEHEARTGOLD_FIELD_FOG_H

#include "global.h"

#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"

void G3X_SetFogTable(u32 *fogTable);

typedef struct FogData {
    BOOL enable;
    GXFogBlend fogMode;
    GXFogSlope fogSlope;
    int fogOffset;
    u16 color;
    u32 alpha;
    u32 fogTable[8];
} FogData;

FogData *Fog_New();
void Fog_Free(FogData **fog);
BOOL Fog_IsEnabled(FogData *fog);
GXFogSlope Fog_GetSlope(FogData *fog);
int Fog_GetOffset(FogData *fog);
u16 Fog_GetColor(FogData *fog);
void Fog_Set(FogData *fog, s32 mask, BOOL enable, GXFogBlend fogMode, GXFogSlope fogSlope, int fogOffset);
void Fog_SetColor(FogData *fog, u32 mask, u16 color, u32 alpha);
void Fog_SetTable(FogData *fog, const void *src);

#endif // POKEHEARTGOLD_FIELD_FOG_H
