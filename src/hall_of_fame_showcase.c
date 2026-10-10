#include "hall_of_fame_showcase.h"

#include "global.h"

#include "constants/moves.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "msgdata/msg.naix"

#include "bg_window.h"
#include "filesystem.h"
#include "follow_mon.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "hall_of_fame.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "palette.h"
#include "pokemon.h"
#include "screen_fade.h"
#include "sprite_system.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_02013FDC.h"
#include "unk_02019BA4.h"
#include "unk_020210A0.h"

#define HOF_SHOWCASE_NUM_WINDOWS 17
#define HOF_SHOWCASE_NUM_SPRITES 18

// Sprites 2-7 and 8-13 are the two alternating sets of party icons, so a page can be drawn
// while the previous one is still shown.
#define HOF_SHOWCASE_SPRITE_PREV_PAGE 15
#define HOF_SHOWCASE_SPRITE_NEXT_PAGE 16
#define HOF_SHOWCASE_SPRITE_EXIT      17

typedef struct HallOfFameShowcase {
    HallOfFame *hallOfFame;                           // 0x000
    BgConfig *bgConfig;                               // 0x004
    Window windows[HOF_SHOWCASE_NUM_WINDOWS];         // 0x008
    MsgData *msgData;                                 // 0x118
    MsgData *speciesNames;                            // 0x11C
    MsgData *moveNames;                               // 0x120
    MessageFormat *msgFormat;                         // 0x124
    String *stringBuf;                                // 0x128
    String *stringBuf2;                               // 0x12C
    SpriteSystem *spriteSystem;                       // 0x130
    SpriteManager *spriteManager;                     // 0x134
    ManagedSprite *sprites[HOF_SHOWCASE_NUM_SPRITES]; // 0x138
    GridInputHandler *gridInput;                      // 0x180
    NARC *pokegraNarc;                                // 0x184
    SHOW_HOFMON mon;                                  // 0x188
    String *nickname;                                 // 0x1A4
    String *otName;                                   // 0x1A8
    u32 numMons;                                      // 0x1AC
    int recordNum;                                    // 0x1B0
    int record;                                       // 0x1B4
    int numRecords;                                   // 0x1B8
    u32 pressedButton;                                // 0x1BC
    u32 buttonAnimTimer;                              // 0x1C0
    u16 monDetailSet;                                 // 0x1C4
    u16 iconSet;                                      // 0x1C6
    int state;                                        // 0x1C8
    int nextState;                                    // 0x1CC
} HallOfFameShowcase;                                 // size: 0x1D0

extern const TouchscreenHitbox _021E6E7C[];
extern const GraphicsModes sShowcaseGraphicsModes;
extern const OamCharTransferParam sShowcaseOamCharTransferParam;
extern const SpriteResourceCountsListUnion sShowcaseSpriteResourceCounts;
extern const OamManagerParam sShowcaseOamManagerParam;
extern const UnkStruct_02014E30 sShowcaseMonPicRect;
extern const GridCallbacks sShowcaseGridCallbacks;
extern const s16 sShowcaseIconPositions[];
extern const s16 sShowcaseIconPositionsY[];
extern const BgTemplate sShowcaseBgTemplate_Main1;
extern const BgTemplate sShowcaseBgTemplate_Sub0;
extern const BgTemplate sShowcaseBgTemplate_Sub1;
extern const BgTemplate sShowcaseBgTemplate_Main0;
extern const TouchscreenHitbox sShowcaseGridHitboxes[];
extern const GraphicsBanks sShowcaseGraphicsBanks;
extern const ManagedSpriteTemplate sShowcasePartyIconSpriteTemplate;
extern const DpadMenuBox sShowcaseGridDpadBoxes[];
extern const WindowTemplate sShowcaseWindowTemplates[];
extern const ManagedSpriteTemplate sShowcaseMonPicSpriteTemplate1;
extern const ManagedSpriteTemplate sShowcaseMonPicSpriteTemplate2;
extern const ManagedSpriteTemplate sShowcaseCursorSpriteTemplate;
extern const ManagedSpriteTemplate sShowcasePrevPageSpriteTemplate;
extern const ManagedSpriteTemplate sShowcaseNextPageSpriteTemplate;
extern const ManagedSpriteTemplate sShowcaseExitSpriteTemplate;

