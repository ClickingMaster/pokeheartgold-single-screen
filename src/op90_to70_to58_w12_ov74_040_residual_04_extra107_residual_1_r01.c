#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "sprite.h"
#include "heap.h"

typedef struct SurveyWork {
    BgConfig *bgConfig;
    u32 unk04;
    u32 heapId;
    u8 unk0C[0x1FC];
    Sprite *sprite;
    u32 timer;
} SurveyWork;

// First 0x34 bytes of the survey work area (the rest is covered by later asm labels).
u32 ov74_0223D454[13];
extern u32 ov74_0223E2D4[];
extern void ov74_02235DA4(void);
extern int ov74_02235AC4(u16 kind);
extern void ov74_02235BD0(SurveyWork *work, int kind, u16 *param);
extern void ov74_02235C10(SurveyWork *work, int kind, u16 *param);
extern void ov74_02235CE4(SurveyWork *work, int kind, u16 *param);

#define sSurveyWork ((SurveyWork *)ov74_0223D454)

void ov74_02235DC4(BgConfig *bgConfig, u16 *param);

// @fn ov74_02235DC4
void ov74_02235DC4(BgConfig *bgConfig, u16 *param) {
    SurveyWork *work = sSurveyWork;
    u16 kind = *param;
    int pal = ov74_02235AC4(kind);
    void *file;
    NNSG2dScreenData *screen;

    GfGfxLoader_LoadCharData((NarcId)113, 34, bgConfig, (GFBgLayer)5, 0, 5120, TRUE, (enum HeapID)sSurveyWork->heapId);
    file = GfGfxLoader_LoadFromNarc((NarcId)113, 35, TRUE, (enum HeapID)sSurveyWork->heapId, TRUE);
    NNS_G2dGetUnpackedScreenData(file, &screen);
    BG_LoadScreenTilemapData(bgConfig, (GFBgLayer)5, (u8 *)screen + 12, 1536);
    Heap_Free(file);
    BgTilemapRectChangePalette(bgConfig, (GFBgLayer)5, 0, 0, 32, 24, (u8)(pal + 8));
    ScheduleBgTilemapBufferTransfer(bgConfig, (GFBgLayer)5);
    ov74_0223E2D4[9] = (u32)ov74_02235DA4;
    sSurveyWork->bgConfig = bgConfig;
    switch (kind) {
    case 4:
    case 5:
    case 6:
    case 11:
        ov74_02235BD0(work, kind, param);
        break;
    case 7:
        work->timer = 120;
    case 1:
    case 2:
    case 13:
        ov74_02235C10(work, kind, param);
        break;
    case 3:
    case 8:
    case 9:
    case 10:
    case 12:
    case 14:
    case 15:
        ov74_02235CE4(work, kind, param);
        break;
    case 0:
        break;
    }
    Sprite_SetDrawFlag(work->sprite, FALSE);
}
