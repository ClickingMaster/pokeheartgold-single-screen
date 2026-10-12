#include "field/encounter_effect.h"

#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"

// Window wipes used by the trainer encounter effects, and BG graphics loaders shared by the effects.
// Both wipes start their tasks from a VWait task so the per-scanline HBlank handler begins on a frame boundary.

// BG0-3 and OBJ
#define WND_PLANEMASK_ALL (GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ)

// Defined in hblank_system_1.c; no header declares them yet.
typedef void (*HBlankTaskFunc)(void *, void *);
void *HBlankSystem_StartTask(HBlankSystem *hBlankSystem, HBlankTaskFunc func, void *data);
void HBlankTask_Delete(void *task);

// The screen is cut into horizontal stripes of stripeHeight lines; every other stripe slides right, the
// others slide left, until the window closes the screen.
typedef struct StripeWipe {
    EncounterEffectQuadInterp offset; // 0x00, fx32 pixels
    u32 stripeHeight;                 // 0x18
    u32 state;                        // 0x1C
    HBlankSystem *hBlankSystem;       // 0x20
    void *hBlankTask;                 // 0x24
    SysTask *task;                    // 0x28
    BOOL *done;                       // 0x2C
} StripeWipe;

// The top half of the screen shrinks towards the top left corner, the bottom half towards the bottom right.
typedef struct SplitWipe {
    EncounterEffectQuadInterp x; // 0x00, fx32 pixels
    EncounterEffectQuadInterp y; // 0x18, fx32 pixels
    u32 middleLine;              // 0x30
    u32 state;                   // 0x34
    HBlankSystem *hBlankSystem;  // 0x38
    void *hBlankTask;            // 0x3C
    SysTask *task;               // 0x40
    BOOL *done;                  // 0x44
} SplitWipe;

// The trainer effects keep the wipes as untyped pointers and declare these locally.
StripeWipe *StripeWipe_New(void);
void StripeWipe_Free(StripeWipe *wipe);
void StripeWipe_Start(EncounterEffectTask *effect, StripeWipe *wipe, u32 stripeHeight, s32 duration, s32 start, s32 end, s32 velocity);
SplitWipe *SplitWipe_New(void);
void SplitWipe_Free(SplitWipe *wipe);
void SplitWipe_Start(EncounterEffectTask *effect, SplitWipe *wipe, s32 duration, s32 velocityX, s32 velocityY);

void Task_StripeWipe_Begin(SysTask *task, void *data);
void Task_StripeWipe_Update(SysTask *task, void *data);
void StripeWipe_End(StripeWipe *wipe);
void HBlank_StripeWipe(void *unused, void *data);
void Task_SplitWipe_Begin(SysTask *task, void *data);
void Task_SplitWipe_Update(SysTask *task, void *data);
void HBlank_SplitWipe(void *unused, void *data);
void SplitWipe_End(SplitWipe *wipe);

#pragma force_active on

StripeWipe *StripeWipe_New(void) {
    StripeWipe *wipe = Heap_Alloc(HEAP_ID_FIELD1, sizeof(StripeWipe));
    memset(wipe, 0, sizeof(StripeWipe));
    G2_SetWnd0InsidePlane(WND_PLANEMASK_ALL, TRUE);
    G2_SetWnd1InsidePlane(GX_WND_PLANEMASK_NONE, FALSE);
    return wipe;
}

#pragma force_active reset

void StripeWipe_Free(StripeWipe *wipe) {
    if (wipe->hBlankTask) {
        StripeWipe_End(wipe);
    }
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    Heap_Free(wipe);
}

void StripeWipe_Start(EncounterEffectTask *effect, StripeWipe *wipe, u32 stripeHeight, s32 duration, s32 start, s32 end, s32 velocity) {
    if (wipe->hBlankTask != NULL) {
        GF_AssertFail();
    }
    effect->unk_18 = FALSE;
    wipe->hBlankSystem = effect->fieldSystem->mapSystems->hBlankSystem;
    wipe->stripeHeight = stripeHeight;
    wipe->state = 0;
    wipe->done = &effect->unk_18;
    EncounterEffectQuadInterp_Init(&wipe->offset, start, end, velocity, duration);
    if (start >= 0) {
        reg_G2_WIN0H = 0xFF - start;
        reg_G2_WIN0V = 192;
        reg_G2_WIN1H = (u8)(start + 1);
        reg_G2_WIN1V = 192;
    } else {
        reg_G2_WIN0H = (u8)(start + 0xFF);
        reg_G2_WIN0V = 192;
        reg_G2_WIN1H = (u8)(1 - start);
        reg_G2_WIN1V = 192;
    }
    SysTask_CreateOnVWaitQueue(Task_StripeWipe_Begin, wipe, 1024);
}

