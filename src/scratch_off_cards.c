#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "overlay_87.h"
#include "palette.h"
#include "player_data.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_system.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "string_util.h"
#include "system.h"
#include "text.h"
#include "touch_sampling_internal.h"
#include "touchscreen.h"
#include "sound_playback.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "unk_020210A0.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

#define SCRATCH_CELL_EMPTY    0xB0
#define SCRATCH_CELL_WILDCARD 4

typedef struct ScratchOffCardsSprite {
    u8 unk_00;
    u8 unk_01;
    u8 isSubScreen;
    u8 unk_03;
    u8 unk_04[8];
    Sprite *sprite;
} ScratchOffCardsSprite;

typedef struct ScratchOffCardsSpriteGfx {
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMan[4];
    SpriteResource *resObj[5][4];
} ScratchOffCardsSpriteGfx;

typedef struct ScratchOffCardsFlags {
    u8 hasPair : 1;
    u8 bounceStep : 7;
} ScratchOffCardsFlags;

typedef struct ScratchOffCardsWork {
    OverlayManager *manager;                     // 0x000
    u32 unk_004;                                 // 0x004
    u8 subState;                                 // 0x008
    u8 mode;                                     // 0x009
    u8 unk_00A[2];                               // 0x00A
    u8 result;                                   // 0x00C
    u8 timer;                                    // 0x00D
    u8 cardNo;                                   // 0x00E
    u8 delay;                                    // 0x00F
    s16 slideX;                                  // 0x010
    s16 bgScrollX;                               // 0x012
    u8 mosaic;                                   // 0x014
    u8 chosenCards[3];                           // 0x015
    u8 unk_018;                                  // 0x018
    u8 unk_019[3];                               // 0x019
    VecFx32 cardScale;                           // 0x01C
    u8 unk_028[0xC];                             // 0x028
    void *paletteFadeTask;                       // 0x034
    MsgData *msgData;                            // 0x038
    MessageFormat *msgFormat;                    // 0x03C
    String *string;                              // 0x040
    String *stringBuffer;                        // 0x044
    u8 unk_048[0x10];                            // 0x048
    BgConfig *bgConfig;                          // 0x058
    Window windows[16];                          // 0x05C
    PaletteData *paletteData;                    // 0x15C
    MessagePrinter *messagePrinter;              // 0x160
    Options *options;                            // 0x164
    SaveData *saveData;                          // 0x168
    ScratchOffCardsSpriteGfx spriteGfx;          // 0x16C
    ScratchOffCardsSprite *cardIconSprites[4];   // 0x2F8
    ScratchOffCardsSprite *cellSprites[9];       // 0x308
    ScratchOffCardsSprite *cardChoiceSprites[4]; // 0x32C
    ScratchOffCardsSprite *buttonSprite;         // 0x33C
    ScratchOffCardsSprite *unk_340;              // 0x340
    ScratchOffCardsSprite *highlightSprites[3];  // 0x344
    ScratchOffCardsSprite *resultSprites[3];     // 0x350
    ScratchOffCardsSprite *winSprite;            // 0x35C
    u8 cells[9];                                 // 0x360
    u8 unk_369;                                  // 0x369
    u16 prizes[4];                               // 0x36A
    u16 unk_372;                                 // 0x372
    u16 *resultOut;                              // 0x374
    u16 *prizeItems;                             // 0x378
    u16 *prizeKinds;                             // 0x37C
    NARC *narc;                                  // 0x380
    void *scratchLayerFile;                      // 0x384
    NNSG2dCharacterData *scratchLayer;           // 0x388
    u8 *scratchPixels;                           // 0x38C
    YesNoPrompt *yesNoPrompt;                    // 0x390
    u8 touchedCells[9];                          // 0x394
    u8 matchFrame;                               // 0x39D
    u8 matchTimer;                               // 0x39E
    u8 matchedSymbol;                            // 0x39F
    ScratchOffCardsFlags flags;                  // 0x3A0
    u8 revealedCount;                            // 0x3A1
    u8 revealed[3];                              // 0x3A2
    u8 cellRevealed[9];                          // 0x3A5
    u8 unk_3AE[10];                              // 0x3AE
    TouchOutput touch;                           // 0x3B8
    u8 scratchMask[160][240];                    // 0x3FA
    u8 unk_99FA[2];                              // 0x99FA
} ScratchOffCardsWork;

// Some loops walk the work struct itself one pointer at a time and read sprite arrays through it
// (cursor->cellSprites[0] is cellSprites[i] after i steps); written this way to keep the original code.
#define NEXT_SPRITE_SLOT(cursor) ((ScratchOffCardsWork *)((u8 *)(cursor) + sizeof(void *)))

typedef struct ScratchOffCardsPos {
    s16 x;
    s16 y;
} ScratchOffCardsPos;

typedef struct ScratchOffCardsSpritePos {
    u16 x;
    u16 y;
} ScratchOffCardsSpritePos;

typedef struct ScratchOffCardsBounds {
    u8 top;
    u8 bottom;
    u8 left;
    u8 right;
} ScratchOffCardsBounds;

// Palette fade helper (no public header declares the heap argument).
void *PaletteBlink_New(PaletteData *paletteData, u32 buffer, u32 palette, enum HeapID heapID);
void PaletteBlink_Delete(void *task);

extern const TouchscreenHitbox ov87_021E8184[2];
extern const TouchscreenHitbox ov87_021E818C[2];
extern const ScratchOffCardsSpritePos ov87_021E8194[3];
extern const ScratchOffCardsPos ov87_021E81A0[4];
extern const ScratchOffCardsPos ov87_021E81A2[]; // ov87_021E81A0[0].y
extern const GraphicsModes ov87_021E81B0;
extern const TouchscreenHitbox ov87_021E81C0[];
extern const BgTemplate ov87_021E81D4;
extern const BgTemplate ov87_021E81F0;
extern const BgTemplate ov87_021E820C;
extern const BgTemplate ov87_021E8228;
extern const BgTemplate ov87_021E8244;
extern const BgTemplate ov87_021E8260;
extern const s8 ov87_021E827C[][8];
extern const ScratchOffCardsBounds ov87_021E829C[9];
extern const s16 ov87_021E82C0[];
extern const s16 ov87_021E82C2[]; // ov87_021E82C0[1]
extern const ScratchOffCardsPos ov87_021E82E4[9];
extern const TouchscreenHitbox ov87_021E8308[10];
extern const GraphicsBanks ov87_021E8330;
extern const u16 ov87_021E8358[];
extern const u8 ov87_021E8388[];
extern const s8 ov87_021E838C[];
extern const u8 ov87_021E8394[4];
extern const ObjCharTransferTemplate ov87_021E8398;
extern const VecFx32 ov87_021E83A8;
extern const WindowTemplate ov87_021E842C[16];
extern u8 ov87_021E84C0[8];