void HallOfFameShowcase_VBlankCB(HallOfFameShowcase *showcase);
void HallOfFameShowcase_LoadMainObjChar(const void *src, u32 offset, u32 size);
void HallOfFameShowcase_LoadSubObjChar(const void *src, u32 offset, u32 size);
void HallOfFameShowcase_LoadMainObjPltt(const void *src, u32 offset, u32 size);
void HallOfFameShowcase_SetGfxBanks(void);
void HallOfFameShowcase_InitBgs(HallOfFameShowcase *showcase);
void HallOfFameShowcase_FreeBgs(HallOfFameShowcase *showcase);
void HallOfFameShowcase_InitSprites(HallOfFameShowcase *showcase);
void HallOfFameShowcase_DeleteSprite(HallOfFameShowcase *showcase, u32 index);
void HallOfFameShowcase_FreeSprites(HallOfFameShowcase *showcase);
void HallOfFameShowcase_TickSprites(HallOfFameShowcase *showcase);
void HallOfFameShowcase_InitStrings(HallOfFameShowcase *showcase);
void HallOfFameShowcase_FreeStrings(HallOfFameShowcase *showcase);
void HallOfFameShowcase_InitWindows(HallOfFameShowcase *showcase);
void HallOfFameShowcase_FreeWindows(HallOfFameShowcase *showcase);
void HallOfFameShowcase_CreateGridInput(HallOfFameShowcase *showcase);
void HallOfFameShowcase_FreeGridInput(HallOfFameShowcase *showcase);
void HallOfFameShowcase_GridOnButton(void);
void HallOfFameShowcase_GridOnSwitchToTouchMode(void);
void HallOfFameShowcase_GridOnTouch(HallOfFameShowcase *showcase, int target);
void HallOfFameShowcase_GridOnKeyMove(HallOfFameShowcase *showcase, int target);
void HallOfFameShowcase_BeginFadeIn(void);
void HallOfFameShowcase_BeginFadeOut(void);
int HallOfFameShowcase_HandleInput(HallOfFameShowcase *showcase);
void HallOfFameShowcase_DeletePartyIcons(HallOfFameShowcase *showcase);
void HallOfFameShowcase_ChangeRecord(HallOfFameShowcase *showcase, int delta);
void HallOfFameShowcase_HideMonDetail(HallOfFameShowcase *showcase);
void HallOfFameShowcase_ShowMonDetail(HallOfFameShowcase *showcase, u32 index);
int HallOfFameShowcase_AnimateButtonPress(HallOfFameShowcase *showcase);
void HallOfFameShowcase_MoveCursor(HallOfFameShowcase *showcase, int target);
void HallOfFameShowcase_CreatePartyIcon(HallOfFameShowcase *showcase, u32 spriteIndex, u32 position, int charResId, u32 plttResId);
int HallOfFameShowcase_GetOverworldModelFileId(int species, int form, int isFemale);

BOOL HallOfFameShowcase_Init(OverlayManager *man, int *state) {
    HallOfFameShowcase *showcase;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_EngineASetPlanes(0);
    GfGfx_EngineBSetPlanes(0);
    *(volatile u16 *)0x04000050 = 0;
    *(volatile u16 *)0x04001050 = 0;
    sub_020210BC();
    sub_02021148(4);
    *(volatile u16 *)0x04000304 &= 0xFFFF7FFF;
    Heap_Create(HEAP_ID_3, HEAP_ID_59, 0x40000);
    showcase = OverlayManager_CreateAndGetData(man, sizeof(HallOfFameShowcase), HEAP_ID_59);
    MI_CpuFill8(showcase, 0, sizeof(HallOfFameShowcase));
    showcase->hallOfFame = OverlayManager_GetArgs(man);
    showcase->numRecords = Save_HOF_GetNumRecords(showcase->hallOfFame);
    showcase->pokegraNarc = NARC_New(NARC_a_1_8_0, HEAP_ID_59);
    HallOfFameShowcase_SetGfxBanks();
    HallOfFameShowcase_InitBgs(showcase);
    HallOfFameShowcase_InitSprites(showcase);
    HallOfFameShowcase_InitStrings(showcase);
    HallOfFameShowcase_InitWindows(showcase);
    HallOfFameShowcase_CreateGridInput(showcase);
    HallOfFameShowcase_ChangeRecord(showcase, 0);
    Main_SetVBlankIntrCB((GFIntrCB)HallOfFameShowcase_VBlankCB, showcase);
    return TRUE;
}

BOOL HallOfFameShowcase_Exit(OverlayManager *man, int *state) {
    HallOfFameShowcase *showcase = OverlayManager_GetData(man);

    Main_SetVBlankIntrCB(NULL, showcase);
    HallOfFameShowcase_FreeGridInput(showcase);
    HallOfFameShowcase_FreeWindows(showcase);
    HallOfFameShowcase_FreeStrings(showcase);
    HallOfFameShowcase_FreeSprites(showcase);
    HallOfFameShowcase_FreeBgs(showcase);
    NARC_Delete(showcase->pokegraNarc);
    sub_02021238();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_59);
    return TRUE;
}

#pragma force_active on

BOOL HallOfFameShowcase_Main(OverlayManager *man, int *state) {
    HallOfFameShowcase *showcase = OverlayManager_GetData(man);

    switch (*state) {
    case 0:
        HallOfFameShowcase_BeginFadeIn();
        (*state)++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            (*state)++;
        }
        break;
    case 2:
        if (HallOfFameShowcase_HandleInput(showcase) == FALSE) {
            (*state)++;
        }
        break;
    case 3:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    HallOfFameShowcase_TickSprites(showcase);
    SpriteSystem_DrawSprites(showcase->spriteManager);
    return FALSE;
}

#pragma force_active reset

void HallOfFameShowcase_VBlankCB(HallOfFameShowcase *showcase) {
    vu32 *dtcm = (vu32 *)0x027E0000;

    DoScheduledBgGpuUpdates(showcase->bgConfig);
    SpriteSystem_TransferOam();
    dtcm[(HW_INTR_CHECK_BUF - HW_DTCM) / 4] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag, through the DTCM base address
}

void HallOfFameShowcase_LoadMainObjChar(const void *src, u32 offset, u32 size) {
    DC_FlushRange(src, size);
    GX_LoadOBJ(src, offset, size);
}

void HallOfFameShowcase_LoadSubObjChar(const void *src, u32 offset, u32 size) {
    DC_FlushRange(src, size);
    GXS_LoadOBJ(src, offset, size);
}

void HallOfFameShowcase_LoadMainObjPltt(const void *src, u32 offset, u32 size) {
    DC_FlushRange(src, size);
    GX_LoadOBJPltt(src, offset, size);
}