void Task_StripeWipe_Begin(SysTask *task, void *data) {
    StripeWipe *wipe = data;
    wipe->task = SysTask_CreateOnVWaitQueue(Task_StripeWipe_Update, wipe, 1024);
    wipe->hBlankTask = HBlankSystem_StartTask(wipe->hBlankSystem, HBlank_StripeWipe, wipe);
    SysTask_Destroy(task);
}

void Task_StripeWipe_Update(SysTask *task, void *data) {
    StripeWipe *wipe = data;
    s32 offset;

    switch (wipe->state) {
    case 0:
        if (EncounterEffectQuadInterp_Update(&wipe->offset)) {
            wipe->state++;
        }
        offset = wipe->offset.value >> FX32_SHIFT;
        if (offset >= 0) {
            reg_G2_WIN0H = 0xFF - offset;
            reg_G2_WIN0V = 192;
            reg_G2_WIN1H = (u8)(offset + 1);
            reg_G2_WIN1V = 192;
        } else {
            reg_G2_WIN0H = (u8)(offset + 0xFF);
            reg_G2_WIN0V = 192;
            reg_G2_WIN1H = (u8)(1 - offset);
            reg_G2_WIN1V = 192;
        }
        break;
    case 1:
        StripeWipe_End(wipe);
        break;
    }
}

void StripeWipe_End(StripeWipe *wipe) {
    G2_SetWnd0InsidePlane(WND_PLANEMASK_ALL, TRUE);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_NONE, FALSE);
    G2_SetWnd0Position(0, 0, 0, 0);
    GX_SetVisibleWnd(GX_WNDMASK_W0);
    *wipe->done = TRUE;
    HBlankTask_Delete(wipe->hBlankTask);
    wipe->hBlankTask = NULL;
    SysTask_Destroy(wipe->task);
    wipe->task = NULL;
}

#pragma force_active on

// Even stripes scroll every BG by +offset and show window 0, odd stripes by -offset and show window 1.
void HBlank_StripeWipe(void *unused, void *data) {
    StripeWipe *wipe = data;
    int offset;
    u32 scroll;

    if (!((reg_GX_VCOUNT / wipe->stripeHeight) & 1)) {
        offset = wipe->offset.value;
    } else {
        offset = -wipe->offset.value;
    }
    offset >>= FX32_SHIFT;
    scroll = offset & 0x1FF;
    reg_G2_BG0OFS = scroll;
    reg_G2_BG1OFS = scroll;
    reg_G2_BG2OFS = scroll;
    reg_G2_BG3OFS = scroll;
    if (offset >= 0) {
        GX_SetVisibleWnd(GX_WNDMASK_W0);
        G2_SetWndOutsidePlane(GX_WND_PLANEMASK_NONE, FALSE);
    } else {
        GX_SetVisibleWnd(GX_WNDMASK_W1);
        G2_SetWndOutsidePlane(WND_PLANEMASK_ALL, TRUE);
    }
}

#pragma force_active reset

SplitWipe *SplitWipe_New(void) {
    SplitWipe *wipe = Heap_Alloc(HEAP_ID_FIELD1, sizeof(SplitWipe));

    memset(wipe, 0, sizeof(SplitWipe));
    G2_SetWnd0InsidePlane(WND_PLANEMASK_ALL, TRUE);
    G2_SetWnd1InsidePlane(WND_PLANEMASK_ALL, TRUE);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_NONE, FALSE);
    return wipe;
}

void SplitWipe_Free(SplitWipe *wipe) {
    if (wipe->hBlankTask) {
        SplitWipe_End(wipe);
    }
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    Heap_Free(wipe);
}

void SplitWipe_Start(EncounterEffectTask *effect, SplitWipe *wipe, s32 duration, s32 velocityX, s32 velocityY) {
    if (wipe->hBlankTask != NULL) {
        GF_AssertFail();
    }
    effect->unk_18 = FALSE;
    wipe->hBlankSystem = effect->fieldSystem->mapSystems->hBlankSystem;
    wipe->middleLine = 96;
    wipe->state = 0;
    wipe->done = &effect->unk_18;
    EncounterEffectQuadInterp_Init(&wipe->x, 0, FX32_CONST(255), velocityX, duration);
    EncounterEffectQuadInterp_Init(&wipe->y, 0, FX32_CONST(96), velocityY, duration);
    reg_G2_WIN0H = 0xFF;
    reg_G2_WIN0V = 192;
    reg_G2_WIN1H = 0xFF;
    reg_G2_WIN1V = 192;
    SysTask_CreateOnVWaitQueue(Task_SplitWipe_Begin, wipe, 1024);
}

void Task_SplitWipe_Begin(SysTask *task, void *data) {
    SplitWipe *wipe = data;
    wipe->task = SysTask_CreateOnVWaitQueue(Task_SplitWipe_Update, wipe, 1024);
    wipe->hBlankTask = HBlankSystem_StartTask(wipe->hBlankSystem, HBlank_SplitWipe, wipe);
    SysTask_Destroy(task);
}