int ScratchOffCards_FadeIn(ScratchOffCardsWork *work);
int ScratchOffCards_SlideInCardChoices(ScratchOffCardsWork *work);
int ScratchOffCards_SelectCard(ScratchOffCardsWork *work);
int ScratchOffCards_ZoomSelectedCard(ScratchOffCardsWork *work);
int ScratchOffCards_ShowCard(ScratchOffCardsWork *work);
int ScratchOffCards_PlayCard(ScratchOffCardsWork *work);
int ScratchOffCards_SlideOutCard(ScratchOffCardsWork *work);
int ScratchOffCards_FadeOut(ScratchOffCardsWork *work);
void ScratchOffCards_CreateHighlightSprites(ScratchOffCardsWork *work);
void ScratchOffCards_CreateResultSprites(ScratchOffCardsWork *work);
void ScratchOffCards_CreateButtonSprites(ScratchOffCardsWork *work);
void ScratchOffCards_DeleteCardChoiceSprites(ScratchOffCardsWork *work);
void ScratchOffCards_FreeResources(ScratchOffCardsWork *work);
void ScratchOffCards_ResetDisplay(void);
void ScratchOffCards_InitResources(ScratchOffCardsWork *work);
void ScratchOffCards_CreateYesNoPrompt(ScratchOffCardsWork *work);
void ScratchOffCards_ShowYesNoPrompt(ScratchOffCardsWork *work);
void ScratchOffCards_DestroyYesNoPrompt(ScratchOffCardsWork *work);
void ScratchOffCards_InitBgGraphics(ScratchOffCardsWork *work);
void ScratchOffCards_InitSpriteGraphics(ScratchOffCardsWork *work);
void ScratchOffCards_FreeBgConfig(BgConfig *bgConfig);
void ScratchOffCards_VBlank(ScratchOffCardsWork *work);
void ScratchOffCards_SetVramBanks(void);
void ScratchOffCards_InitBgs(BgConfig *bgConfig);
void ov87_021E6DBC(ScratchOffCardsWork *work, GFBgLayer bgId);
void ov87_021E6E00(ScratchOffCardsWork *work, GFBgLayer bgId);
void ov87_021E6E44(void);
void ov87_021E6E78(void);
void ov87_021E6EB0(ScratchOffCardsWork *work, GFBgLayer bgId);
void ov87_021E6EF4(ScratchOffCardsWork *work, GFBgLayer bgId);
int ScratchOffCards_PrintMessage(ScratchOffCardsWork *work, Window *window, int msgNo, u32 x, u32 y, u32 textSpeed, u8 fg, u8 shadow, u8 bg, u8 fontId);
int ScratchOffCards_PrintMessageCentered(ScratchOffCardsWork *work, Window *window, int msgNo, u32 x, u32 y, u32 textSpeed, u8 fg, u8 shadow, u8 bg, u8 fontId);
int ScratchOffCards_PrintStopPrompt(ScratchOffCardsWork *work);
int ScratchOffCards_PrintSelectCardMessage(ScratchOffCardsWork *work);
int ScratchOffCards_PrintStartCardMessage(ScratchOffCardsWork *work);
int ScratchOffCards_ShowResult(ScratchOffCardsWork *work);
int ScratchOffCards_PrintTooBadMessage(ScratchOffCardsWork *work);
int ScratchOffCards_PrintNextCardButton(ScratchOffCardsWork *work);
int ScratchOffCards_PrintCancelButton(ScratchOffCardsWork *work);
int ScratchOffCards_PrintPrizeName(ScratchOffCardsWork *work, int prizeNo);
void ScratchOffCards_BufferNumber(ScratchOffCardsWork *work, int fieldNo, int number);
void ScratchOffCards_SetState(ScratchOffCardsWork *work, int *state, u32 newState);
void ScratchOffCards_PlaceWildcards(ScratchOffCardsWork *work);
void ScratchOffCards_FillCells(ScratchOffCardsWork *work);
void ScratchOffCards_ShuffleCells(ScratchOffCardsWork *work, int unused);
void ScratchOffCards_ClearCells(ScratchOffCardsWork *work);
void ScratchOffCards_ChoosePrizes(ScratchOffCardsWork *work, int unused);
int ScratchOffCards_BounceCardChoice(ScratchOffCardsWork *work, int cardNo);
void ScratchOffCards_SetBgPalette(ScratchOffCardsWork *work, int bgId, u8 palette, u8 width, u8 height);
u8 ScratchOffCards_CountTouchedCells(ScratchOffCardsWork *work);
void ScratchOffCards_ClearTouchedCells(ScratchOffCardsWork *work);
void ScratchOffCards_ClearRevealedCells(ScratchOffCardsWork *work);
void ScratchOffCards_ScratchCell(ScratchOffCardsWork *work, int cellNo);
void ScratchOffCards_MarkTouchedArea(ScratchOffCardsWork *work);
void ScratchOffCards_MarkArea(ScratchOffCardsWork *work, int x, int y);
int ScratchOffCards_CheckCellRevealed(ScratchOffCardsWork *work, u8 cellNo);
void ScratchOffCards_AnimateMatch(ScratchOffCardsWork *work);
int ScratchOffCards_CheckMatch(ScratchOffCardsWork *work);
u32 ScratchOffCards_StartWildcardMosaic(ScratchOffCardsWork *work);
int ScratchOffCards_UpdateWildcardMosaic(ScratchOffCardsWork *work, int fadeOut);
void ov87_021E7990(ScratchOffCardsWork *work);
void ov87_021E7998(ScratchOffCardsWork *work);
void ScratchOffCards_HideStartCardMessage(ScratchOffCardsWork *work);
void ScratchOffCards_LoadScratchLayer(ScratchOffCardsWork *work);
void ScratchOffCards_AllocScratchPixels(ScratchOffCardsWork *work);
void ScratchOffCards_ResetScratchPixels(ScratchOffCardsWork *work);
void ScratchOffCards_ScratchAt(ScratchOffCardsWork *work, int x, int y);
void ScratchOffCards_ClearScratchPixel(ScratchOffCardsWork *work, u32 x, u32 y);
void ScratchOffCardsSpriteGfx_Init(ScratchOffCardsSpriteGfx *gfx);
void ov87_021E7BC4(ScratchOffCardsSpriteGfx *gfx, u32 unused);
void ov87_021E7C50(ScratchOffCardsSpriteGfx *gfx, u32 slot, u32 id, u32 vram);
void ov87_021E7CE0(ScratchOffCardsSpriteGfx *gfx, u32 unused);
void ov87_021E7D6C(ScratchOffCardsSpriteGfx *gfx, u32 unused);
Sprite *ScratchOffCardsSpriteGfx_CreateSprite(ScratchOffCardsSpriteGfx *gfx, int resId, int animSeq, int priority, int drawPriority, u8 isSubScreen);
void ScratchOffCardsSpriteGfx_Free(ScratchOffCardsSpriteGfx *gfx);
void ScratchOffCardsSpriteGfx_InitTransfer(void);
void ScratchOffCardsSpriteGfx_ReplaceMatchPalette(ScratchOffCardsSpriteGfx *gfx, int frame);
ScratchOffCardsSprite *ScratchOffCardsSprite_New(ScratchOffCardsSpriteGfx *gfx, int resId, int animSeq, u16 x, u16 y, u8 isSubScreen, int priority, int drawPriority);
int ScratchOffCardsSprite_Delete(ScratchOffCardsSprite *sprite);
void ScratchOffCardsSprite_SetVisible(ScratchOffCardsSprite *sprite, int visible);
void ScratchOffCardsSprite_SetDrawPriority(ScratchOffCardsSprite *sprite, int priority);
void ScratchOffCardsSprite_SetPosition(ScratchOffCardsSprite *sprite, int x, int y);
void ScratchOffCardsSprite_GetPosition(ScratchOffCardsSprite *sprite, int *x, int *y);
void ScratchOffCardsSprite_SetAnim(ScratchOffCardsSprite *sprite, int seq);
void ScratchOffCardsSprite_ResetScale(ScratchOffCardsSprite *sprite, int unused);
void ScratchOffCardsSprite_SetScale(ScratchOffCardsSprite *sprite, VecFx32 *scale);
void ScratchOffCardsSprite_SetPalette(ScratchOffCardsSprite *sprite, int palette);
void ScratchOffCardsSprite_SetAnimActive(ScratchOffCardsSprite *sprite, int active);
void ScratchOffCardsSprite_SetMosaic(ScratchOffCardsSprite *sprite, int mosaic);
int ScratchOffCardsSprite_IsAnimated(ScratchOffCardsSprite *sprite);
void ScratchOffCards_AddWindows(BgConfig *bgConfig, Window *windows);
void ScratchOffCards_RemoveWindows(Window *windows);
void ScratchOffCards_DrawMessageFrame(BgConfig *bgConfig, Window *window);
void ScratchOffCards_DrawWindowFrame(Window *window, int frame);

#pragma force_active on
BOOL ScratchOffCards_Init(OverlayManager *man, int *state) {
    ScratchOffCardsWork *work;
    ScratchOffCardsArgs *args;
    int i;
    ScratchOffCards_ResetDisplay();
    Heap_Create(HEAP_ID_3, HEAP_ID_122, 0x48000);
    work = OverlayManager_CreateAndGetData(man, sizeof(ScratchOffCardsWork), HEAP_ID_122);
    memset(work, 0, sizeof(ScratchOffCardsWork));
    work->bgConfig = BgConfig_Alloc(HEAP_ID_122);
    work->manager = man;
    args = OverlayManager_GetArgs(man);
    work->saveData = args->saveData;
    work->mode = args->filler_04[0];
    work->resultOut = (u16 *)args->filler_14;
    work->options = Save_PlayerData_GetOptionsAddr(work->saveData);
    work->prizeItems = args->unk_08;
    work->prizeKinds = args->unk_0E;
    ScratchOffCards_ClearCells(work);
    for (i = 0; i < 3; i++) {
        work->prizeItems[i] = 0;
        work->prizeKinds[i] = 0;
    }
    work->matchFrame = 0;
    ScratchOffCards_InitResources(work);
    *state = 0;
    Sound_SetSceneAndPlayBGM(66, 0, 0);
    return TRUE;
}
#pragma force_active reset

#pragma force_active on
BOOL ScratchOffCards_Main(OverlayManager *man, int *state) {
    ScratchOffCardsWork *work = OverlayManager_GetData(man);
    switch (*state) {
    case 0:
        if (ScratchOffCards_FadeIn(work) == TRUE) {
            ScratchOffCards_SetState(work, state, 1);
        }
        break;
    case 1:
        if (ScratchOffCards_SlideInCardChoices(work) == TRUE) {
            ScratchOffCards_SetState(work, state, 2);
        }
        break;
    case 2:
        if (ScratchOffCards_SelectCard(work) == TRUE) {
            ScratchOffCards_SetState(work, state, 3);
        }
        break;
    case 3:
        if (ScratchOffCards_ZoomSelectedCard(work) == TRUE) {
            if (work->cardNo >= 3) {
                work->cardNo = 0;
                ScratchOffCards_DeleteCardChoiceSprites(work);
                ScratchOffCards_CreateHighlightSprites(work);
                ScratchOffCards_CreateResultSprites(work);
                ScratchOffCards_SetState(work, state, 4);
            } else {
                ScratchOffCards_SetState(work, state, 1);
            }
        }
        break;
    case 4:
        if (ScratchOffCards_ShowCard(work) == TRUE) {
            ScratchOffCards_SetState(work, state, 5);
        }
        break;
    case 5:
        if (ScratchOffCards_PlayCard(work) == TRUE) {
            if (work->cardNo >= 3) {
                ScratchOffCards_SetState(work, state, 7);
            } else {
                ScratchOffCards_SetState(work, state, 6);
            }
        }
        break;
    case 6:
        if (ScratchOffCards_SlideOutCard(work) == TRUE) {
            ScratchOffCards_SetState(work, state, 4);
        }
        break;
    case 7:
        if (ScratchOffCards_FadeOut(work) == TRUE) {
            return TRUE;
        }
        break;
    }
    SpriteList_RenderAndAnimateSprites(work->spriteGfx.spriteList);
    return FALSE;
}
#pragma force_active reset

BOOL ScratchOffCards_Exit(OverlayManager *man, int *state) {
    ScratchOffCardsWork *work = OverlayManager_GetData(man);
    sub_02021238();
    *work->resultOut = work->result;
    GF_DestroyVramTransferManager();
    ScratchOffCards_FreeResources(work);
    OverlayManager_FreeData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_122);
    return TRUE;
}

int ScratchOffCards_FadeIn(ScratchOffCardsWork *work) {
    switch (work->subState) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 3, HEAP_ID_122);
        work->subState++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            work->cardNo = 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

#pragma force_active on
int ScratchOffCards_SlideInCardChoices(ScratchOffCardsWork *work) {
    int i;
    switch (work->subState) {
    case 0:
        work->slideX = -256;
        for (i = 0; i < 4; i++) {
            ScratchOffCardsSprite_SetPosition(work->cardChoiceSprites[i], work->slideX + ov87_021E81A0[i].x, ov87_021E81A0[i].y);
            ScratchOffCardsSprite_SetDrawPriority(work->cardChoiceSprites[i], i + 20);
        }
        ScratchOffCards_DrawWindowFrame(&work->windows[15], Options_GetFrame(work->options));
        ScratchOffCards_PrintSelectCardMessage(work);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
        work->bgScrollX = 3; // used here as the index of the card sliding in
        PlaySE(SEQ_SE_PL_SYU03);
        work->subState = 1;
        break;
    case 1:
        work->slideX += 32;
        ScratchOffCardsSprite_SetPosition(work->cardChoiceSprites[work->bgScrollX], work->slideX + ov87_021E81A0[work->bgScrollX].x, ov87_021E81A2[work->bgScrollX].x);
        if (work->slideX >= 0) {
            if (work->bgScrollX == 0) {
                work->subState = 2;
            } else {
                PlaySE(SEQ_SE_PL_SYU03);
                work->bgScrollX--;
                work->slideX = -256;
            }
        }
        break;
    case 2:
        work->slideX = 0;
        return TRUE;
    }
    return FALSE;
}
#pragma force_active reset

