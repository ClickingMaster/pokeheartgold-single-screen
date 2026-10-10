#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    int time, pad;
    u8 *data;
} Animation;

typedef struct {
    u32 pad, diffuseAmbient, specularEmission, poly;
} Material;

extern u32 GetMatColAnmValue_(u8 *, u32, int);
extern u32 GetMatColAnmuAlphaValue_(u8 *, u32, int);

static inline u32 Preserve(u32 old) {
    return (old & 0x8000) != 0;
}

void NNSi_G3dAnmCalcNsBma(Material *, Animation *, u32);

void NNSi_G3dAnmCalcNsBma(Material *out, Animation *a, u32 index) {
    u8 *data = a->data;
    int time = a->time >> 12;
    u8 *dict = data + 8;
    u32 *values;
    u32 first, second;
    u32 address = (u32)dict;
    if (address && (u32)data[9] > (u32)(u16)index) {
        u16 offset = *(u16 *)(data + 14);
        u16 stride = *(u16 *)(dict + offset);
        values = (u32 *)(dict + offset + 4 + stride * (u16)index);
    } else {
        values = NULL;
    }
    first = GetMatColAnmValue_(data, values[0], time);
    second = GetMatColAnmValue_(data, values[1], time);
    out->diffuseAmbient = (first | (second << 16)) | (Preserve(out->diffuseAmbient) << 15);
    first = GetMatColAnmValue_(data, values[3], time);
    second = GetMatColAnmValue_(data, values[2], time);
    out->specularEmission = (second | (first << 16)) | (Preserve(out->specularEmission) << 15);
    {
        u32 alpha = GetMatColAnmuAlphaValue_(data, values[4], time);
        out->poly = (out->poly & ~0x1f0000) | (alpha << 16);
    }
}