// Window 0 covers x 0..255-x and lines 0..96-y, window 1 covers x x..255 and lines 96+y..192.
void Task_SplitWipe_Update(SysTask *task, void *data) {
    SplitWipe *wipe = data;
    s32 x;
    s32 y;

    switch (wipe->state) {
    case 0:
        EncounterEffectQuadInterp_Update(&wipe->y);
        if (EncounterEffectQuadInterp_Update(&wipe->x)) {
            wipe->state++;
        }
        x = wipe->x.value >> FX32_SHIFT;
        y = wipe->y.value >> FX32_SHIFT;
        reg_G2_WIN0H = 0xFF - x;
        reg_G2_WIN0V = (u8)(96 - y);
        reg_G2_WIN1H = 0xFF | ((x << 8) & 0xFF00);
        reg_G2_WIN1V = 192 | (((y + 96) << 8) & 0xFF00);
        break;
    case 1:
        SplitWipe_End(wipe);
        break;
    }
}

// Window 0 above the middle line, window 1 below it.
void HBlank_SplitWipe(void *unused, void *data) {
    SplitWipe *wipe = data;

    if (reg_GX_VCOUNT <= wipe->middleLine) {
        GX_SetVisibleWnd(GX_WNDMASK_W0);
    } else {
        GX_SetVisibleWnd(GX_WNDMASK_W1);
    }
}

void SplitWipe_End(SplitWipe *wipe) {
    G2_SetWnd0InsidePlane(WND_PLANEMASK_ALL, TRUE);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_NONE, FALSE);
    reg_G2_WIN0H = 0;
    reg_G2_WIN0V = 0;
    GX_SetVisibleWnd(GX_WNDMASK_W0);
    *wipe->done = TRUE;
    HBlankTask_Delete(wipe->hBlankTask);
    wipe->hBlankTask = NULL;
    SysTask_Destroy(wipe->task);
    wipe->task = NULL;
}

// Loads a screen with its tiles and palette (palette slot palette, paletteCount palettes) into bgId.
void EncounterEffect_LoadBgGraphics(NARC *narc, int screenMember, int charMember, int paletteMember, int palette, int paletteCount, BgConfig *bgConfig, int bgId) {
    NNSG2dScreenData *screen;
    void *screenFile;

    GfGfxLoader_GXLoadPalFromOpenNarc(narc, paletteMember, GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)(palette * 32), paletteCount * 32, HEAP_ID_FIELD1);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, charMember, bgConfig, (GFBgLayer)bgId, 0, 0, FALSE, HEAP_ID_FIELD1);
    screenFile = GfGfxLoader_GetScrnDataFromOpenNarc(narc, screenMember, FALSE, &screen, HEAP_ID_FIELD1);
    LoadRectToBgTilemapRect(bgConfig, bgId, screen->rawData, 0, 0, screen->screenWidth / 8, screen->screenHeight / 8);
    BgTilemapRectChangePalette(bgConfig, bgId, 0, 0, screen->screenWidth / 8, screen->screenHeight / 8, palette);
    Heap_Free(screenFile);
    ScheduleBgTilemapBufferTransfer(bgConfig, bgId);
}

#pragma force_active on

// Loads the same graphics into bgId and mirrorBgId, then flips mirrorBgId's tilemap horizontally in runs of
// 16 tiles.
void EncounterEffect_LoadBgGraphicsMirrored(NARC *narc, int screenMember, int charMember, int paletteMember, int palette, int paletteCount, BgConfig *bgConfig, int bgId, int mirrorBgId) {
    u16 *tilemap;
    int row, i, right, left;
    u16 tile;

    EncounterEffect_LoadBgGraphics(narc, screenMember, charMember, paletteMember, palette, paletteCount, bgConfig, bgId);
    EncounterEffect_LoadBgGraphics(narc, screenMember, charMember, paletteMember, palette, paletteCount, bgConfig, mirrorBgId);
    tilemap = GetBgTilemapBuffer(bgConfig, mirrorBgId);
    for (row = 0, left = 0, right = 15; row < 24; left += 16, right += 16, row++) {
        for (i = 0; i < 8; i++) {
            u16 a = i + left;
            u16 b = right - i;
            tile = tilemap[a];
            tilemap[a] = tilemap[b];
            tilemap[b] = tile;
            // toggle the horizontal flip bit
            tilemap[a] = (tilemap[a] & 0xFBFF) | (((tilemap[a] >> 10) ^ 1) << 10);
            tilemap[b] = (tilemap[b] & 0xFBFF) | (((tilemap[b] >> 10) ^ 1) << 10);
        }
    }
    ScheduleBgTilemapBufferTransfer(bgConfig, mirrorBgId);
}

#pragma force_active reset