int ScratchOffCards_SelectCard(ScratchOffCardsWork *work) {
    int hit;
    u32 x, y;
    switch (work->subState) {
    case 0:
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        work->delay = 30;
        work->subState = 1;
        break;
    case 1:
        if (work->delay == 0) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            ScratchOffCardsSprite_SetVisible(work->unk_340, FALSE);
        } else {
            work->delay--;
        }
        hit = TouchscreenHitbox_FindRectAtTouchHeld(ov87_021E81C0);
        if (hit != -1) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            ScratchOffCardsSprite_SetVisible(work->unk_340, FALSE);
            PlaySE(SEQ_SE_DP_BUTTON9);
            work->chosenCards[work->cardNo] = hit;
            work->cardNo++;
            System_GetTouchHeldCoords(&x, &y);
            ClearFrameAndWindow2(&work->windows[15], FALSE);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
            work->subState = 2;
        }
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}

int ScratchOffCards_ZoomSelectedCard(ScratchOffCardsWork *work) {
    int index = work->cardNo;
    int selected;
    index--;
    selected = work->chosenCards[index];
    switch (work->subState) {
    case 0:
        work->cardScale.x = FX32_ONE;
        work->cardScale.y = FX32_ONE;
        work->cardScale.z = FX32_ONE;
        work->slideX = 0;
        work->flags.bounceStep = 0;
        ScratchOffCardsSprite_SetDrawPriority(work->cardChoiceSprites[selected], 0);
        work->subState = 1;
        break;
    case 1:
        ScratchOffCards_BounceCardChoice(work, (u8)selected);
        work->cardScale.x += 0x100;
        work->cardScale.y += 0x100;
        work->cardScale.z += 0x100;
        work->slideX++;
        ScratchOffCardsSprite_SetScale(work->cardChoiceSprites[selected], &work->cardScale);
        if (work->cardScale.x == 0x1800) {
            work->slideX = 0;
            work->cardScale.x = FX32_ONE;
            work->cardScale.y = FX32_ONE;
            work->cardScale.z = FX32_ONE;
            PlaySE(SEQ_SE_PL_SYU03);
            work->subState = 2;
        }
        break;
    case 2: {
        int i;
        work->slideX += 16;
        i = 0;
        for (; i < 4; i++) {
            int x, y;
            ScratchOffCardsSprite_GetPosition(work->cardChoiceSprites[i], &x, &y);
            ScratchOffCardsSprite_SetPosition(work->cardChoiceSprites[i], x + 16, y);
        }
        if (work->slideX >= 256) {
            ScratchOffCardsSprite_ResetScale(work->cardChoiceSprites[selected], 0);
            work->subState = 3;
        }
        break;
    }
    case 3:
        work->slideX = 0;
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on
int ScratchOffCards_ShowCard(ScratchOffCardsWork *work) {
    int i;
    switch (work->subState) {
    case 0:
        ScratchOffCards_ResetScratchPixels(work);
        BG_LoadCharTilesData(work->bgConfig, GF_BG_LYR_MAIN_0, work->scratchPixels, work->scratchLayer->szByte, 0);
        BgCommitTilemapBufferToVram(work->bgConfig, GF_BG_LYR_MAIN_0);
        work->bgScrollX = 256;
        BgSetPosTextAndCommit(work->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_X, work->bgScrollX);
        BgSetPosTextAndCommit(work->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, work->bgScrollX);
        BgSetPosTextAndCommit(work->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, work->bgScrollX);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        work->slideX = -256;
        work->unk_019[work->cardNo] = 0;
        for (i = 0; i < 0x9600; i++) {
            ((u8 *)((u32)work + i))[offsetof(ScratchOffCardsWork, scratchMask)] = 0;
        }
        work->revealedCount = 0;
        ScratchOffCards_ClearCells(work);
        ScratchOffCards_ShuffleCells(work, work->unk_019[work->cardNo]);
        ScratchOffCards_ChoosePrizes(work, work->unk_019[work->cardNo]);
        ScratchOffCards_ClearTouchedCells(work);
        ScratchOffCards_ClearRevealedCells(work);
        work->subState = 1;
        break;
    case 1: {
        int j = 0;
        int y = 26;
        ScratchOffCardsWork *cursor = work; // advanced by one sprite pointer per iteration
        for (; j < 4; j++) {
            ScratchOffCardsSprite_SetPosition(cursor->cardIconSprites[0], work->slideX + 60, y);
            ScratchOffCardsSprite_SetVisible(cursor->cardIconSprites[0], TRUE);
            y += 42;
            cursor = NEXT_SPRITE_SLOT(cursor);
        }
        {
            int k = 0;
            cursor = work;
            for (; k < 3; k++) {
                ScratchOffCardsSprite_SetPalette(cursor->highlightSprites[0], 1);
                cursor = NEXT_SPRITE_SLOT(cursor);
            }
        }
        {
            int k = 0;
            const s16 *pos = &ov87_021E82E4[0].x;
            work->matchFrame = k;
            work->flags.hasPair = FALSE;
            work->revealed[0] = 177;
            work->revealed[1] = 178;
            work->revealed[2] = 179;
            cursor = work;
            for (; k < 9; k++) {
                ScratchOffCardsSprite_SetPosition(cursor->cellSprites[0], work->slideX + pos[0], pos[1]);
                ScratchOffCardsSprite_SetVisible(cursor->cellSprites[0], TRUE);
                ScratchOffCardsSprite_SetAnim(cursor->cellSprites[0], work->cells[k]);
                pos += 2;
                cursor = NEXT_SPRITE_SLOT(cursor);
            }
        }
        ScratchOffCards_SetBgPalette(work, GF_BG_LYR_MAIN_1, (u8)(work->chosenCards[work->cardNo] + 4), 32, 24);
        ScratchOffCards_SetBgPalette(work, GF_BG_LYR_SUB_2, (u8)(work->chosenCards[work->cardNo] + 1), 32, 24);
        PlaySE(SEQ_SE_PL_SYU03);
        work->subState = 2;
        break;
    }
    case 2:
        BgSetPosTextAndCommit(work->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_X, work->bgScrollX);
        BgSetPosTextAndCommit(work->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, work->bgScrollX);
        BgSetPosTextAndCommit(work->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, work->bgScrollX);
        work->bgScrollX -= 16;
        if (work->slideX >= 0) {
            work->subState = 3;
        } else {
            int y;
            ScratchOffCardsWork *cursor;
            const s16 *pos;
            i = 0;
            y = 26;
            cursor = work;
            for (; i < 4; i++) {
                ScratchOffCardsSprite_SetPosition(cursor->cardIconSprites[0], work->slideX + 60, y);
                y += 42;
                cursor = NEXT_SPRITE_SLOT(cursor);
            }
            pos = &ov87_021E82E4[0].x;
            i = 0;
            cursor = work;
            for (; i < 9; i++) {
                ScratchOffCardsSprite_SetPosition(cursor->cellSprites[0], work->slideX + pos[0], pos[1]);
                pos += 2;
                cursor = NEXT_SPRITE_SLOT(cursor);
            }
        }
        work->slideX += 16;
        break;
    case 3:
        work->slideX = 0;
        work->bgScrollX = 0;
        return TRUE;
    }
    return FALSE;
}
#pragma force_active reset

#pragma force_active on
int ScratchOffCards_PlayCard(ScratchOffCardsWork *work) {
    int i;
    int hit;
    switch (work->subState) {
    case 0:
        if (work->cardNo == 2) {
            ClearWindowTilemapAndCopyToVram(&work->windows[13]);
            ScratchOffCards_PrintCancelButton(work);
            ScratchOffCardsSprite_SetPosition(work->buttonSprite, 88, 160);
            ScratchOffCardsSprite_SetAnim(work->buttonSprite, 0);
        } else {
            ClearWindowTilemapAndCopyToVram(&work->windows[14]);
            ScratchOffCards_PrintNextCardButton(work);
        }
        work->delay = 30;
        ScratchOffCardsSprite_SetVisible(work->unk_340, TRUE);
        ScratchOffCards_PrintStartCardMessage(work);
        for (i = 0; i < 4; i++) {
            ScratchOffCards_PrintPrizeName(work, (u8)i);
        }
        ScratchOffCardsSprite_SetVisible(work->buttonSprite, TRUE);
        ScratchOffCardsSprite_SetAnimActive(work->buttonSprite, FALSE);
        PlaySE(SEQ_SE_PL_UG_020);
        work->subState = 1;
        break;
    case 1:
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
        work->subState = 2;
        break;
    case 2:
        if (work->delay == 0) {
            ScratchOffCardsSprite_SetVisible(work->unk_340, FALSE);
            FillWindowPixelBuffer(&work->windows[10], 0);
            ScheduleWindowCopyToVram(&work->windows[10]);
            work->subState = 3;
        } else {
            work->delay--;
        }
        break;
    case 3:
        sub_02021280(&work->touch, 4, 2);
        hit = TouchscreenHitbox_FindRectAtTouchHeld(ov87_021E8308);
        if (hit != -1) {
            if (ScratchOffCards_CountTouchedCells(work) < 3) {
                work->touchedCells[hit] = TRUE;
            }
            if (work->touchedCells[hit] == TRUE) {
                ScratchOffCards_MarkTouchedArea(work);
                ScratchOffCards_CheckCellRevealed(work, hit);
                ScratchOffCards_ScratchCell(work, hit);
                if (!IsSEPlaying(SEQ_SE_PL_KEZURI)) {
                    PlaySE(SEQ_SE_PL_KEZURI);
                }
            }
            if (ScratchOffCards_CheckMatch(work) == TRUE) {
                StopSE(SEQ_SE_PL_KEZURI, 0);
                ScratchOffCardsSprite_SetVisible(work->unk_340, TRUE);
                work->prizeItems[work->cardNo] = work->prizes[work->matchedSymbol];
                if (work->prizes[work->matchedSymbol] == 92) {
                    work->prizeKinds[work->cardNo] = 1;
                } else {
                    work->prizeKinds[work->cardNo] = 3;
                }
                work->delay = ScratchOffCards_StartWildcardMosaic(work) == TRUE ? 30 : 0;
                ScratchOffCards_ShowResult(work);
                work->subState = 4;
                return FALSE;
            }
            if (work->revealedCount >= 3) {
                StopSE(SEQ_SE_PL_KEZURI, 0);
                work->delay = 60;
                ScratchOffCardsSprite_SetVisible(work->unk_340, TRUE);
                ScratchOffCards_PrintTooBadMessage(work);
                work->subState = 7;
                return FALSE;
            }
        } else {
            StopSE(SEQ_SE_PL_KEZURI, 0);
        }
        if (work->flags.hasPair == TRUE) {
            ScratchOffCards_AnimateMatch(work);
        }
        if (work->cardNo == 2) {
            if (TouchscreenHitbox_FindRectAtTouchNew(ov87_021E8184) == -1) {
                break;
            }
            ScratchOffCardsSprite_SetAnimActive(work->buttonSprite, TRUE);
            ScratchOffCards_PrintStopPrompt(work);
            PlaySE(SEQ_SE_DP_BUTTON9);
            ScratchOffCardsSprite_SetAnim(work->buttonSprite, 0);
            ScratchOffCards_CreateYesNoPrompt(work);
            ScratchOffCards_ShowYesNoPrompt(work);
            work->subState = 10;
        } else {
            if (TouchscreenHitbox_FindRectAtTouchNew(ov87_021E818C) == -1) {
                break;
            }
            ScratchOffCardsSprite_SetAnimActive(work->buttonSprite, TRUE);
            ScratchOffCards_PrintStopPrompt(work);
            PlaySE(SEQ_SE_DP_BUTTON9);
            ScratchOffCardsSprite_SetAnim(work->buttonSprite, 1);
            ScratchOffCards_CreateYesNoPrompt(work);
            ScratchOffCards_ShowYesNoPrompt(work);
            work->subState = 9;
        }
        break;
    case 4:
        if (work->delay == 0) {
            if (!ScratchOffCards_UpdateWildcardMosaic(work, FALSE)) {
                work->subState = 5;
            }
        } else {
            work->delay--;
        }
        break;
    case 5:
        if (!ScratchOffCards_UpdateWildcardMosaic(work, TRUE)) {
            work->delay = 0;
            work->subState = 6;
        }
        break;
    case 6:
        if (ScratchOffCardsSprite_IsAnimated(work->winSprite) == TRUE) {
            break;
        }
        if (work->delay == 0) {
            for (i = 0; i < 3; i++) {
                ScratchOffCardsSprite_SetVisible(work->resultSprites[i], FALSE);
            }
            ScratchOffCardsSprite_SetVisible(work->winSprite, FALSE);
            ov87_021E7998(work);
            ScratchOffCards_HideStartCardMessage(work);
            return TRUE;
        }
        work->delay--;
        break;
    case 7:
        if (work->delay == 0) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
            ov87_021E7998(work);
            ScratchOffCards_HideStartCardMessage(work);
            work->delay = 30;
            work->subState = 8;
        } else {
            work->delay--;
        }
        break;
    case 8:
        if (work->delay == 0) {
            return TRUE;
        }
        work->delay--;
        break;
    case 9:
        hit = (int)YesNoPrompt_HandleInputForSave(work->yesNoPrompt);
        if (hit == YESNORESPONSE_YES) {
            ScratchOffCardsSprite_SetAnimActive(work->buttonSprite, FALSE);
            EraseFrameAndWindow1(&work->windows[0], FALSE);
            ClearWindowTilemapAndCopyToVram(&work->windows[0]);
            ov87_021E7998(work);
            ScratchOffCards_DestroyYesNoPrompt(work);
            return TRUE;
        }
        if (hit == YESNORESPONSE_NO) {
            ScratchOffCardsSprite_SetAnimActive(work->buttonSprite, FALSE);
            EraseFrameAndWindow1(&work->windows[0], FALSE);
            ClearWindowTilemapAndCopyToVram(&work->windows[0]);
            ScratchOffCards_DestroyYesNoPrompt(work);
            work->subState = 11;
        }
        break;
    case 10:
        hit = (int)YesNoPrompt_HandleInputForSave(work->yesNoPrompt);
        if (hit == YESNORESPONSE_YES) {
            ScratchOffCardsSprite_SetAnimActive(work->buttonSprite, FALSE);
            EraseFrameAndWindow1(&work->windows[0], FALSE);
            ClearWindowTilemapAndCopyToVram(&work->windows[0]);
            ov87_021E7990(work);
            ScratchOffCards_DestroyYesNoPrompt(work);
            return TRUE;
        }
        if (hit == YESNORESPONSE_NO) {
            ScratchOffCardsSprite_SetAnimActive(work->buttonSprite, FALSE);
            EraseFrameAndWindow1(&work->windows[0], FALSE);
            ClearWindowTilemapAndCopyToVram(&work->windows[0]);
            ScratchOffCards_DestroyYesNoPrompt(work);
            work->subState = 11;
        }
        break;
    case 11:
        if (!System_GetTouchHeld()) {
            work->subState = 3;
        }
        break;
    }
    return FALSE;
}
#pragma force_active reset

int ScratchOffCards_SlideOutCard(ScratchOffCardsWork *work) {
    switch (work->subState) {
    case 0:
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
        ScratchOffCardsSprite_SetVisible(work->buttonSprite, FALSE);
        work->slideX = 0;
        work->bgScrollX = 0;
        PlaySE(SEQ_SE_PL_SYU03);
        work->subState = 1;
        break;
    case 1:
        BgSetPosTextAndCommit(work->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_X, work->bgScrollX);
        BgSetPosTextAndCommit(work->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, work->bgScrollX);
        BgSetPosTextAndCommit(work->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, work->bgScrollX);
        work->bgScrollX -= 16;
        if (work->slideX >= 256) {
            work->subState = 2;
        } else {
            int i;
            int y;
            i = 0;
            y = 26;
            for (; i < 4; y += 42, i++) {
                ScratchOffCardsSprite_SetPosition(work->cardIconSprites[i], work->slideX + 60, y);
            }
            for (i = 0; i < 3; i++) {
                ScratchOffCardsSprite_SetVisible(work->highlightSprites[i], FALSE);
            }
            for (i = 0; i < 9; i++) {
                ScratchOffCardsSprite_SetPosition(work->cellSprites[i], work->slideX + ov87_021E82E4[i].x, ov87_021E82E4[i].y);
            }
        }
        work->slideX += 16;
        break;
    case 2:
        work->slideX = 0;
        work->bgScrollX = 0;
        return TRUE;
    }
    return FALSE;
}

int ScratchOffCards_FadeOut(ScratchOffCardsWork *work) {
    switch (work->subState) {
    case 0:
        work->timer = 10;
        work->subState++;
        break;
    case 1:
        work->timer--;
        if (work->timer == 0) {
            ClearFrameAndWindow2(&work->windows[0], FALSE);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_122);
            work->subState++;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void ScratchOffCards_CreateHighlightSprites(ScratchOffCardsWork *work) {
    int i;
    const ScratchOffCardsPos *pos = ov87_021E82E4;
    ScratchOffCardsSpriteGfx *gfx;
    i = 0;
    gfx = &work->spriteGfx;
    do {
        work->highlightSprites[i] = ScratchOffCardsSprite_New(gfx, 3, 2, pos->x, pos->y, FALSE, 2, 11);
        ScratchOffCardsSprite_SetVisible(work->highlightSprites[i], FALSE);
        pos++;
        i++;
    } while (i < 3);
}

void ScratchOffCards_CreateResultSprites(ScratchOffCardsWork *work) {
    int i;
    const ScratchOffCardsSpritePos *pos = ov87_021E8194;
    ScratchOffCardsSpriteGfx *gfx;
    i = 0;
    gfx = &work->spriteGfx;
    do {
        work->resultSprites[i] = ScratchOffCardsSprite_New(gfx, 1, 0, pos->x, pos->y, FALSE, 0, 0);
        ScratchOffCardsSprite_SetVisible(work->resultSprites[i], FALSE);
        pos++;
        i++;
    } while (i < 3);
}

void ScratchOffCards_CreateButtonSprites(ScratchOffCardsWork *work) {
    work->buttonSprite = ScratchOffCardsSprite_New(&work->spriteGfx, 3, 1, 0x44, 0xA0, FALSE, 1, 0);
    ScratchOffCardsSprite_SetVisible(work->buttonSprite, FALSE);
    work->unk_340 = ScratchOffCardsSprite_New(&work->spriteGfx, 3, 3, 0x44, 0x44, FALSE, 1, 0);
    ScratchOffCardsSprite_SetVisible(work->unk_340, FALSE);
}

void ScratchOffCards_DeleteCardChoiceSprites(ScratchOffCardsWork *work) {
    int i;
    for (i = 0; i < 4; i++) {
        ScratchOffCardsSprite *sprite = work->cardChoiceSprites[i];
        if (sprite) {
            ScratchOffCardsSprite_Delete(sprite);
        }
    }
}

#pragma force_active on
void ScratchOffCards_FreeResources(ScratchOffCardsWork *work) {
    int i;
    for (i = 0; i < 4; i++) {
        ScratchOffCardsSprite *sprite = work->cardIconSprites[i];
        if (sprite) {
            ScratchOffCardsSprite_Delete(sprite);
        }
    }
    for (i = 0; i < 9; i++) {
        ScratchOffCardsSprite *sprite = work->cellSprites[i];
        if (sprite) {
            ScratchOffCardsSprite_Delete(sprite);
        }
    }
    if (work->buttonSprite) {
        ScratchOffCardsSprite_Delete(work->buttonSprite);
    }
    if (work->unk_340) {
        ScratchOffCardsSprite_Delete(work->unk_340);
    }
    for (i = 0; i < 3; i++) {
        ScratchOffCardsSprite *sprite = work->highlightSprites[i];
        if (sprite) {
            ScratchOffCardsSprite_Delete(sprite);
        }
    }
    for (i = 0; i < 3; i++) {
        ScratchOffCardsSprite *sprite = work->resultSprites[i];
        if (sprite) {
            ScratchOffCardsSprite_Delete(sprite);
        }
    }
    if (work->winSprite) {
        ScratchOffCardsSprite_Delete(work->winSprite);
    }
    FontID_Release(2);
    PaletteData_FreeBuffers(work->paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(work->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_Free(work->paletteData);
    work->paletteData = NULL;
    ScratchOffCardsSpriteGfx_Free(&work->spriteGfx);
    PaletteBlink_Delete(work->paletteFadeTask);
    DestroyMsgData(work->msgData);
    MessageFormat_Delete(work->msgFormat);
    String_Delete(work->string);
    String_Delete(work->stringBuffer);
    MessagePrinter_Delete(work->messagePrinter);
    Heap_Free(work->scratchLayerFile);
    Heap_Free(work->scratchPixels);
    ScratchOffCards_RemoveWindows(work->windows);
    ScratchOffCards_FreeBgConfig(work->bgConfig);
    NARC_Delete(work->narc);
}
#pragma force_active reset

void ScratchOffCards_ResetDisplay(void) {
    volatile u32 *dispcnt = &reg_GX_DISPCNT;
    volatile u32 *dispcntSub = &reg_GXS_DB_DISPCNT;
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    *dispcnt &= 0xFFFFE0FF;
    *dispcntSub &= 0xFFFFE0FF;
}

#pragma force_active on
void ScratchOffCards_InitResources(ScratchOffCardsWork *work) {
    int i;
    work->narc = NARC_New(NARC_a_2_0_5, HEAP_ID_122);
    ScratchOffCards_InitBgGraphics(work);
    ScratchOffCards_InitSpriteGraphics(work);
    work->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x1B0, HEAP_ID_122);
    work->msgFormat = MessageFormat_New(HEAP_ID_122);
    work->string = String_New(600, HEAP_ID_122);
    work->stringBuffer = String_New(600, HEAP_ID_122);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0x1A0, HEAP_ID_122);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0x1A0, HEAP_ID_122);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0x180, HEAP_ID_122);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0x180, HEAP_ID_122);
    FontID_Alloc(2, HEAP_ID_122);
    work->messagePrinter = MessagePrinter_New(15, 14, 0, HEAP_ID_122);
    ScratchOffCards_AddWindows(work->bgConfig, work->windows);
    work->paletteFadeTask = PaletteBlink_New(NULL, 1, 12, HEAP_ID_122);
    ScratchOffCards_LoadScratchLayer(work);
    ScratchOffCards_AllocScratchPixels(work);
    ScratchOffCards_ResetScratchPixels(work);
    GfGfx_BothDispOn();
    for (i = 0; i < 4; i++) {
        work->cardIconSprites[i] = ScratchOffCardsSprite_New(&work->spriteGfx, 0, i, 0x3C, 0x1A + i * 0x2A, TRUE, 0, 0);
        ScratchOffCardsSprite_SetVisible(work->cardIconSprites[i], FALSE);
    }
    for (i = 0; i < 9; i++) {
        work->cellSprites[i] = ScratchOffCardsSprite_New(&work->spriteGfx, 1, 0, ov87_021E82E4[i].x, ov87_021E82E4[i].y, FALSE, 2, 10);
        ScratchOffCardsSprite_SetVisible(work->cellSprites[i], FALSE);
    }
    ScratchOffCards_CreateButtonSprites(work);
    for (i = 0; i < 4; i++) {
        work->cardChoiceSprites[i] = ScratchOffCardsSprite_New(&work->spriteGfx, 2, i, ov87_021E81A0[i].x, ov87_021E81A0[i].y, FALSE, 1, 0);
        ScratchOffCardsSprite_SetPosition(work->cardChoiceSprites[i], ov87_021E81A0[i].x - 256, ov87_021E81A0[i].y);
        ScratchOffCardsSprite_SetDrawPriority(work->cardChoiceSprites[i], i + 20);
    }
    work->winSprite = ScratchOffCardsSprite_New(&work->spriteGfx, 4, 0, 0x26, 0x38, FALSE, 0, 10);
    ScratchOffCardsSprite_SetVisible(work->winSprite, FALSE);
    reg_GX_POWCNT &= ~0x8000;
    sub_020210BC();
    sub_02021148(1);
    Main_SetVBlankIntrCB((GFIntrCB)ScratchOffCards_VBlank, work);
}
#pragma force_active reset

