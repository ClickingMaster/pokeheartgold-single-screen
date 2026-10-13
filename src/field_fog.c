#include "field_fog.h"

#include "global.h"

FogData *Fog_New() {
    FogData *dst = Heap_Alloc(HEAP_ID_FIELD1, sizeof(FogData));
    MIi_CpuClear32(0, (void *)dst, sizeof(FogData));
    return dst;
}

void Fog_Free(FogData **fog) {
    Heap_FreeExplicit(HEAP_ID_FIELD1, *fog);
    *fog = NULL;
}

BOOL Fog_IsEnabled(FogData *fog) {
    return fog->enable;
}

GXFogSlope Fog_GetSlope(FogData *fog) {
    return fog->fogSlope;
}

int Fog_GetOffset(FogData *fog) {
    return fog->fogOffset;
}

u16 Fog_GetColor(FogData *fog) {
    return fog->color;
}

void Fog_Set(FogData *fog, s32 mask, BOOL enable, GXFogBlend fogMode, GXFogSlope fogSlope, int fogOffset) {
    if (mask & 1 << 0) {
        fog->enable = enable;
    }
    if (mask & 1 << 1) {
        fog->fogMode = fogMode;
    }
    if (mask & 1 << 2) {
        fog->fogSlope = fogSlope;
    }
    if (mask & 1 << 3) {
        fog->fogOffset = fogOffset;
    }
    G3X_SetFog(fog->enable, fog->fogMode, fog->fogSlope, fog->fogOffset);
}

void Fog_SetColor(FogData *fog, u32 mask, u16 color, u32 alpha) {
    if (mask & 1 << 4) {
        fog->color = color;
    }
    if (mask & 1 << 5) {
        fog->alpha = alpha;
    }
    reg_G3X_FOG_COLOR = fog->color | (fog->alpha << 16);
}

void Fog_SetTable(FogData *fog, const void *src) {
    MIi_CpuCopy32(src, fog->fogTable, 32);
    G3X_SetFogTable(fog->fogTable);
}
