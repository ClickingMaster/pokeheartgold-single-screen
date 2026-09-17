#include "overlay108_model_resource_cleanup_private.h"

void ov108_021E90C4(void *outer, void *raw) {
    u8 *p = raw;
    int i = 0;
    u8 *entry;
    u32 keys[2];
    int count;
    count = U16(p, 0xB6);
    if (count > 0) {
        entry = p + 0x64;
        do {
            ov108_021E9204(p, entry);
            ov108_021E91D4(outer, entry);
            i++;
            entry += 0x14;
        } while (i < U16(p, 0xB6));
    }
    if (PTR(p, 0xC) != NULL) {
        NNS_G3dTexReleaseTexKey(PTR(p, 0xC), &keys[1], &keys[0]);
        NNS_GfdDefaultFuncFreeTexVram(keys[1]);
        NNS_GfdDefaultFuncFreeTexVram(keys[0]);
        NNS_GfdDefaultFuncFreePlttVram(NNS_G3dPlttReleasePlttKey(PTR(p, 0xC)));
    }
    if (PTR(p, 0) != NULL) {
        Heap_Free(PTR(p, 0));
    }
    MI_CpuFill8(p, 0, 0xB8);
}

void ov108_021E9144(void *raw) {
    u8 *p = raw;
    int i;
    u8 *entry;
    int count;
    MtxFx33 matrix;
    VecFx32 translation;
    VecFx32 scale;
    i = 0;
    count = U16(p, 0xB6);
    if (count > 0) {
        entry = p + 0x64;
        do {
            ov108_021E9210(entry);
            i++;
            entry += 0x14;
        } while (i < U16(p, 0xB6));
    }
    translation.x = 0;
    translation.y = 0;
    translation.z = 0;
    scale.x = FX32_ONE;
    scale.y = FX32_ONE;
    scale.z = FX32_ONE;
    MTX_Identity33_(&matrix);
    GF3dRender_DrawModel(p + 0x10, &translation, &matrix, &scale);
}