void ScratchOffCards_CreateYesNoPrompt(ScratchOffCardsWork *work) {
    work->yesNoPrompt = YesNoPrompt_Create(HEAP_ID_122);
}

void ScratchOffCards_ShowYesNoPrompt(ScratchOffCardsWork *work) {
    YesNoPromptTemplate template;
    template.bgConfig = work->bgConfig;
    template.bgId = GF_BG_LYR_MAIN_2;
    template.tileStart = 0x380;
    template.plttSlot = 9;
    template.x = 24;
    template.y = 8;
    template.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(work->yesNoPrompt, &template);
}

void ScratchOffCards_DestroyYesNoPrompt(ScratchOffCardsWork *work) {
    YesNoPrompt_Destroy(work->yesNoPrompt);
}

void ScratchOffCards_InitBgGraphics(ScratchOffCardsWork *work) {
    ScratchOffCards_SetVramBanks();
    ScratchOffCards_InitBgs(work->bgConfig);
    work->paletteData = PaletteData_Init(HEAP_ID_122);
    PaletteData_AllocBuffers(work->paletteData, PLTTBUF_MAIN_OBJ, 0x200, HEAP_ID_122);
    PaletteData_AllocBuffers(work->paletteData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_122);
    ov87_021E6DBC(work, GF_BG_LYR_SUB_3);
    ov87_021E6E44();
    ov87_021E6E00(work, GF_BG_LYR_SUB_2);
    ov87_021E6EB0(work, GF_BG_LYR_MAIN_1);
    ov87_021E6E78();
    ov87_021E6EF4(work, GF_BG_LYR_MAIN_0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
}

void ScratchOffCards_InitSpriteGraphics(ScratchOffCardsWork *work) {
    ScratchOffCardsSpriteGfx_Init(&work->spriteGfx);
}

void ScratchOffCards_FreeBgConfig(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_ALL, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_ALL, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    Heap_Free(bgConfig);
}

void ScratchOffCards_VBlank(ScratchOffCardsWork *work) {
    vu32 *dtcm;
    if (work->unk_004 == 0) {
        PaletteData *paletteData = work->paletteData;
        if (paletteData != NULL) {
            PaletteData_PushTransparentBuffers(paletteData);
        }
        DoScheduledBgGpuUpdates(work->bgConfig);
        GF_RunVramTransferTasks();
        OamManager_ApplyAndResetBuffers();
        // OS_SetIrqCheckFlag(OS_IE_V_BLANK), with the DTCM base as a literal
        dtcm = (vu32 *)0x027E0000;
        dtcm[0x3FF8 / 4] |= OS_IE_V_BLANK;
    }
}

void ScratchOffCards_SetVramBanks(void) {
    GraphicsBanks banks = ov87_021E8330;
    GfGfx_SetBanks(&banks);
}

#pragma force_active on
void ScratchOffCards_InitBgs(BgConfig *bgConfig) {
    GraphicsModes modes = ov87_021E81B0;
    BgTemplate sub0;
    BgTemplate sub3;
    BgTemplate sub2;
    BgTemplate main1;
    BgTemplate main0;
    BgTemplate main2;
    SetBothScreensModesAndDisable(&modes);
    sub0 = ov87_021E8228;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sub0, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 32, 0, HEAP_ID_122);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    sub3 = ov87_021E8260;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &sub3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
    sub2 = ov87_021E81D4;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &sub2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    main1 = ov87_021E81F0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &main1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    main0 = ov87_021E820C;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &main0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    main2 = ov87_021E8244;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &main2, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 32, 0, HEAP_ID_122);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}
