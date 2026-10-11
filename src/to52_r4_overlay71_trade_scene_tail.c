#include "global.h"

typedef struct Ov71Work {
    u8 pad00[8];
    s32 timer;
    u32 taskState;
    u8 pad10[0x20 - 0x10];
    void *sprite;
    void *bgConfig;
    u8 pad28[4];
    void *unk2C;
    u8 pad30[4];
    u8 sub34[1];
} Ov71Work;

typedef struct Ov71State {
    s32 state;
} Ov71State;

extern void ov71_02249670(Ov71Work *);
extern void ov71_022497E0(Ov71Work *);
extern void ov71_022498F0(Ov71Work *);
extern void ov71_02249C98(void *);
extern void *SysTask_CreateOnVWaitQueue(void (*)(void *), void *, u32);
extern void BeginNormalPaletteFade(u32, s32, s32, u16, s32, s32, u32);
extern void PlaySE(u16);
extern void ov71_022499A4(Ov71Work *, void *);
extern void ov71_02247704(void *, int);
extern int ov71_02249A50(void *);
extern void Sprite_SetAnimCtrlSeq(void *, int);
extern void Sprite_SetDrawFlag(void *, int);
extern int Sprite_IsAnimated(void *);
extern int IsPaletteFadeFinished(void);

extern const u32 ov71_0224BDC0[];
extern const u32 ov71_0224BD84[];
extern const u32 ov71_0224BDA4[];
extern void GfGfx_SetBanks(const void *);
extern void SetBothScreensModesAndDisable(const void *);
extern void InitBgFromTemplate(void *, int, const void *, int);
extern int TradeSequence_GetTimeOfDay(void *);
extern void GfGfxLoader_LoadCharData(int, int, void *, int, int, int, int, int);
extern void GfGfxLoader_LoadScrnData(int, int, void *, int, int, int, int, int);
extern void GfGfxLoader_GXLoadPalWithSrcOffset(int, int, int, int, int, int, int);
extern void BgSetPosTextAndCommit(void *, int, int, int);
extern void GfGfx_EngineATogglePlanes(int, int);
extern void GfGfx_EngineBTogglePlanes(int, int);

int ov71_02249558(Ov71Work *);
int ov71_022495A0(Ov71Work *, Ov71State *);

int ov71_02249558(Ov71Work *work) {
    ov71_02249670(work);
    ov71_022497E0(work);
    ov71_022498F0(work);
    work->taskState = 0;
    SysTask_CreateOnVWaitQueue(ov71_02249C98, work, 0);
    BeginNormalPaletteFade(0, 1, 1, 0x7FFF, 3, 1, 0x39);
    return 1;
}

int ov71_022495A0(Ov71Work *work, Ov71State *state) {
    switch (state->state) {
    case 0:
        work->timer = 0;
        state->state++;
        break;
    case 1:
        work->timer++;
        if (work->timer > 10) {
            PlaySE(0x6AB);
            ov71_022499A4(work, work->sub34);
            ov71_02247704(work->unk2C, 1);
            state->state++;
        }
        break;
    case 2:
        if (ov71_02249A50(work->sub34)) {
            PlaySE(0x6AC);
            Sprite_SetAnimCtrlSeq(work->sprite, 3);
            Sprite_SetDrawFlag(work->sprite, 1);
            state->state++;
        }
        break;
    case 3:
        if (!Sprite_IsAnimated(work->sprite)) {
            BeginNormalPaletteFade(0, 0, 0, 0x7FFF, 0x10, 1, 0x39);
            state->state++;
        }
        break;
    case 4:
        if (IsPaletteFadeFinished()) {
            return 1;
        }
        break;
    }
    return 0;
}

void ov71_02249670(Ov71Work *work) {
    int scrn;
    int chr;
    int pal;
    int offset;

    GfGfx_SetBanks(ov71_0224BDC0);
    {
        u32 reg = *(volatile u16 *)0x04000304;
        reg &= 0xFFFF7FFF;
        *(volatile u16 *)0x04000304 = reg;
    }
    SetBothScreensModesAndDisable(ov71_0224BD84);
    InitBgFromTemplate(work->bgConfig, 3, ov71_0224BDA4, 0);
    InitBgFromTemplate(work->bgConfig, 7, ov71_0224BDA4, 0);

    switch (TradeSequence_GetTimeOfDay(*(void **)work)) {
    default:
    case 0:
        scrn = 0xF;
        chr = 0x10;
        pal = 0x11;
        offset = 0;
        break;
    case 1:
        scrn = 0xF;
        chr = 0x10;
        pal = 0x11;
        offset = 0x20;
        break;
    case 2:
        scrn = 0xF;
        chr = 0x10;
        pal = 0x11;
        offset = 0x40;
        break;
    case 3:
        scrn = 0x18;
        chr = 0x19;
        pal = 0x1A;
        offset = 0;
        break;
    }
    GfGfxLoader_LoadCharData(0x59, chr, work->bgConfig, 3, 0, 0, 1, 0x39);
    GfGfxLoader_LoadCharData(0x59, chr, work->bgConfig, 7, 0, 0, 1, 0x39);
    GfGfxLoader_LoadScrnData(0x59, scrn, work->bgConfig, 3, 0, 0, 1, 0x39);
    GfGfxLoader_LoadScrnData(0x59, scrn, work->bgConfig, 7, 0, 0, 1, 0x39);
    GfGfxLoader_GXLoadPalWithSrcOffset(0x59, pal, 0, offset, 0, 0x20, 0x39);
    GfGfxLoader_GXLoadPalWithSrcOffset(0x59, pal, 4, offset, 0, 0x20, 0x39);
    BgSetPosTextAndCommit(work->bgConfig, 3, 3, 0x100);
    BgSetPosTextAndCommit(work->bgConfig, 7, 3, 0);
    GfGfx_EngineATogglePlanes(0x10, 1);
    GfGfx_EngineBTogglePlanes(0x10, 1);
}