void HallOfFameShowcase_SetGfxBanks(void) {
    GfGfx_SetBanks(&sShowcaseGraphicsBanks);
}

void HallOfFameShowcase_InitBgs(HallOfFameShowcase *showcase) {
    GraphicsModes modes;
    BgTemplate bg0;
    BgTemplate bg1;
    BgTemplate bg4;
    BgTemplate bg5;
    NARC *narc;

    showcase->bgConfig = BgConfig_Alloc(HEAP_ID_59);
    modes = sShowcaseGraphicsModes;
    SetBothScreensModesAndDisable(&modes);

    bg0 = sShowcaseBgTemplate_Main0;
    InitBgFromTemplate(showcase->bgConfig, GF_BG_LYR_MAIN_0, &bg0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(showcase->bgConfig, GF_BG_LYR_MAIN_0);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_59);

    bg1 = sShowcaseBgTemplate_Main1;
    InitBgFromTemplate(showcase->bgConfig, GF_BG_LYR_MAIN_1, &bg1, GF_BG_TYPE_TEXT);

    bg4 = sShowcaseBgTemplate_Sub0;
    InitBgFromTemplate(showcase->bgConfig, GF_BG_LYR_SUB_0, &bg4, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(showcase->bgConfig, GF_BG_LYR_SUB_0);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_59);

    bg5 = sShowcaseBgTemplate_Sub1;
    InitBgFromTemplate(showcase->bgConfig, GF_BG_LYR_SUB_1, &bg5, GF_BG_TYPE_TEXT);

    narc = NARC_New(NARC_a_0_9_7, HEAP_ID_59);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0, showcase->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, HEAP_ID_59);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, showcase->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, HEAP_ID_59);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 2, GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0, 0x20, HEAP_ID_59);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 3, showcase->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, HEAP_ID_59);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 4, showcase->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, HEAP_ID_59);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 5, GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0, 0x20, HEAP_ID_59);
    NARC_Delete(narc);

    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0x1E0, HEAP_ID_59);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0x1E0, HEAP_ID_59);
}