#pragma force_active reset

void ov87_021E6DBC(ScratchOffCardsWork *work, GFBgLayer bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x23, work->bgConfig, bgId, 0, 0, FALSE, HEAP_ID_122);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 0x25, work->bgConfig, bgId, 0, 0, FALSE, HEAP_ID_122);
}

void ov87_021E6E00(ScratchOffCardsWork *work, GFBgLayer bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x26, work->bgConfig, bgId, 0, 0, FALSE, HEAP_ID_122);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 0x28, work->bgConfig, bgId, 0, 0, FALSE, HEAP_ID_122);
}

void ov87_021E6E44(void) {
    NNSG2dPaletteData *plttData;
    void *file = GfGfxLoader_GetPlttData(NARC_a_2_0_5, 0x22, &plttData, HEAP_ID_122);
    DC_FlushRange(plttData->pRawData, 0xA0);
    GXS_LoadBGPltt(plttData->pRawData, 0, 0xA0);
    Heap_Free(file);
}

void ov87_021E6E78(void) {
    NNSG2dPaletteData *plttData;
    void *file = GfGfxLoader_GetPlttData(NARC_a_2_0_5, 0, &plttData, HEAP_ID_122);
    DC_FlushRange(plttData->pRawData, 0x180);
    GX_LoadBGPltt(plttData->pRawData, 0, 0x180);
    Heap_Free(file);
}

void ov87_021E6EB0(ScratchOffCardsWork *work, GFBgLayer bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 4, work->bgConfig, bgId, 0, 0, FALSE, HEAP_ID_122);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 6, work->bgConfig, bgId, 0, 0, FALSE, HEAP_ID_122);
}

void ov87_021E6EF4(ScratchOffCardsWork *work, GFBgLayer bgId) {
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 7, work->bgConfig, bgId, 0, 0, FALSE, HEAP_ID_122);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 9, work->bgConfig, bgId, 0, 0, FALSE, HEAP_ID_122);
}

#pragma force_active on
int ScratchOffCards_PrintMessage(ScratchOffCardsWork *work, Window *window, int msgNo, u32 x, u32 y, u32 textSpeed, u8 fg, u8 shadow, u8 bg, u8 fontId) {
    FillWindowPixelBuffer(window, bg);
    ReadMsgDataIntoString(work->msgData, msgNo, work->stringBuffer);
    StringExpandPlaceholders(work->msgFormat, work->string, work->stringBuffer);
    return AddTextPrinterParameterizedWithColor(window, fontId, work->string, x, y, textSpeed, MAKE_TEXT_COLOR(fg, shadow, bg), NULL);
}
#pragma force_active reset

#pragma force_active on
int ScratchOffCards_PrintMessageCentered(ScratchOffCardsWork *work, Window *window, int msgNo, u32 x, u32 y, u32 textSpeed, u8 fg, u8 shadow, u8 bg, u8 fontId) {
    FillWindowPixelBuffer(window, bg);
    ReadMsgDataIntoString(work->msgData, msgNo, work->stringBuffer);
    StringExpandPlaceholders(work->msgFormat, work->string, work->stringBuffer);
    x -= (FontID_String_GetWidth(fontId, work->string, 0) + 1) / 2;
    return AddTextPrinterParameterizedWithColor(window, fontId, work->string, x, y, textSpeed, MAKE_TEXT_COLOR(fg, shadow, bg), NULL);
}
#pragma force_active reset

