#include "global.h"
#include "constants/sndseq.h"

#define PTR(p,o) (*(void **)((u8 *)(p) + (o)))
#define U32AT(p,o) (*(u32 *)((u8 *)(p) + (o)))
#define U16AT(p,o) (*(u16 *)((u8 *)(p) + (o)))

extern void *GfGfxLoader_LoadFromNarc(int, int, int, int, int);
extern void BG_LoadScreenTilemapData(void *, u8, const void *, u32);
extern void GfGfxLoader_GXLoadPal(int, int, int, int, int, int);
extern void GfGfxLoader_LoadCharData(int, int, void *, int, int, int, int, int);
extern void GfGfxLoader_LoadScrnData(int, int, void *, int, int, int, int, int);
extern void BgTilemapRectChangePalette(void *, int, int, int, int, int, int);
extern void BgCommitTilemapBufferToVram(void *, int);
extern void *OverlayManager_GetData(void *);
extern int ListMenu_ProcessInput(void *);
extern void Sprite_SetDrawFlag(void *, int);
extern u32 GetMonIconNaixEx(u32, u32, u32);
extern void *GfGfxLoader_GetCharData(int, int, int, NNSG2dCharacterData **, int);
extern u32 GetMonIconPaletteEx(u32, u32, u32);
extern void Sprite_SetPaletteOverride(void *, int);
extern void Heap_Free(void *);
extern void PlaySE(u32);
extern u32 ov74_0223567C(void);
extern void ov74_0222D024(void *);
extern void ov74_0223563C(void);
extern void ov74_02235690(void);
extern void ov74_02235728(int, int, int, int, int, int);
extern u32 sub_02074490(void);
extern void *ov74_02235930(int, void *, int, int, int);
extern u32 ov74_0223D0BC;

typedef struct Ov74WorkR311 {
    u32 heapId;
    u8 pad0004[0x29F8];
    void *bgConfig;
} Ov74WorkR311;

void ov74_0222DB30(void *, int, int, int);
void ov74_0222DB70(Ov74WorkR311 *, int);
void ov74_0222DC60(void *, int *, int (*)(void *));
void ov74_0222DCD4(void *);

void ov74_0222DB30(void *work, int member, int bgId, int offset) {
    void *raw;
    NNSG2dScreenData *screen;
    raw = GfGfxLoader_LoadFromNarc(0x71, member, 1, U32AT(work, 0), 1);
    NNS_G2dGetUnpackedScreenData(raw, &screen);
    BG_LoadScreenTilemapData(PTR(work, 0x29fc), (u8)bgId, screen->rawData, offset);
    Heap_Free(raw);
}

void ov74_0222DB70(Ov74WorkR311 *work, int variant) {
    GfGfxLoader_GXLoadPal(0x71, 3, 0, 0, 0x100, work->heapId);
    GfGfxLoader_LoadCharData(0x71, 6, work->bgConfig, 1, 0, 0x3000, 1, work->heapId);
    switch (variant) {
    case 0:
        GfGfxLoader_LoadScrnData(0x71, 4, work->bgConfig, 1, 0, 0x600, 1, work->heapId);
        break;
    case 1:
        GfGfxLoader_LoadScrnData(0x71, 5, work->bgConfig, 1, 0, 0x600, 1, work->heapId);
        break;
    }
    GfGfxLoader_GXLoadPal(0x71, 0, 0, 0x160, 0x20, work->heapId);
    GfGfxLoader_LoadCharData(0x71, 1, work->bgConfig, 3, 0, 0x200, 1, work->heapId);
    ov74_0222DB30(work, 2, 3, 0x600);
    BgTilemapRectChangePalette(work->bgConfig, 3, 0, 0, 0x20, 0x18, 0xb);
    BgCommitTilemapBufferToVram(work->bgConfig, 3);
}

void ov74_0222DC60(void *manager, int *out, int (*callback)(void *)) {
    u32 input = ListMenu_ProcessInput(PTR(OverlayManager_GetData(manager), 0x2bc8));
    int result;
    if (input == (u32)-2) {
        goto cancel;
    }
    if (input == (u32)-1) {
        goto done;
    }
    goto selected;
cancel:
    PlaySE(SEQ_SE_DP_SELECT);
    if (callback == NULL) {
        goto done;
    }
    result = callback(manager);
    if (result == -1) {
        goto done;
    }
    *out = result;
    return;
selected:
    PlaySE(SEQ_SE_DP_SELECT);
    if (input == 0) {
        goto done;
    }
    if (input < 0x1f) {
        *out = input;
        return;
    }
    ov74_0223D0BC = input;
    result = ((int (*)(void *))input)(manager);
    if (result != -1) {
        *out = result;
    }
done:
    return;
}
