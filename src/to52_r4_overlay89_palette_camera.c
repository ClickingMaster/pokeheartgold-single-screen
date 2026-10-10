#include "to52_r4_overlay89_palette_camera_private.h"

BOOL ov89_02259A3C(int id, int b, ManagedSprite *sprite, NARC *narc, NARC *alternate, int which, void *network) {
    void *file;
    NNSG2dCharacterData *charData;
    u8 *src1;
    u8 *src2;
    u32 dst;
    if (id == 0 || id > 0x1ed) {
        return FALSE;
    }
    if (ov89_0225C88C(id, b, (u32)network) == 1) {
        file = GfGfxLoader_LoadFromOpenNarc(alternate, id + 3, TRUE, HEAP_ID_125, TRUE);
        NNS_G2dGetUnpackedCharacterData(file, &charData);
        DC_FlushRange(charData->pRawData, 0x100);
        src2 = charData->pRawData;
        src1 = src2 + 0x80;
    } else {
        file = GfGfxLoader_LoadFromOpenNarc(narc, 0x10, FALSE, HEAP_ID_125, TRUE);
        NNS_G2dGetUnpackedCharacterData(file, &charData);
        DC_FlushRange(charData->pRawData, 0x80);
        src2 = charData->pRawData;
        src1 = src2;
        src2 += 0x40;
    }
    if (which == 1) {
        dst = 0x6400000;
    } else {
        dst = 0x6600000;
    }
    {
        NNSG2dImageProxy *proxy = Sprite_GetImageProxy(sprite->sprite);
        MIi_CpuCopy16((u16 *)src1, (u16 *)(dst + proxy->vramLocation.baseAddrOfVram[which]), 0x40);
        MIi_CpuCopy16((u16 *)src2, (u16 *)(dst + 0x40 + proxy->vramLocation.baseAddrOfVram[which]), 0x40);
    }
    Heap_Free(file);
    return TRUE;
}

void ov89_02259B00(u8 *work) {
    VecFx32 target = ov89_0225C9C0;
    fx32 dx, dy;
    Camera *camera;
    camera = Camera_New(HEAP_ID_125);
    PTR(work, 0xcc) = camera;
    Camera_Init_FromTargetDistanceAndAngle(&target, 0x7c000, &ov89_0225C9B8, 0xfa4, 0, FALSE, PTR(work, 0xcc));
    Camera_SetPerspectiveClippingPlane(FX32_ONE, 0x384000, PTR(work, 0xcc));
    Camera_SetStaticPtr(PTR(work, 0xcc));
    Math_CalcPerspectiveViewSize(Camera_GetPerspectiveAngle(PTR(work, 0xcc)), Camera_GetDistance(PTR(work, 0xcc)), 0x1555, &dx, &dy);
    *(fx32 *)(work + 0xd0) = dx;
    *(fx32 *)(work + 0xd4) = dy;
}