int ScratchOffCards_PrintStopPrompt(ScratchOffCardsWork *work) {
    int ret;
    ScratchOffCards_DrawMessageFrame(work->bgConfig, &work->windows[0]);
    ret = ScratchOffCards_PrintMessage(work, &work->windows[0], 6, 1, 1, TEXT_SPEED_INSTANT, 1, 2, 15, 0);
    ScheduleWindowCopyToVram(&work->windows[0]);
    return ret;
}

int ScratchOffCards_PrintSelectCardMessage(ScratchOffCardsWork *work) {
    int ret;
    ScratchOffCards_BufferNumber(work, 0, work->cardNo + 1);
    ret = ScratchOffCards_PrintMessage(work, &work->windows[15], 12, 1, 1, TEXT_SPEED_INSTANT, 1, 2, 15, 1);
    ScheduleWindowCopyToVram(&work->windows[15]);
    return ret;
}

int ScratchOffCards_PrintStartCardMessage(ScratchOffCardsWork *work) {
    int ret;
    ScratchOffCards_BufferNumber(work, 0, work->cardNo + 1);
    ret = ScratchOffCards_PrintMessageCentered(work, &work->windows[10], 9, 0x30, 5, TEXT_SPEED_INSTANT, 1, 2, 0, 0);
    ScheduleWindowCopyToVram(&work->windows[10]);
    return ret;
}

#pragma force_active on
int ScratchOffCards_ShowResult(ScratchOffCardsWork *work) {
    u8 i;
    for (i = 0; i < 3; i++) {
        ScratchOffCardsSprite_SetAnim(work->resultSprites[i], work->cells[work->revealed[i]]);
        ScratchOffCardsSprite_SetVisible(work->resultSprites[i], TRUE);
    }
    ScratchOffCardsSprite_SetVisible(work->winSprite, TRUE);
    ScratchOffCardsSprite_SetAnimActive(work->winSprite, TRUE);
    PlaySE(SEQ_SE_PL_CALL);
    return 0;
}
#pragma force_active reset

int ScratchOffCards_PrintTooBadMessage(ScratchOffCardsWork *work) {
    int ret;
    PlaySE(SEQ_SE_DP_BOX03);
    ret = ScratchOffCards_PrintMessageCentered(work, &work->windows[12], 11, 0x30, 5, TEXT_SPEED_INSTANT, 1, 2, 0, 0);
    ScheduleWindowCopyToVram(&work->windows[12]);
    return ret;
}

int ScratchOffCards_PrintNextCardButton(ScratchOffCardsWork *work) {
    int ret;
    ret = ScratchOffCards_PrintMessageCentered(work, &work->windows[13], 4, 0x30, 1, TEXT_SPEED_INSTANT, 1, 1, 0, 2);
    ScheduleWindowCopyToVram(&work->windows[13]);
    return ret;
}

int ScratchOffCards_PrintCancelButton(ScratchOffCardsWork *work) {
    int ret;
    ret = ScratchOffCards_PrintMessageCentered(work, &work->windows[14], 5, 0x18, 1, TEXT_SPEED_INSTANT, 1, 1, 0, 2);
    ScheduleWindowCopyToVram(&work->windows[14]);
    return ret;
}

#pragma force_active on
int ScratchOffCards_PrintPrizeName(ScratchOffCardsWork *work, int prizeNo) {
    int ret;
    BufferItemName(work->msgFormat, 0, work->prizes[prizeNo]);
    ret = ScratchOffCards_PrintMessageCentered(work, &work->windows[prizeNo + 2], 1, 44, ov87_021E8388[prizeNo], TEXT_SPEED_INSTANT, 1, 2, 0, 0);
    ScheduleWindowCopyToVram(&work->windows[prizeNo + 2]);
    return ret;
}
#pragma force_active reset