void HallOfFameShowcase_FreeBgs(HallOfFameShowcase *showcase) {
    FreeBgTilemapBuffer(showcase->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(showcase->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(showcase->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(showcase->bgConfig, GF_BG_LYR_MAIN_0);
    Heap_Free(showcase->bgConfig);
}

#pragma force_active on

void HallOfFameShowcase_InitSprites(HallOfFameShowcase *showcase) {
    OamManagerParam oamParam;
    OamCharTransferParam charTransferParam;
    SpriteResourceCountsListUnion resourceCounts;
    NARC *narc;
    u32 i;

    showcase->spriteSystem = SpriteSystem_Alloc(HEAP_ID_59);
    showcase->spriteManager = SpriteManager_New(showcase->spriteSystem);
    oamParam = sShowcaseOamManagerParam;
    charTransferParam = sShowcaseOamCharTransferParam;
    SpriteSystem_Init(showcase->spriteSystem, &oamParam, &charTransferParam, 32);
    resourceCounts = sShowcaseSpriteResourceCounts;
    SpriteSystem_InitSprites(showcase->spriteSystem, showcase->spriteManager, HOF_SHOWCASE_NUM_SPRITES);
    SpriteSystem_InitManagerWithCapacities(showcase->spriteSystem, showcase->spriteManager, &resourceCounts);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(showcase->spriteSystem), 0, FX32_CONST(512));

    narc = NARC_New(NARC_a_0_0_8, HEAP_ID_59);
    SpriteSystem_LoadCharResObjFromOpenNarc(showcase->spriteSystem, showcase->spriteManager, narc, 76, FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 0xDCC0);
    SpriteSystem_LoadCharResObjFromOpenNarc(showcase->spriteSystem, showcase->spriteManager, narc, 76, FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 0xDCC1);
    SpriteSystem_LoadPlttResObjFromOpenNarc(showcase->spriteSystem, showcase->spriteManager, narc, 75, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 0xDCC0);
    SpriteSystem_LoadPlttResObjFromOpenNarc(showcase->spriteSystem, showcase->spriteManager, narc, 75, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 0xDCC1);
    SpriteSystem_LoadCellResObjFromOpenNarc(showcase->spriteSystem, showcase->spriteManager, narc, 77, FALSE, 0xDCC0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(showcase->spriteSystem, showcase->spriteManager, narc, 78, FALSE, 0xDCC0);
    NARC_Delete(narc);

    narc = NARC_New(NARC_a_0_9_7, HEAP_ID_59);
    SpriteSystem_LoadCharResObjFromOpenNarc(showcase->spriteSystem, showcase->spriteManager, narc, 6, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 0xDCCE);
    SpriteSystem_LoadCellResObjFromOpenNarc(showcase->spriteSystem, showcase->spriteManager, narc, 7, TRUE, 0xDCC3);
    SpriteSystem_LoadAnimResObjFromOpenNarc(showcase->spriteSystem, showcase->spriteManager, narc, 8, TRUE, 0xDCC3);
    SpriteSystem_LoadPlttResObjFromOpenNarc(showcase->spriteSystem, showcase->spriteManager, narc, 9, FALSE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0xDCCE);
    SpriteSystem_LoadCellResObjFromOpenNarc(showcase->spriteSystem, showcase->spriteManager, narc, 11, TRUE, 0xDCC1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(showcase->spriteSystem, showcase->spriteManager, narc, 12, TRUE, 0xDCC1);
    SpriteSystem_LoadCellResObjFromOpenNarc(showcase->spriteSystem, showcase->spriteManager, narc, 14, TRUE, 0xDCC2);
    SpriteSystem_LoadAnimResObjFromOpenNarc(showcase->spriteSystem, showcase->spriteManager, narc, 15, TRUE, 0xDCC2);
    for (i = 0xDCC2; i <= 0xDCCD; i++) {
        SpriteSystem_LoadPlttResObjFromOpenNarc(showcase->spriteSystem, showcase->spriteManager, narc, 16, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, i);
    }
    NARC_Delete(narc);

    showcase->sprites[0] = SpriteSystem_NewSpriteWithYOffset(showcase->spriteSystem, showcase->spriteManager, &sShowcaseMonPicSpriteTemplate1, FX32_CONST(512));
    showcase->sprites[1] = SpriteSystem_NewSpriteWithYOffset(showcase->spriteSystem, showcase->spriteManager, &sShowcaseMonPicSpriteTemplate2, FX32_CONST(512));
    ManagedSprite_SetDrawFlag(showcase->sprites[0], FALSE);
    ManagedSprite_SetDrawFlag(showcase->sprites[1], FALSE);
    showcase->sprites[14] = SpriteSystem_NewSprite(showcase->spriteSystem, showcase->spriteManager, &sShowcaseCursorSpriteTemplate);
    showcase->sprites[HOF_SHOWCASE_SPRITE_PREV_PAGE] = SpriteSystem_NewSprite(showcase->spriteSystem, showcase->spriteManager, &sShowcasePrevPageSpriteTemplate);
    showcase->sprites[HOF_SHOWCASE_SPRITE_NEXT_PAGE] = SpriteSystem_NewSprite(showcase->spriteSystem, showcase->spriteManager, &sShowcaseNextPageSpriteTemplate);
    showcase->sprites[HOF_SHOWCASE_SPRITE_EXIT] = SpriteSystem_NewSprite(showcase->spriteSystem, showcase->spriteManager, &sShowcaseExitSpriteTemplate);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

#pragma force_active reset

void HallOfFameShowcase_DeleteSprite(HallOfFameShowcase *showcase, u32 index) {
    if (showcase->sprites[index] != NULL) {
        Sprite_DeleteAndFreeResources(showcase->sprites[index]);
        showcase->sprites[index] = NULL;
    }
}

void HallOfFameShowcase_FreeSprites(HallOfFameShowcase *showcase) {
    u32 i;

    for (i = 0; i < HOF_SHOWCASE_NUM_SPRITES; i++) {
        HallOfFameShowcase_DeleteSprite(showcase, i);
    }
    SpriteSystem_FreeResourcesAndManager(showcase->spriteSystem, showcase->spriteManager);
    SpriteSystem_Free(showcase->spriteSystem);
}

void HallOfFameShowcase_TickSprites(HallOfFameShowcase *showcase) {
    u32 i;

    for (i = 0; i < HOF_SHOWCASE_NUM_SPRITES; i++) {
        if (showcase->sprites[i] != NULL) {
            ManagedSprite_TickFrame(showcase->sprites[i]);
        }
    }
}

#pragma force_active on

void HallOfFameShowcase_InitStrings(HallOfFameShowcase *showcase) {
    showcase->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0181_bin, HEAP_ID_59);
    showcase->speciesNames = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0237_bin, HEAP_ID_59);
    showcase->moveNames = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0750_bin, HEAP_ID_59);
    showcase->msgFormat = MessageFormat_New(HEAP_ID_59);
    showcase->stringBuf = String_New(256, HEAP_ID_59);
    showcase->stringBuf2 = String_New(256, HEAP_ID_59);
    showcase->nickname = String_New(256, HEAP_ID_59);
    showcase->otName = String_New(256, HEAP_ID_59);
    showcase->mon.nickname = showcase->nickname;
    showcase->mon.otname = showcase->otName;
}

#pragma force_active reset

void HallOfFameShowcase_FreeStrings(HallOfFameShowcase *showcase) {
    String_Delete(showcase->otName);
    String_Delete(showcase->nickname);
    String_Delete(showcase->stringBuf2);
    String_Delete(showcase->stringBuf);
    MessageFormat_Delete(showcase->msgFormat);
    DestroyMsgData(showcase->moveNames);
    DestroyMsgData(showcase->speciesNames);
    DestroyMsgData(showcase->msgData);
}

#pragma force_active on

void HallOfFameShowcase_InitWindows(HallOfFameShowcase *showcase) {
    u32 i;
    u32 width;

    FontID_Alloc(4, HEAP_ID_59);
    for (i = 0; i < HOF_SHOWCASE_NUM_WINDOWS; i++) {
        AddWindow(showcase->bgConfig, &showcase->windows[i], &sShowcaseWindowTemplates[i]);
    }
    FillWindowPixelBuffer(&showcase->windows[16], 0);
    ReadMsgDataIntoString(showcase->msgData, 6, showcase->stringBuf);
    width = FontID_String_GetWidth(4, showcase->stringBuf, 0);
    AddTextPrinterParameterizedWithColor(&showcase->windows[16], 4, showcase->stringBuf, 24 - width / 2, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 1, 0), NULL);
    CopyWindowPixelsToVram_TextMode(&showcase->windows[16]);
    ScheduleWindowCopyToVram(&showcase->windows[16]);
}

#pragma force_active reset

void HallOfFameShowcase_FreeWindows(HallOfFameShowcase *showcase) {
    u32 i;

    for (i = 0; i < HOF_SHOWCASE_NUM_WINDOWS; i++) {
        RemoveWindow(&showcase->windows[i]);
    }
    FontID_Release(4);
}

void HallOfFameShowcase_CreateGridInput(HallOfFameShowcase *showcase) {
    showcase->gridInput = GridInputHandler_Create(sShowcaseGridHitboxes, sShowcaseGridDpadBoxes, &sShowcaseGridCallbacks, showcase, TRUE, 0, HEAP_ID_59);
}

void HallOfFameShowcase_FreeGridInput(HallOfFameShowcase *showcase) {
    GridInputHandler_Free(showcase->gridInput);
}

void HallOfFameShowcase_GridOnButton(void) {
}

void HallOfFameShowcase_GridOnSwitchToTouchMode(void) {
}

void HallOfFameShowcase_GridOnTouch(HallOfFameShowcase *showcase, int target) {
    HallOfFameShowcase_MoveCursor(showcase, target);
}

void HallOfFameShowcase_GridOnKeyMove(HallOfFameShowcase *showcase, int target) {
    HallOfFameShowcase_MoveCursor(showcase, target);
}

void HallOfFameShowcase_BeginFadeIn(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_59);
}

