#include "global.h"

#include "gf_gfx_loader.h"
#include "unk_0205B3DC.h"

#define PTR(base, offset) (*(void **)((u8 *)(base) + (offset)))
#define U32(base, offset) (*(u32 *)((u8 *)(base) + (offset)))

extern void GF_AssertFail(void);
extern u32 sub_0203769C(void);
extern void Sprite_SetAnimCtrlSeq(void *sprite, u32 sequence);
extern void Sprite_SetDrawFlag(void *sprite, u32 draw);
extern void PlaySE(u32 seqNo);
extern void GX_LoadOBJ(const void *src, u32 offset, u32 size);
extern void GX_LoadOBJPltt(const void *src, u32 offset, u32 size);
extern const u16 ov73_021EA520[];

typedef struct Ov73ProfileSlotR12 {
    void *profile;
    u32 unk4;
} Ov73ProfileSlotR12;

typedef struct Ov73WorkR12 {
    u8 pad_000[0x22C];
    void *sprites[5];
    u8 pad_240[0x2EC - 0x240];
    void *charAlloc;
    void *charAlloc2;
    void *charData;
    void *charData2;
    void *paletteAlloc;
    void *paletteAlloc2;
    void *paletteData;
    u8 pad_308[0x338 - 0x308];
    Ov73ProfileSlotR12 profiles[5];
    u32 states[5];
} Ov73WorkR12;

void ov73_021E7740(void *work, NARC *narc);
void ov73_021E77A4(void *charResource, void *paletteResource, u32 index, int trainerClass, int gender);

void ov73_021E7740(void *work, NARC *narc) {
    PTR(work, 0x2FC) = GfGfxLoader_GetPlttData((NarcId)0x64, 9, (NNSG2dPaletteData **)((u8 *)work + 0x304), HEAP_ID_50);
    PTR(work, 0x300) = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 0xB, (NNSG2dPaletteData **)((u8 *)work + 0x308), HEAP_ID_50);
    PTR(work, 0x2EC) = GfGfxLoader_GetCharData((NarcId)0x64, 0x2B, TRUE, (NNSG2dCharacterData **)((u8 *)work + 0x2F4), HEAP_ID_50);
    PTR(work, 0x2F0) = GfGfxLoader_GetCharDataFromOpenNarc(narc, 0xC, TRUE, (NNSG2dCharacterData **)((u8 *)work + 0x2F8), HEAP_ID_50);
}

void ov73_021E77A4(void *charResource, void *paletteResource, u32 index, int trainerClass, int gender) {
    int avatar = SpriteToUnionRoomAvatarIdx(gender, trainerClass);
    u8 *palette = PTR(PTR(paletteResource, 4), 0xC);
    u8 *chars = PTR(PTR(charResource, 4), 0x14);

    GX_LoadOBJ(chars + avatar * 0x600, ov73_021EA520[index], 0x600);
    GX_LoadOBJPltt(palette + avatar * 0x20, (index + 7) * 0x20, 0x20);
}