void ScratchOffCards_BufferNumber(ScratchOffCardsWork *work, int fieldNo, int number) {
    BufferIntegerAsString(work->msgFormat, fieldNo, number, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
}

void ScratchOffCards_SetState(ScratchOffCardsWork *work, int *state, u32 newState) {
    work->subState = 0;
    *state = newState;
}

void ScratchOffCards_PlaceWildcards(ScratchOffCardsWork *work) {
    int i = 0;
    u8 cellNo;
    do {
        do {
            cellNo = (u8)((s32)LCRandom() % 9);
        } while (work->cells[cellNo] != SCRATCH_CELL_EMPTY);
        work->cells[cellNo] = SCRATCH_CELL_WILDCARD;
        i++;
    } while (i < 2);
}

#pragma force_active on
void ScratchOffCards_FillCells(ScratchOffCardsWork *work) {
    int failures, i, j, k;
    u8 symbol = LCRandom() % 4;
    failures = 0;
    for (i = 0; i < 9; i++) {
retry:
        j = LCRandom() % 9;
        if (work->cells[j] == SCRATCH_CELL_EMPTY) {
            failures = 0;
            work->cells[j] = symbol;
            if (i == 2 || i == 4 || i == 6) {
                symbol++;
                if (symbol == 4) {
                    symbol = 0;
                }
            }
        } else {
            failures++;
            if (failures < 30) {
                goto retry;
            }
            failures = 0;
            for (k = 0; k < 9; k++) {
                if (work->cells[k] == SCRATCH_CELL_EMPTY) {
                    work->cells[k] = symbol;
                    if (i == 2 || i == 4 || i == 6) {
                        symbol++;
                        if (symbol == 4) {
                            symbol = 0;
                        }
                    }
                    break;
                }
            }
        }
    }
}
#pragma force_active reset

void ScratchOffCards_ShuffleCells(ScratchOffCardsWork *work, int unused) {
    ScratchOffCards_PlaceWildcards(work);
    ScratchOffCards_FillCells(work);
}

void ScratchOffCards_ClearCells(ScratchOffCardsWork *work) {
    int i = 0;
    do {
        work->cells[i] = SCRATCH_CELL_EMPTY;
        i++;
    } while (i < 9);
}

#pragma force_active on
void ScratchOffCards_ChoosePrizes(ScratchOffCardsWork *work, int unused) {
    u16 choices[4];
    struct {
        int i;
        int j;
        u16 wildcardSlot;
        u16 *current;
    } loop;
    for (loop.i = 0; loop.i < 4; loop.i++) {
        choices[loop.i] = 255;
    }
    loop.wildcardSlot = (int)LCRandom() % 4;
    loop.current = choices;
    for (loop.i = 0; loop.i < 4; loop.i++, loop.current++) {
        if (loop.i == loop.wildcardSlot) {
            work->prizes[loop.i] = 92;
        } else {
            do {
                *loop.current = (u32)LCRandom() % 23;
                for (loop.j = 0; loop.j < loop.i; loop.j++) {
                    u16 previous = choices[loop.j];
                    if (previous == *loop.current) {
                        break;
                    }
                }
            } while (loop.j != loop.i);
            work->prizes[loop.i] = ov87_021E8358[*loop.current];
        }
    }
}
#pragma force_active reset

#pragma force_active on
int ScratchOffCards_BounceCardChoice(ScratchOffCardsWork *work, int cardNo) {
    int x, y, newX, newY;
    int step;
    s8 dy;
    if (work->flags.bounceStep >= 8) {
        return TRUE;
    }
    ScratchOffCardsSprite_GetPosition(work->cardChoiceSprites[cardNo], &x, &y);
    step = work->flags.bounceStep;
    dy = ov87_021E838C[step];
    newX = x + ov87_021E827C[cardNo][step];
    newY = y + dy;
    work->flags.bounceStep = step + 1;
    ScratchOffCardsSprite_SetPosition(work->cardChoiceSprites[cardNo], newX, newY);
    return FALSE;
}
#pragma force_active reset

void ScratchOffCards_SetBgPalette(ScratchOffCardsWork *work, int bgId, u8 palette, u8 width, u8 height) {
    BgTilemapRectChangePalette(work->bgConfig, bgId, 0, 0, width, height, palette);
    ScheduleBgTilemapBufferTransfer(work->bgConfig, bgId);
}

u8 ScratchOffCards_CountTouchedCells(ScratchOffCardsWork *work) {
    u8 i, count;
    count = 0;
    i = count;
    do {
        if (work->touchedCells[i] == TRUE) {
            count++;
        }
        i++;
    } while (i < 9);
    return count;
}

void ScratchOffCards_ClearTouchedCells(ScratchOffCardsWork *work) {
    u8 i = 0;
    do {
        work->touchedCells[i] = FALSE;
        i++;
    } while (i < 9);
}

void ScratchOffCards_ClearRevealedCells(ScratchOffCardsWork *work) {
    u8 i = 0;
    do {
        work->cellRevealed[i] = FALSE;
        i++;
    } while (i < 9);
}

#pragma force_active on
void ScratchOffCards_ScratchCell(ScratchOffCardsWork *work, int cellNo) {
    int i = 0;
    if (i < work->touch.count) {
        ScratchOffCardsWork *cursor = work; // advanced by one touch sample per iteration
        const ScratchOffCardsBounds *bounds = &ov87_021E829C[cellNo];
        int left = bounds->left;
        do {
            int x = cursor->touch.samples[0].x;
            int y = cursor->touch.samples[0].y;
            if (left <= x && x <= bounds->right && bounds->top <= y && y <= bounds->bottom) {
                ScratchOffCards_ScratchAt(work, x, y);
            }
            cursor = (ScratchOffCardsWork *)((u8 *)cursor + sizeof(TPData));
            i++;
        } while (i < work->touch.count);
    }
}
#pragma force_active reset

void ScratchOffCards_MarkTouchedArea(ScratchOffCardsWork *work) {
    int i;
    i = 0;
    if (i < work->touch.count) {
        do {
            ScratchOffCards_MarkArea(work, (u8)work->touch.samples[i].x, (u8)work->touch.samples[i].y);
            i++;
        } while (i < work->touch.count);
    }
}

#pragma force_active on
void ScratchOffCards_MarkArea(ScratchOffCardsWork *work, int x, int y) {
    int dy, dx;
    int py, px;
    for (dy = -3; dy < 3; dy++) {
        for (dx = -3; dx < 3; dx++) {
            px = x + dx;
            py = y + dy;
            if (px > 0 && px < 240 && py > 0 && py < 160) {
                work->scratchMask[py][x + dx] = 1;
            }
        }
    }
}
#pragma force_active reset

#pragma force_active on
int ScratchOffCards_CheckCellRevealed(ScratchOffCardsWork *work, u8 cellNo) {
    int x;
    int y;
    int count;
    int left;
    int top;
    count = 0;
    left = ov87_021E82C0[cellNo * 2];
    top = ov87_021E82C2[cellNo * 2];
    for (y = top; y < top + 20; y++) {
        for (x = left; x < left + 25; x++) {
            if (work->scratchMask[0][y * 240 + x] == 1) {
                count++;
            }
        }
    }
    if (count >= 380) {
        if (work->cellRevealed[cellNo] == FALSE) {
            PlaySE(SEQ_SE_DP_PIRORIRO);
            work->revealed[work->revealedCount] = cellNo;
            work->revealedCount++;
        }
        work->cellRevealed[cellNo] = TRUE;
        return TRUE;
    }
    return FALSE;
}
#pragma force_active reset

#pragma force_active on
void ScratchOffCards_AnimateMatch(ScratchOffCardsWork *work) {
    int i;
    work->matchTimer++;
    if (work->matchTimer >= 1) {
        work->matchTimer = 0;
        work->matchFrame++;
        if (work->matchFrame >= 8) {
            work->matchFrame = 0;
        }
        if (work->revealedCount == 2) {
            for (i = 0; i < 2; i++) {
                ScratchOffCardsSprite_SetPalette(work->highlightSprites[i], 3);
                ScratchOffCardsSprite_SetVisible(work->highlightSprites[i], TRUE);
                ScratchOffCardsSprite_SetPosition(work->highlightSprites[i], ov87_021E82E4[work->revealed[i]].x - 36, ov87_021E82E4[work->revealed[i]].y - 10);
            }
        }
        ScratchOffCardsSpriteGfx_ReplaceMatchPalette(&work->spriteGfx, work->matchFrame);
    }
}
#pragma force_active reset

int ScratchOffCards_CheckMatch(ScratchOffCardsWork *work) {
    int match = FALSE;
    u8 a, b;
    if (work->revealedCount <= 1) {
        return FALSE;
    }
    a = work->cells[work->revealed[1]];
    b = work->cells[work->revealed[0]];
    if (b == a) {
        work->flags.hasPair = TRUE;
    } else if (b == SCRATCH_CELL_WILDCARD) {
        work->flags.hasPair = TRUE;
    } else if (a == SCRATCH_CELL_WILDCARD) {
        work->flags.hasPair = TRUE;
    }
    if (work->revealedCount <= 2) {
        return FALSE;
    }
    b = work->cells[work->revealed[1]];
    a = work->cells[work->revealed[0]];
    if (a == b && a == work->cells[work->revealed[2]]) {
        match = TRUE;
    }
    if (a == SCRATCH_CELL_WILDCARD && b == SCRATCH_CELL_WILDCARD) {
        match = TRUE;
    }
    if (a == SCRATCH_CELL_WILDCARD && work->cells[work->revealed[2]] == SCRATCH_CELL_WILDCARD) {
        match = TRUE;
    }
    if (b == SCRATCH_CELL_WILDCARD && work->cells[work->revealed[2]] == SCRATCH_CELL_WILDCARD) {
        match = TRUE;
    }
    if (a == SCRATCH_CELL_WILDCARD && b == work->cells[work->revealed[2]]) {
        match = TRUE;
    }
    if (b == SCRATCH_CELL_WILDCARD && a == work->cells[work->revealed[2]]) {
        match = TRUE;
    }
    if (work->cells[work->revealed[2]] == SCRATCH_CELL_WILDCARD && a == b) {
        match = TRUE;
    }
    if (match == TRUE) {
        int i;
        for (i = 0; i < 3; i++) {
            b = work->cells[work->revealed[i]];
            if (b != SCRATCH_CELL_WILDCARD) {
                work->matchedSymbol = b;
                break;
            }
        }
        return TRUE;
    }
    return FALSE;
}

// The OBJ mosaic size lives in the upper byte of the mosaic register.
#define REG_OBJ_MOSAIC (*(vu8 *)(REG_MOSAIC_ADDR + 1))

u32 ScratchOffCards_StartWildcardMosaic(ScratchOffCardsWork *work) {
    u32 ret;
    int i;
    ret = FALSE;
    i = 0;
    do {
        if (work->cells[work->revealed[i]] == SCRATCH_CELL_WILDCARD) {
            ScratchOffCardsSprite_SetMosaic(work->resultSprites[i], TRUE);
            ret = TRUE;
        }
        i++;
    } while (i < 3);
    work->mosaic = 0;
    REG_OBJ_MOSAIC = work->mosaic | (work->mosaic << 4);
    return ret;
}

int ScratchOffCards_UpdateWildcardMosaic(ScratchOffCardsWork *work, int fadeOut) {
    int i;
    for (i = 0; i < 3; i++) {
        if (work->cells[work->revealed[i]] == SCRATCH_CELL_WILDCARD) {
            break;
        }
    }
    if (i == 3) {
        return FALSE;
    }
    if (fadeOut == FALSE) {
        if (work->mosaic == 0) {
            PlaySE(SEQ_SE_PL_W100);
        }
        if (work->mosaic < 4) {
            work->mosaic++;
        } else {
            i = 0;
            do {
                if (work->cells[work->revealed[i]] == SCRATCH_CELL_WILDCARD) {
                    ScratchOffCardsSprite_SetAnim(work->resultSprites[i], work->matchedSymbol);
                }
                i++;
            } while (i < 3);
            return FALSE;
        }
    } else {
        if (work->mosaic != 0) {
            work->mosaic--;
        } else {
            int off;
            i = 0;
            off = i;
            do {
                ScratchOffCardsSprite_SetMosaic(work->resultSprites[i], off);
                i++;
            } while (i < 3);
            return FALSE;
        }
    }
    REG_OBJ_MOSAIC = work->mosaic | (work->mosaic << 4);
    return TRUE;
}

void ov87_021E7990(ScratchOffCardsWork *work) {
    work->cardNo++;
}

void ov87_021E7998(ScratchOffCardsWork *work) {
    work->cardNo++;
}

void ScratchOffCards_HideStartCardMessage(ScratchOffCardsWork *work) {
    ScratchOffCardsSprite_SetVisible(work->unk_340, FALSE);
    FillWindowPixelBuffer(&work->windows[10], 0);
    ScheduleWindowCopyToVram(&work->windows[10]);
}

void ScratchOffCards_LoadScratchLayer(ScratchOffCardsWork *work) {
    NNSG2dCharacterData *charData;
    work->scratchLayerFile = GfGfxLoader_GetCharData(NARC_a_2_0_5, 7, FALSE, &work->scratchLayer, HEAP_ID_122);
    charData = work->scratchLayer;
    BG_LoadCharTilesData(work->bgConfig, GF_BG_LYR_MAIN_0, charData->pRawData, charData->szByte, 0);
    BgCommitTilemapBufferToVram(work->bgConfig, GF_BG_LYR_MAIN_0);
}

void ScratchOffCards_AllocScratchPixels(ScratchOffCardsWork *work) {
    NNSG2dCharacterData *charData = work->scratchLayer;
    work->scratchPixels = Heap_Alloc(HEAP_ID_122, charData->szByte);
    if (work->scratchPixels == NULL) {
        GF_ASSERT(work->scratchPixels != NULL);
    }
}

void ScratchOffCards_ResetScratchPixels(ScratchOffCardsWork *work) {
    NNSG2dCharacterData *charData = work->scratchLayer;
    memcpy(work->scratchPixels, charData->pRawData, charData->szByte);
}

void ScratchOffCards_ScratchAt(ScratchOffCardsWork *work, int x, int y) {
    int dx;
    int dy;
    for (dx = -3; dx < 3; dx++) {
        for (dy = -3; dy < 3; dy++) {
            if (x + dy > 0 && x + dy < 256 && y + dx > 0 && y + dx < 192) {
                ScratchOffCards_ClearScratchPixel(work, x + dy, y + dx);
            }
        }
    }
    BG_LoadCharTilesData(work->bgConfig, GF_BG_LYR_MAIN_0, work->scratchPixels, work->scratchLayer->szByte, 0);
    BgCommitTilemapBufferToVram(work->bgConfig, GF_BG_LYR_MAIN_0);
}

#pragma force_active on
void ScratchOffCards_ClearScratchPixel(ScratchOffCardsWork *work, u32 x, u32 y) {
    u8 mask;
    u32 tileX, tileY, pixelX;
    mask = 0xFF;
    if ((x & 1) == 0) {
        mask ^= 0x0F;
    } else {
        mask ^= 0xF0;
    }
    tileX = x / 8 * 0x20;
    tileY = y / 8 * 0x400;
    pixelX = (x % 8) / 2;
    if (y < 8) {
        y = y * 4;
    } else {
        y = (y % 8) * 4;
    }
    work->scratchPixels[(u32)(y + (pixelX + (tileY + tileX)))] &= mask;
}
#pragma force_active reset

void ScratchOffCardsSpriteGfx_Init(ScratchOffCardsSpriteGfx *gfx) {
    const u8 *counts;
    int i;
    GF_CreateVramTransferManager(0x20, HEAP_ID_122);
    ScratchOffCardsSpriteGfx_InitTransfer();
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x80, 0, 0x20, 0, 0x80, 0, 0x20, HEAP_ID_122);
    gfx->spriteList = G2dRenderer_Init(0x28, &gfx->renderer, HEAP_ID_122);
    G2dRenderer_SetSubSurfaceCoords(&gfx->renderer, 0, 2 << 20);
    counts = ov87_021E8394;
    for (i = 0; i < 4; i++) {
        gfx->resMan[i] = Create2DGfxResObjMan(*counts, (GfGfxResType)i, HEAP_ID_122);
        counts++;
    }
    ov87_021E7BC4(gfx, 1);
    ov87_021E7C50(gfx, 1, 1, 1);
    ov87_021E7CE0(gfx, 1);
    ov87_021E7D6C(gfx, 1);
    ov87_021E7C50(gfx, 0, 0, 2);
    for (i = 0; i < 5; i++) {
        SpriteTransfer_CreateCharTransferTask(gfx->resObj[i][GF_GFX_RES_TYPE_CHAR]);
        SpriteTransfer_CreateExtPlttTransferTask(gfx->resObj[i][GF_GFX_RES_TYPE_PLTT]);
    }
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void ov87_021E7BC4(ScratchOffCardsSpriteGfx *gfx, u32 unused) {
    gfx->resObj[2][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], NARC_a_2_0_5, 0xC, FALSE, 2, 1, HEAP_ID_122);
    gfx->resObj[2][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], NARC_a_2_0_5, 0xD, FALSE, 2, 1, 4, HEAP_ID_122);
    gfx->resObj[2][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_CELL], NARC_a_2_0_5, 0xB, FALSE, 2, GF_GFX_RES_TYPE_CELL, HEAP_ID_122);
    gfx->resObj[2][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_ANIM], NARC_a_2_0_5, 0xA, FALSE, 2, GF_GFX_RES_TYPE_ANIM, HEAP_ID_122);
}