void HallOfFameShowcase_BeginFadeOut(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_59);
}

static inline void HallOfFameShowcase_StartPrevPage(HallOfFameShowcase *showcase) {
    showcase->pressedButton = HOF_SHOWCASE_SPRITE_PREV_PAGE;
    showcase->nextState = 2;
    showcase->state = 1;
    HallOfFameShowcase_DeletePartyIcons(showcase);
    PlaySE(SEQ_SE_DP_SELECT);
}

static inline void HallOfFameShowcase_StartNextPage(HallOfFameShowcase *showcase) {
    showcase->pressedButton = HOF_SHOWCASE_SPRITE_NEXT_PAGE;
    showcase->nextState = 3;
    showcase->state = 1;
    HallOfFameShowcase_DeletePartyIcons(showcase);
    PlaySE(SEQ_SE_DP_SELECT);
}

#pragma force_active on

int HallOfFameShowcase_HandleInput(HallOfFameShowcase *showcase) {
    u32 input;

    switch (showcase->state) {
    case 0: {
        int hit = TouchscreenHitbox_FindRectAtTouchNew(_021E6E7C);
        if (hit == 0) {
            if (showcase->record > 0) {
                HallOfFameShowcase_StartPrevPage(showcase);
            }
        } else if (hit == 1) {
            if (showcase->numRecords >= 1 && showcase->record != showcase->numRecords - 1) {
                HallOfFameShowcase_StartNextPage(showcase);
            }
        } else {
            input = GridInputHandler_HandleInput_AllowHold(showcase->gridInput);
            switch (input) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                HallOfFameShowcase_ShowMonDetail(showcase, input);
                if (!(gSystem.newKeys & PAD_BUTTON_A) && input < showcase->numMons) {
                    PlaySE(SEQ_SE_DP_SELECT);
                }
                break;
            case 6:
            case GRID_MENU_CANCEL:
                PlaySE(SEQ_SE_DP_SELECT);
                showcase->pressedButton = HOF_SHOWCASE_SPRITE_EXIT;
                showcase->nextState = 4;
                showcase->state++;
                break;
            case GRID_MENU_CURSOR_MOVE:
                HallOfFameShowcase_ShowMonDetail(showcase, GridInputHandler_GetNextInput(showcase->gridInput));
                PlaySE(SEQ_SE_DP_SELECT);
                break;
            case GRID_MENU_NOTHING_CHOSEN:
                input = GridInputHandler_GetNextInput(showcase->gridInput);
                if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
                    if ((input == 2 || input == 5) && showcase->numRecords >= 1 && showcase->record != showcase->numRecords - 1) {
                        HallOfFameShowcase_StartNextPage(showcase);
                    }
                } else if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
                    if ((input == 0 || input == 3) && showcase->record > 0) {
                        HallOfFameShowcase_StartPrevPage(showcase);
                    }
                }
                break;
            case GRID_MENU_BUTTON_MODE:
                break;
            }
        }
        break;
    }
    case 1:
        if (!HallOfFameShowcase_AnimateButtonPress(showcase)) {
            showcase->state = showcase->nextState;
        }
        break;
    case 2:
        HallOfFameShowcase_ChangeRecord(showcase, -1);
        showcase->state = 0;
        break;
    case 3:
        HallOfFameShowcase_ChangeRecord(showcase, 1);
        showcase->state = 0;
        break;
    case 4:
        HallOfFameShowcase_BeginFadeOut();
        PlaySE(SEQ_SE_DP_PC_LOGOFF);
        return FALSE;
    }
    return TRUE;
}

#pragma force_active reset

void HallOfFameShowcase_DeletePartyIcons(HallOfFameShowcase *showcase) {
    u32 i;

    for (i = 2; i <= 13; i++) {
        HallOfFameShowcase_DeleteSprite(showcase, i);
    }
    for (i = 0xDCC2; i <= 0xDCCD; i++) {
        SpriteManager_UnloadCharObjById(showcase->spriteManager, i);
    }
}

#pragma force_active on

