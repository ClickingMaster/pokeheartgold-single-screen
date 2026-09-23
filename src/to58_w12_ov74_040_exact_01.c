#include "global.h"

extern u8 _0223B73C[];
extern u8 ov74_0223D454[];
extern u8 ov74_0223E2D4[];
extern void GF_RunVramTransferTasks(void);
extern void OamManager_ApplyAndResetBuffers(void);
extern void DoScheduledBgGpuUpdates(void *);
extern void GfGfxLoader_GXLoadPal(int, int, int, int, int, void *);
extern void ov74_02235728(int, int, int, int, int, int);
extern void ov74_0223589C(int, int);
extern void *ov74_02235930(int, void *, int, int, int);
extern void *Heap_Alloc(int, int);
extern void Heap_Free(void *);

extern u16 MATH_CalcCRC16(void *, void *, int);
extern void OS_GetMacAddress(u8 *);
extern void *sub_02035754(int);
extern void CRYPTO_RC4Init(void *, void *, int);
extern void CRYPTO_RC4Encrypt(void *, void *, int, void *);

extern u32 GetMonGender(void *);
extern u32 MonIsShiny(void *);
extern void GetMonSpriteCharAndPlttNarcIdsEx(void *, u16, u8, int, u8, u8, int);
extern u32 GetMonData(void *, int, void *);
extern void sub_02014494(u16, u16, void *, int, int, int, int, void *, u32, int, int, int);
extern void *Sprite_GetImageProxy(void *);
extern void *Sprite_GetPaletteProxy(void *);

void ov74_02235B14(void *sprite, void *mon, int species, int form, void *buf, u16 *ids);
void ov74_02235B14(void *sprite, void *mon, int species, int form, void *buf, u16 *ids) {
    u32 gender = GetMonGender(mon);
    u8 shiny = MonIsShiny(mon);
    GetMonSpriteCharAndPlttNarcIdsEx(ids, species, gender, 2, shiny, form, 0);
    {
        u32 personality = GetMonData(mon, 0, 0);
        sub_02014494(ids[0], ids[1], *(void **)(ov74_0223D454 + 8), 0, 0, 10, 10, buf, personality, 0, 2, species);
    }
    DC_FlushRange(buf, 0xc80);
    GXS_LoadOBJ(buf, NNS_G2dGetImageLocation((const NNSG2dImageProxy *)Sprite_GetImageProxy(sprite), NNS_G2D_VRAM_TYPE_2DSUB) + 0xc80, 0xc80);
    GfGfxLoader_GXLoadPal(ids[0], ids[2], 5, NNS_G2dGetImagePaletteLocation((const NNSG2dImagePaletteProxy *)Sprite_GetPaletteProxy(sprite), NNS_G2D_VRAM_TYPE_2DSUB) + 0x60, 0x20, *(void **)(ov74_0223D454 + 8));
}

void ov74_02235BD0(u8 *p);
void ov74_02235BD0(u8 *p) {
    ov74_02235728(0x71, 0x27, 0x24, 0x26, 0x25, 1);
    ov74_0223589C(0, 0x100000);
    *(void **)(p + 0x208) = ov74_02235930(1, *(void **)(p + 0x208), 0x80, 0, 0);
}