void ov87_021E7C50(ScratchOffCardsSpriteGfx *gfx, u32 slot, u32 id, u32 vram) {
    gfx->resObj[slot][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], NARC_a_2_0_5, 0x20, FALSE, id, vram, HEAP_ID_122);
    gfx->resObj[slot][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], NARC_a_2_0_5, 0x21, FALSE, id, vram, 5, HEAP_ID_122);
    gfx->resObj[slot][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_CELL], NARC_a_2_0_5, 0x1F, FALSE, id, GF_GFX_RES_TYPE_CELL, HEAP_ID_122);
    gfx->resObj[slot][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_ANIM], NARC_a_2_0_5, 0x1E, FALSE, id, GF_GFX_RES_TYPE_ANIM, HEAP_ID_122);
}

void ov87_021E7CE0(ScratchOffCardsSpriteGfx *gfx, u32 unused) {
    gfx->resObj[3][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], NARC_a_2_0_5, 0x10, FALSE, 3, 1, HEAP_ID_122);
    gfx->resObj[3][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], NARC_a_2_0_5, 0x11, FALSE, 3, 1, 4, HEAP_ID_122);
    gfx->resObj[3][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_CELL], NARC_a_2_0_5, 0xF, FALSE, 3, GF_GFX_RES_TYPE_CELL, HEAP_ID_122);
    gfx->resObj[3][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_ANIM], NARC_a_2_0_5, 0xE, FALSE, 3, GF_GFX_RES_TYPE_ANIM, HEAP_ID_122);
}

void ov87_021E7D6C(ScratchOffCardsSpriteGfx *gfx, u32 unused) {
    gfx->resObj[4][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], NARC_a_2_0_5, 0x14, FALSE, 4, 1, HEAP_ID_122);
    gfx->resObj[4][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], NARC_a_2_0_5, 0x15, FALSE, 4, 1, 1, HEAP_ID_122);
    gfx->resObj[4][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_CELL], NARC_a_2_0_5, 0x13, FALSE, 4, GF_GFX_RES_TYPE_CELL, HEAP_ID_122);
    gfx->resObj[4][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_ANIM], NARC_a_2_0_5, 0x12, FALSE, 4, GF_GFX_RES_TYPE_ANIM, HEAP_ID_122);
}

#pragma force_active on
Sprite *ScratchOffCardsSpriteGfx_CreateSprite(ScratchOffCardsSpriteGfx *gfx, int resId, int animSeq, int priority, int drawPriority, u8 isSubScreen) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    Sprite *sprite;
    CreateSpriteResourcesHeader(&header, resId, resId, resId, resId, -1, -1, FALSE, priority, gfx->resMan[GF_GFX_RES_TYPE_CHAR], gfx->resMan[GF_GFX_RES_TYPE_PLTT], gfx->resMan[GF_GFX_RES_TYPE_CELL], gfx->resMan[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    template.spriteList = gfx->spriteList;
    template.header = &header;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = drawPriority;
    template.heapID = HEAP_ID_122;
    if (isSubScreen == FALSE) {
        template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    } else {
        template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    }
    sprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(sprite, FALSE);
    Sprite_SetAnimSpeed(sprite, FX32_ONE);
    Sprite_SetAnimCtrlSeq(sprite, animSeq);
    return sprite;
}
#pragma force_active reset

void ScratchOffCardsSpriteGfx_Free(ScratchOffCardsSpriteGfx *gfx) {
    u8 i;
    for (i = 0; i < 5; i++) {
        SpriteTransfer_DeleteCharTransferTask(gfx->resObj[i][GF_GFX_RES_TYPE_CHAR]);
        SpriteTransfer_DeletePlttTransferTask(gfx->resObj[i][GF_GFX_RES_TYPE_PLTT]);
    }
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(gfx->resMan[i]);
    }
    SpriteList_Delete(gfx->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}

void ScratchOffCardsSpriteGfx_InitTransfer(void) {
    ObjCharTransferTemplate template = ov87_021E8398;
    ObjCharTransfer_InitEx(&template, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K);
    ObjPlttTransfer_Init(14, HEAP_ID_122);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

#pragma force_active on
void ScratchOffCardsSpriteGfx_ReplaceMatchPalette(ScratchOffCardsSpriteGfx *gfx, int frame) {
    SpriteResource *resource;
    if (frame >= 8) {
        GF_AssertFail();
    }
    resource = SpriteResourceCollection_Find(gfx->resMan[GF_GFX_RES_TYPE_PLTT], 3);
    ReplacePlttResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], resource, NARC_a_2_0_5, ov87_021E84C0[frame], FALSE, HEAP_ID_122);
    SpriteTransfer_ReplacePlttData(resource);
}
#pragma force_active reset

#pragma force_active on
ScratchOffCardsSprite *ScratchOffCardsSprite_New(ScratchOffCardsSpriteGfx *gfx, int resId, int animSeq, u16 x, u16 y, u8 isSubScreen, int priority, int drawPriority) {
    ScratchOffCardsSprite *sprite = Heap_Alloc(HEAP_ID_122, sizeof(ScratchOffCardsSprite));
    memset(sprite, 0, sizeof(ScratchOffCardsSprite));
    sprite->sprite = ScratchOffCardsSpriteGfx_CreateSprite(gfx, resId, animSeq, priority, drawPriority, isSubScreen);
    sprite->isSubScreen = isSubScreen;
    ScratchOffCardsSprite_SetPosition(sprite, x, y);
    return sprite;
}
#pragma force_active reset

int ScratchOffCardsSprite_Delete(ScratchOffCardsSprite *sprite) {
    Sprite_Delete(sprite->sprite);
    Heap_Free(sprite);
    return 0;
}

void ScratchOffCardsSprite_SetVisible(ScratchOffCardsSprite *sprite, int visible) {
    Sprite_SetDrawFlag(sprite->sprite, visible);
}

void ScratchOffCardsSprite_SetDrawPriority(ScratchOffCardsSprite *sprite, int priority) {
    Sprite_SetDrawPriority(sprite->sprite, priority);
}

void ScratchOffCardsSprite_SetPosition(ScratchOffCardsSprite *sprite, int x, int y) {
    VecFx32 pos;
    pos.y = y << FX32_SHIFT;
    pos.x = x << FX32_SHIFT;
    if (sprite->isSubScreen == TRUE) {
        pos.y += 512 << FX32_SHIFT;
    }
    Sprite_SetMatrix(sprite->sprite, &pos);
}

void ScratchOffCardsSprite_GetPosition(ScratchOffCardsSprite *sprite, int *x, int *y) {
    VecFx32 *pos = Sprite_GetMatrixPtr(sprite->sprite);
    *x = (s32)(pos->x + ((u32)(pos->x >> 11) >> 20)) >> FX32_SHIFT;
    *y = (s32)(pos->y + ((u32)(pos->y >> 11) >> 20)) >> FX32_SHIFT;
}

void ScratchOffCardsSprite_SetAnim(ScratchOffCardsSprite *sprite, int seq) {
    Sprite_SetAnimSpeed(sprite->sprite, FX32_ONE);
    Sprite_TryChangeAnimSeq(sprite->sprite, seq);
}

void ScratchOffCardsSprite_ResetScale(ScratchOffCardsSprite *sprite, int unused) {
    Sprite_SetScaleAndAffineType(sprite->sprite, (VecFx32 *)&ov87_021E83A8, 1);
}

void ScratchOffCardsSprite_SetScale(ScratchOffCardsSprite *sprite, VecFx32 *scale) {
    Sprite_SetScaleAndAffineType(sprite->sprite, scale, 2);
}

void ScratchOffCardsSprite_SetPalette(ScratchOffCardsSprite *sprite, int palette) {
    Sprite_SetPalIndexRespectVramOffset(sprite->sprite, palette);
}

void ScratchOffCardsSprite_SetAnimActive(ScratchOffCardsSprite *sprite, int active) {
    int seq;
    Sprite_SetAnimActiveFlag(sprite->sprite, active);
    Sprite_SetAnimSpeed(sprite->sprite, FX32_ONE);
    seq = Sprite_GetAnimationNumber(sprite->sprite);
    Sprite_SetAnimCtrlSeq(sprite->sprite, seq);
}

void ScratchOffCardsSprite_SetMosaic(ScratchOffCardsSprite *sprite, int mosaic) {
    Sprite_SetMosaic(sprite->sprite, mosaic);
}

int ScratchOffCardsSprite_IsAnimated(ScratchOffCardsSprite *sprite) {
    return Sprite_IsAnimated(sprite->sprite);
}

void ScratchOffCards_AddWindows(BgConfig *bgConfig, Window *windows) {
    u8 i = 0;
    do {
        AddWindow(bgConfig, &windows[i], &ov87_021E842C[i]);
        FillWindowPixelBuffer(&windows[i], 0);
        i++;
    } while (i < 16);
}

void ScratchOffCards_RemoveWindows(Window *windows) {
    u16 i = 0;
    do {
        RemoveWindow(&windows[i]);
        i++;
    } while (i < 16);
}

void ScratchOffCards_DrawMessageFrame(BgConfig *bgConfig, Window *window) {
    LoadUserFrameGfx1Tiles(bgConfig, GF_BG_LYR_MAIN_2, 0x3F7, 0, HEAP_ID_122);
    DrawFrameAndWindow1(window, TRUE, 0x3F7, 11);
}

void ScratchOffCards_DrawWindowFrame(Window *window, int frame) {
    int bgId = GetWindowBgId(window);
    LoadUserFrameGfx2(window->bgConfig, (GFBgLayer)bgId, 0x3D9, 10, (u8)frame, HEAP_ID_122);
    FillWindowPixelBuffer(window, 15);
    DrawFrameAndWindow2(window, FALSE, 0x3D9, 10);
}