void HallOfFameShowcase_ChangeRecord(HallOfFameShowcase *showcase, int delta) {
    struct {
        int windowIndex;
        int resIdBase;
        RTCDate date;
    } page; // kept in one struct: matches the original stack layout

    u32 spriteIndex;
    u32 i;
    Window *window;
    HallOfFameShowcase *shifted;

    showcase->record += delta;
    if (showcase->record < 0) {
        showcase->record = 0;
    } else if (showcase->record >= showcase->numRecords) {
        showcase->record = showcase->numRecords - 1;
    }
    if (showcase->numRecords == 1) {
        ManagedSprite_SetDrawFlag(showcase->sprites[HOF_SHOWCASE_SPRITE_PREV_PAGE], FALSE);
        ManagedSprite_SetDrawFlag(showcase->sprites[HOF_SHOWCASE_SPRITE_NEXT_PAGE], FALSE);
    } else {
        if (showcase->record == 0) {
            ManagedSprite_SetDrawFlag(showcase->sprites[HOF_SHOWCASE_SPRITE_PREV_PAGE], FALSE);
        } else {
            ManagedSprite_SetDrawFlag(showcase->sprites[HOF_SHOWCASE_SPRITE_PREV_PAGE], TRUE);
        }
        if (showcase->record == showcase->numRecords - 1) {
            ManagedSprite_SetDrawFlag(showcase->sprites[HOF_SHOWCASE_SPRITE_NEXT_PAGE], FALSE);
        } else {
            ManagedSprite_SetDrawFlag(showcase->sprites[HOF_SHOWCASE_SPRITE_NEXT_PAGE], TRUE);
        }
    }
    showcase->recordNum = Save_HOF_TranslateRecordIdx(showcase->hallOfFame, showcase->record);
    showcase->numMons = Save_HOF_RecordCountMons(showcase->hallOfFame, showcase->record);
    if (showcase->iconSet == 0) {
        page.resIdBase = 0xDCC2;
        spriteIndex = 2;
        page.windowIndex = 7;
    } else {
        page.resIdBase = 0xDCC8;
        spriteIndex = 8;
        page.windowIndex = 15;
    }
    i = 0;
    if (i < showcase->numMons) {
        // The work pointer shifted by spriteIndex slots: sprites[0] through it is sprites[spriteIndex].
        shifted = (HallOfFameShowcase *)((u8 *)showcase + spriteIndex * sizeof(ManagedSprite *));
        do {
            Save_HOF_GetMonStatsByIndexPair(showcase->hallOfFame, showcase->record, i, &showcase->mon);
            HallOfFameShowcase_CreatePartyIcon(showcase, spriteIndex, i, page.resIdBase + i, page.resIdBase + i);
            ManagedSprite_SetAnimationFrame(shifted->sprites[0], 0);
            ManagedSprite_SetAnim(shifted->sprites[0], 0);
            i++;
            spriteIndex++;
            shifted = (HallOfFameShowcase *)((u8 *)shifted + sizeof(ManagedSprite *));
        } while (i < showcase->numMons);
    }
    HallOfFameShowcase_ShowMonDetail(showcase, GridInputHandler_GetNextInput(showcase->gridInput));

    window = &showcase->windows[page.windowIndex];
    FillWindowPixelBuffer(window, 0);
    ReadMsgDataIntoString(showcase->msgData, 0, showcase->stringBuf);
    BufferIntegerAsString(showcase->msgFormat, 0, Save_HOF_TranslateRecordIdx(showcase->hallOfFame, showcase->record), 4, PRINTING_MODE_LEFT_ALIGN, TRUE);
    Save_HOF_GetClearDate(showcase->hallOfFame, showcase->record, &page.date);
    BufferIntegerAsString(showcase->msgFormat, 1, page.date.year + 2000, 4, PRINTING_MODE_LEFT_ALIGN, TRUE);
    BufferMonthNameAbbr(showcase->msgFormat, 2, page.date.month);
    BufferIntegerAsString(showcase->msgFormat, 3, page.date.day, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(showcase->msgFormat, showcase->stringBuf2, showcase->stringBuf);
    AddTextPrinterParameterizedWithColor(window, 0, showcase->stringBuf2, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    CopyWindowPixelsToVram_TextMode(window);
    ScheduleWindowCopyToVram(window);
    showcase->iconSet ^= 1;
}

#pragma force_active reset

void HallOfFameShowcase_HideMonDetail(HallOfFameShowcase *showcase) {
    ManagedSprite_SetDrawFlag(showcase->sprites[0], FALSE);
    ManagedSprite_SetDrawFlag(showcase->sprites[1], FALSE);
    BgFillTilemapBufferAndSchedule(showcase->bgConfig, GF_BG_LYR_SUB_0, 0);
}

static inline void HallOfFameShowcase_PrintMonDetail(HallOfFameShowcase *showcase, s32 firstWindow, u32 gender) {
    u32 i;
    u32 width;

    for (i = 0; i < 7; i++) {
        FillWindowPixelBuffer(&showcase->windows[firstWindow + i], 0);
    }
    AddTextPrinterParameterizedWithColor(&showcase->windows[firstWindow], 0, showcase->mon.nickname, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    ReadMsgDataIntoString(showcase->speciesNames, showcase->mon.species, showcase->stringBuf);
    AddTextPrinterParameterizedWithColor(&showcase->windows[firstWindow + 1], 0, showcase->stringBuf, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    if (showcase->mon.species != SPECIES_NIDORAN_F && showcase->mon.species != SPECIES_NIDORAN_M) {
        if (gender == MON_MALE) {
            ReadMsgDataIntoString(showcase->msgData, 3, showcase->stringBuf2);
            width = FontID_String_GetWidth(0, showcase->stringBuf, 0);
            AddTextPrinterParameterizedWithColor(&showcase->windows[firstWindow + 1], 0, showcase->stringBuf2, width + 8, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 2, 0), NULL);
        } else if (gender == MON_FEMALE) {
            ReadMsgDataIntoString(showcase->msgData, 4, showcase->stringBuf2);
            width = FontID_String_GetWidth(0, showcase->stringBuf, 0);
            AddTextPrinterParameterizedWithColor(&showcase->windows[firstWindow + 1], 0, showcase->stringBuf2, width + 8, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 2, 0), NULL);
        }
    }
    ReadMsgDataIntoString(showcase->msgData, 1, showcase->stringBuf);
    BufferIntegerAsString(showcase->msgFormat, 0, showcase->mon.level, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    StringExpandPlaceholders(showcase->msgFormat, showcase->stringBuf2, showcase->stringBuf);
    AddTextPrinterParameterizedWithColor(&showcase->windows[firstWindow + 1], 0, showcase->stringBuf2, 0, 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    ReadMsgDataIntoString(showcase->msgData, 2, showcase->stringBuf);
    AddTextPrinterParameterizedWithColor(&showcase->windows[firstWindow + 2], 0, showcase->stringBuf, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    width = FontID_String_GetWidth(0, showcase->stringBuf, 0);
    AddTextPrinterParameterizedWithColor(&showcase->windows[firstWindow + 2], 0, showcase->mon.otname, width, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    for (i = 0; i < MAX_MON_MOVES; i++) {
        u16 move = showcase->mon.moves[i];
        if (move != MOVE_NONE) {
            ReadMsgDataIntoString(showcase->moveNames, move, showcase->stringBuf);
            width = FontID_String_GetWidth(0, showcase->stringBuf, 0);
            AddTextPrinterParameterizedWithColor(&showcase->windows[firstWindow + 3 + i], 0, showcase->stringBuf, 56 - (width >> 1), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 2, 0), NULL);
        }
    }
    for (i = 0; i < 7; i++) {
        CopyWindowPixelsToVram_TextMode(&showcase->windows[firstWindow + i]);
        ScheduleWindowCopyToVram(&showcase->windows[firstWindow + i]);
    }
}

#pragma force_active on

void HallOfFameShowcase_ShowMonDetail(HallOfFameShowcase *showcase, u32 index) {
    PokepicTemplate pokepic;
    UnkStruct_02014E30 picRect = sShowcaseMonPicRect;
    s8 yOffset[4];
    s32 firstWindow;
    u32 gender;
    u32 slot;
    ManagedSprite **sprites;
    int height;
    int shiny;
    void *buffer;
    u32 personality;

    if (index >= showcase->numMons) {
        HallOfFameShowcase_HideMonDetail(showcase);
        return;
    }
    ManagedSprite_SetDrawFlag(showcase->sprites[0], FALSE);
    ManagedSprite_SetDrawFlag(showcase->sprites[1], FALSE);
    if (showcase->monDetailSet == 0) {
        slot = 0;
        firstWindow = 0;
    } else {
        slot = 1;
        firstWindow = 8;
    }
    Save_HOF_GetMonStatsByIndexPair(showcase->hallOfFame, showcase->record, index, &showcase->mon);
    gender = GetGenderBySpeciesAndPersonality(showcase->mon.species, showcase->mon.personality);
    // The prototype in pokemon.h has an extra, unused parameter that no caller passes.
    ((void (*)(NARC *, s8 *, u16))sub_020729D8)(showcase->pokegraNarc, yOffset, showcase->mon.species);
    height = GetMonPicHeightBySpeciesGenderForm(showcase->mon.species, (u8)gender, 2, showcase->mon.form, showcase->mon.personality);
    sprites = showcase->sprites;
    ManagedSprite_SetPositionXYWithSubscreenOffset(showcase->sprites[slot], 64, (s16)(height + 64 - yOffset[0]), FX32_CONST(512));
    shiny = CalcShininessByOtIdAndPersonality(showcase->mon.otid, showcase->mon.personality) != 0;
    buffer = Heap_AllocAtEnd(HEAP_ID_59, 3200);
    personality = showcase->mon.personality;
    GetMonSpriteCharAndPlttNarcIdsEx(&pokepic, showcase->mon.species, GetGenderBySpeciesAndPersonality(showcase->mon.species, personality), 2, (u8)shiny, showcase->mon.form, personality);
    sub_02014510((NarcId)pokepic.narcID, pokepic.charDataID, HEAP_ID_59, &picRect, buffer, showcase->mon.personality, FALSE, 2, showcase->mon.species);
    HallOfFameShowcase_LoadSubObjChar(buffer, NNS_G2dGetImageLocation(Sprite_GetImageProxy(sprites[slot]->sprite), NNS_G2D_VRAM_TYPE_2DSUB), 3200);
    Heap_Free(buffer);
    GfGfxLoader_GXLoadPal((NarcId)pokepic.narcID, pokepic.palDataID, GF_PAL_LOCATION_SUB_OBJ, (enum GFPalSlotOffset)NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(sprites[slot]->sprite), NNS_G2D_VRAM_TYPE_2DSUB), 32, HEAP_ID_59);
    ManagedSprite_SetDrawFlag(sprites[slot], TRUE);
    HallOfFameShowcase_PrintMonDetail(showcase, firstWindow, gender);
    showcase->monDetailSet ^= 1;
}

#pragma force_active reset

int HallOfFameShowcase_AnimateButtonPress(HallOfFameShowcase *showcase) {
    if (showcase->buttonAnimTimer == 0) {
        ManagedSprite_SetPaletteOverride(showcase->sprites[showcase->pressedButton], 1);
    } else if (showcase->buttonAnimTimer == 4) {
        ManagedSprite_SetPaletteOverride(showcase->sprites[showcase->pressedButton], 0);
    } else if (showcase->buttonAnimTimer == 6) {
        showcase->buttonAnimTimer = 0;
        return FALSE;
    }
    showcase->buttonAnimTimer++;
    return TRUE;
}

void HallOfFameShowcase_MoveCursor(HallOfFameShowcase *showcase, int target) {
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(showcase->gridInput, target);

    ManagedSprite_SetPositionXY(showcase->sprites[14], box->left, box->top);
    if (target == 6) {
        target = 4;
    } else {
        target = 3;
    }
    ManagedSprite_SetAnimationFrame(showcase->sprites[14], 0);
    ManagedSprite_SetAnim(showcase->sprites[14], target);
}

void HallOfFameShowcase_CreatePartyIcon(HallOfFameShowcase *showcase, u32 spriteIndex, u32 position, int charResId, u32 plttResId) {
    u8 modelInfo[4];
    ManagedSpriteTemplate template;
    NARC *narc;
    int dim;
    int charFile;
    int cellAnimResId;
    u32 imageLocation;
    u32 tileSize;
    void *modelFile;
    NNSG3dResTex *tex;
    u8 *texData;
    u8 *buffer;
    u8 *plttData;
    u32 plttLocation;
    ManagedSprite **sprites;
    u32 spriteOffset;
    u16 member;

    ReadWholeNarcMemberByIdPair(modelInfo, NARC_fielddata_tsurepoke_tp_param, SpeciesToOverworldModelIndexOffset(showcase->mon.species));
    narc = NARC_New(NARC_a_0_9_7, HEAP_ID_59);
    if (modelInfo[1]) {
        cellAnimResId = 0xDCC2;
        charFile = 13;
        dim = 8;
    } else {
        cellAnimResId = 0xDCC1;
        charFile = 10;
        dim = 4;
    }
    SpriteSystem_LoadCharResObjFromOpenNarc(showcase->spriteSystem, showcase->spriteManager, narc, charFile, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, charResId);
    NARC_Delete(narc);

    template = sShowcasePartyIconSpriteTemplate;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = charResId;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = plttResId;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = cellAnimResId;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = cellAnimResId;
    template.x = sShowcaseIconPositions[position * 2];
    template.y = sShowcaseIconPositionsY[position * 2];
    sprites = showcase->sprites;
    spriteOffset = spriteIndex * sizeof(ManagedSprite *);
    *(ManagedSprite **)((u8 *)sprites + spriteOffset) = SpriteSystem_NewSprite(showcase->spriteSystem, showcase->spriteManager, &template);
    imageLocation = NNS_G2dGetImageLocation(Sprite_GetImageProxy((*(ManagedSprite **)((u8 *)sprites + spriteOffset))->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);

    tileSize = dim * (dim << 5);
    member = HallOfFameShowcase_GetOverworldModelFileId(showcase->mon.species, showcase->mon.form, GetGenderBySpeciesAndPersonality(showcase->mon.species, showcase->mon.personality));
    modelFile = AllocAndReadWholeNarcMemberByIdPair(NARC_data_mmodel_mmodel, member, HEAP_ID_59);
    tex = NNS_G3dGetTex((NNSG3dResFileHeader *)modelFile);
    texData = (u8 *)tex + tex->texInfo.ofsTex;
    buffer = Heap_Alloc(HEAP_ID_59, tileSize);
    // Krabby and Kingler (species 98, 99) walk sideways: their icon uses other frames of the texture.
    if ((u16)(showcase->mon.species + 0xFF9E) <= 1) {
        sub_020145B4(texData + tileSize * 6, dim, 0, 0, dim, dim, buffer);
        HallOfFameShowcase_LoadMainObjChar(buffer, imageLocation, tileSize);
        sub_020145B4(texData + tileSize * 7, dim, 0, 0, dim, dim, buffer);
        HallOfFameShowcase_LoadMainObjChar(buffer, imageLocation + tileSize, tileSize);
    } else {
        sub_020145B4(texData + tileSize * 2, dim, 0, 0, dim, dim, buffer);
        HallOfFameShowcase_LoadMainObjChar(buffer, imageLocation, tileSize);
        sub_020145B4(texData + tileSize * 3, dim, 0, 0, dim, dim, buffer);
        HallOfFameShowcase_LoadMainObjChar(buffer, imageLocation + tileSize, tileSize);
    }
    Heap_Free(buffer);
    plttLocation = NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy((*(ManagedSprite **)((u8 *)sprites + spriteOffset))->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    plttData = (u8 *)tex + tex->plttInfo.ofsPlttData;
    if (CalcShininessByOtIdAndPersonality(showcase->mon.otid, showcase->mon.personality)) {
        plttData += 0x20;
    }
    HallOfFameShowcase_LoadMainObjPltt(plttData, plttLocation, 0x20);
    Heap_Free(modelFile);
}

int HallOfFameShowcase_GetOverworldModelFileId(int species, int form, int isFemale) {
    int member;

    if (species <= SPECIES_NONE || species > NATIONAL_DEX_COUNT) {
        member = 1;
    } else {
        member = SpeciesToOverworldModelIndexOffset(species) + 0x129;
        if (OverworldModelLookupHasFemaleForm(species)) {
            if (isFemale == TRUE) {
                member++;
            }
        } else {
            if (form > OverworldModelLookupFormCount(species)) {
                form = 0;
            }
            member += form;
        }
    }
    return member;
}
