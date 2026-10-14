#include "overlay_trainer_card_main.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0727.h"

#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "overlay_manager.h"
#include "overlay_trainer_card_main_internal.h"
#include "palette.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_system.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "sound_playback.h"
#include "sprite_resource.h"
#include "oam_manager.h"
#include "touchpad.h"
#include "communication_core.h"
#include "wireless_status_icon.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

typedef struct TrainerCardPaletteTable {
    u32 members[6];
} TrainerCardPaletteTable;

typedef struct TrainerCardSpritePosition {
    s32 x;
    s32 y;
} TrainerCardSpritePosition;

typedef struct TrainerCardButtonPosition {
    u8 x;
    u8 y;
} TrainerCardButtonPosition;

typedef struct TrainerCardObjVramTypes {
    u32 types[2];
} TrainerCardObjVramTypes;

typedef struct TrainerCardObjFiles {
    u8 fileIds[2][4];
} TrainerCardObjFiles;

typedef struct TrainerCardBadgePalettes {
    u32 fileIds[16];
} TrainerCardBadgePalettes;

typedef struct TrainerCardBgPair {
    BgTemplate templates[2];
} TrainerCardBgPair;

extern const TouchscreenHitbox sTrainerCardCancelHitbox;
extern const TouchscreenHitbox sTrainerCardSignHitbox;
extern const TouchscreenHitbox sTrainerCardCardHitbox;
extern const GraphicsModes sTrainerCardGraphicsModes;
extern const TrainerCardPaletteTable sTrainerCardPlatinumPalettes;
extern const TrainerCardPaletteTable sTrainerCardDiamondPalettes;
extern const TrainerCardPaletteTable sTrainerCardHGSSPalettes;
extern const TrainerCardPaletteTable sTrainerCardPearlPalettes;
extern const BgTemplate sTrainerCardMainBg2Template;
extern const BgTemplate sTrainerCardMainBg3Template;
extern const BgTemplate sTrainerCardSubBg2Template;
extern const BgTemplate sTrainerCardSubBg3Template;
extern const GraphicsBanks sTrainerCardGraphicsBanks;
extern const TrainerCardBgPair sTrainerCardSubBg01Templates;
extern const u32 sTrainerCardCaseCharFileIds[];
extern const WindowTemplate sTrainerCardWindowTemplates[];
extern const u8 sTrainerCardResourceCounts[4];
extern const TrainerCardObjVramTypes sTrainerCardObjVramTypes;
extern const TrainerCardObjFiles sTrainerCardObjFileIds;
extern const ObjCharTransferTemplate sTrainerCardObjCharTransferTemplate;
extern const TrainerCardBadgePalettes sTrainerCardBadgePaletteFileIds;
extern const TrainerCardSpritePosition sTrainerCardBadgeSpritePositions[16];
extern const u8 sTrainerCardButtonSpriteAnims[];
extern const TrainerCardButtonPosition sTrainerCardButtonSpritePositions[];

void TrainerCardMain_LoadStrings(TrainerCardMain *cardMain);
void TrainerCardMain_FreeStrings(TrainerCardMain *cardMain);
void TrainerCardMain_SetGfxBanks(void);
void TrainerCardMain_LoadCardColorPalette(u32 stars, BOOL pokedexObtained, u8 version);
void TrainerCardMain_LoadCasePalette(int palette);
void TrainerCardMain_InitBgLayers(BgConfig *bgConfig);
void TrainerCardMain_LoadBgGraphics(TrainerCardMain *cardMain);
void TrainerCardMain_FreeBgLayers(BgConfig *bgConfig);
void TrainerCardMain_LoadSideLayout(TrainerCardMain *cardMain);
void TrainerCardMain_DrawCard(TrainerCardMain *cardMain);
int TrainerCardMain_RunSignPrompt(TrainerCardMain *cardMain);
int TrainerCardMain_RunFlip(TrainerCardMain *cardMain);
void TrainerCardMain_ApplyFlipScale(TrainerCardMain *cardMain);
int TrainerCardMain_HandleKeys(TrainerCardMain *cardMain);
int TrainerCardMain_HandleTouch(TrainerCardMain *cardMain, int *touched);
int TrainerCardMain_HandleInput(TrainerCardMain *cardMain);
void TrainerCardMain_VBlankCB(TrainerCardMain *cardMain);
void TrainerCardMain_ResetBgAffine(void);
void TrainerCardMain_DrawFrontPicture(TrainerCardMain *cardMain);
void TrainerCardMain_ClearBackSideRect(TrainerCardMain *cardMain);
void TrainerCardMain_UnpackSignature(u8 *signature, u8 *pixels);
void TrainerCardMain_DrawSignature(BgConfig *bgConfig, int bgId, u8 *pixels);
void TrainerCardMain_UpdatePlayTime(TrainerCardMain *cardMain, int update);
void TrainerCardMain_SetButtonsHidden(TrainerCardMain *cardMain, int hide);
void TrainerCardMain_CreateWindows(TrainerCardMain *cardMain);
void TrainerCardMain_DestroyWindows(TrainerCardMain *cardMain);
void TrainerCardMain_PrintFront(TrainerCardMain *cardMain, Window *windows, TrainerCardMainCard *card);
void TrainerCardMain_CopySideWindowsToVram(TrainerCardMain *cardMain, Window *windows);
void TrainerCardMain_PrintBack(TrainerCardMain *cardMain, Window *windows, TrainerCardMainCard *card);
void TrainerCardMain_ClearWindows(Window *windows, u8 first, u8 last);
void TrainerCardMain_PrintNumberRightAligned(Window *window, int right, int gap, int y, String *string, int value, u8 digits, int mode, int unused);
void TrainerCardMain_PrintStringRightAligned(Window *window, int right, int gap, int y, String *string);
void TrainerCardMain_PrintLivePlayTime(Window *windows, TrainerCardMainCard *card, String *unused);
void TrainerCardMain_DrawPlayTimeColon(Window *window, BOOL show, String *string);
void TrainerCardMain_PrintCancelButton(TrainerCardMain *cardMain, int unused);
void TrainerCardMain_PrintActionButton(TrainerCardMain *cardMain, int mode);
void TrainerCardMain_PrintSignPrompt(TrainerCardMain *cardMain, int message);
void TrainerCardMain_ShowYesNoPrompt(TrainerCardMain *cardMain, int unused);
int TrainerCardMain_HandleYesNoPrompt(TrainerCardMain *cardMain, BOOL keepFrame);
void TrainerCardMain_SetButtonWindowsHidden(TrainerCardMain *cardMain, int hide);
void TrainerCardMain_InitSprites(TrainerCardMainSprites *sprites);
void TrainerCardMain_CreateBadgeSprites(TrainerCardMainSprites *sprites, const u8 *badgeFlags, int showKantoBadges);
void TrainerCardMain_CreateButtonSprites(TrainerCardMainSprites *sprites);
void TrainerCardMain_FreeSprites(TrainerCardMainSprites *sprites);
void TrainerCardMain_SetButtonSpriteAnim(TrainerCardMainSprites *sprites, int button, int sequence, int draw);
void TrainerCardMain_InitObjTransfer(void);
void TrainerCardMain_InitUnk33A0Pairs(u8 *pairs);

#pragma force_active on

int TrainerCardMainApp_Init(OverlayManager *manager, int *state) {
    TrainerCardMain *cardMain;
    Options *options;
    int i;
    u16 mask;

    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xffffe0ff;
    reg_GXS_DB_DISPCNT &= 0xffffe0ff;
    SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
    SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
    SetKeyRepeatTimers(4, 8);
    Heap_Create(HEAP_ID_3, HEAP_ID_25, 0x50000);
    cardMain = OverlayManager_CreateAndGetData(manager, sizeof(TrainerCardMain), HEAP_ID_25);
    memset(cardMain, 0, sizeof(TrainerCardMain));
    cardMain->args = OverlayManager_GetArgs(manager);
    cardMain->card = (TrainerCardMainCard *)cardMain->args;
    cardMain->inTouchMode = MenuInputStateMgr_GetState(cardMain->args->unk66C.menuInputStatePtr);
    options = Save_PlayerData_GetOptionsAddr(cardMain->args->unk66C.saveData);
    cardMain->textSpeed = Options_GetTextFrameDelay(options);
    cardMain->frame = Options_GetFrame(options);
    if (Comm_IsActive() == 1) {
        cardMain->isConnected = TRUE;
    }
    if (cardMain->card->clearMonth != 0) {
        cardMain->showKantoBadges = TRUE;
    } else {
        cardMain->showKantoBadges = FALSE;
    }
    if (cardMain->isConnected == TRUE && cardMain->args->unk66C.gameCleared == FALSE) {
        cardMain->showKantoBadges = FALSE;
    }
    if ((u8)(cardMain->card->version + 0xf9) <= 1) {
        mask = 1;
        for (i = 0; i < 16; i++) {
            if (cardMain->card->badges & mask) {
                cardMain->badgeFlags[i] = 1;
            } else {
                cardMain->badgeFlags[i] = 0;
            }
            mask <<= 1;
        }
    } else {
        for (i = 0; i < 16; i++) {
            cardMain->badgeFlags[i] = 0;
        }
    }
    cardMain->bgConfig = BgConfig_Alloc(HEAP_ID_25);
    TrainerCardMain_LoadStrings(cardMain);
    TrainerCardMain_SetGfxBanks();
    TrainerCardMain_InitBgLayers(cardMain->bgConfig);
    TrainerCardMain_LoadBgGraphics(cardMain);
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    Sound_SetSceneAndPlayBGM(0x38, 0, 0);
    TrainerCardMain_InitUnk33A0Pairs(&cardMain->unk_33A0[0][0]);
    PlaySE(SEQ_SE_DP_CARD3);
    TrainerCardMain_InitSprites(&cardMain->sprites);
    TrainerCardMain_CreateBadgeSprites(&cardMain->sprites, cardMain->badgeFlags, cardMain->showKantoBadges);
    TrainerCardMain_CreateButtonSprites(&cardMain->sprites);
    TrainerCardMain_CreateWindows(cardMain);
    TrainerCardMain_DrawPlayTimeColon(&cardMain->windows[5], TRUE, cardMain->colonString);
    if (cardMain->args->unk66C.showBack) {
        cardMain->showingBack = TRUE;
    } else {
        cardMain->showingBack = FALSE;
    }
    cardMain->args->unk66C.showBack = 0;
    cardMain->unk_3434 = 0;
    cardMain->unk_3108 = -1;
    cardMain->unk_339C = 0;
    cardMain->unk_3438 = 0;
    TrainerCardMain_ResetBgAffine();
    TrainerCardMain_DrawCard(cardMain);
    TrainerCardMain_PrintCancelButton(cardMain, 0);
    Main_SetVBlankIntrCB((GFIntrCB)TrainerCardMain_VBlankCB, cardMain);
    WirelessStatusIcon_CreateIfConnected();
    GF_SndHandleSetPlayerVolume(1, 0x2a);
    BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_DOWNWARD_IN, FADE_TYPE_DOWNWARD_IN, RGB_BLACK, 6, 1, HEAP_ID_25);
    if (cardMain->card->version != VERSION_HEARTGOLD && cardMain->card->version != VERSION_SOULSILVER) {
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    }
    return TRUE;
}

#pragma force_active reset

int TrainerCardMainApp_Main(OverlayManager *manager, int *state) {
    TrainerCardMain *cardMain = OverlayManager_GetData(manager);

    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished()) {
            *state = 1;
        }
        break;
    case 1: {
        int input = TrainerCardMain_HandleInput(cardMain);
        if (input == 3) {
            cardMain->subState = 0;
            *state = 3;
        } else if (input == 4) {
            TrainerCardMain_SetButtonSpriteAnim(&cardMain->sprites, 1, 3, 1);
            cardMain->subState = 0;
            *state = 4;
        } else if (input == 5) {
            TrainerCardMain_SetButtonSpriteAnim(&cardMain->sprites, 0, 1, 1);
            BeginNormalPaletteFade(FADE_MAIN_THEN_SUB, FADE_TYPE_UPWARD_OUT, FADE_TYPE_UPWARD_OUT, RGB_BLACK, 6, 1, HEAP_ID_25);
            *state = 2;
        }
        TrainerCardMain_UpdatePlayTime(cardMain, cardMain->card->liveIgt);
        break;
    }
    case 2:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    case 3:
        if (TrainerCardMain_RunFlip(cardMain)) {
            *state = 1;
        }
        break;
    case 4: {
        switch (TrainerCardMain_RunSignPrompt(cardMain)) {
        case 1:
            *state = 1;
            break;
        case 2:
            *state = 5;
            break;
        }
        break;
    }
    case 5:
        BeginNormalPaletteFade(FADE_MAIN_THEN_SUB, FADE_TYPE_UPWARD_OUT, FADE_TYPE_UPWARD_OUT, RGB_BLACK, 6, 1, HEAP_ID_25);
        cardMain->args->unk66C.showBack = 1;
        *state = 2;
        break;
    }
    cardMain->scrollOffset++;
    if (cardMain->scrollOffset >= 0x80) {
        cardMain->scrollOffset = 0;
    }
    SpriteList_RenderAndAnimateSprites(cardMain->sprites.spriteList);
    return FALSE;
}

int TrainerCardMainApp_Exit(OverlayManager *manager, int *state) {
    TrainerCardMain *cardMain = OverlayManager_GetData(manager);

    TrainerCardMain_ResetBgAffine();
    TrainerCardMain_FreeStrings(cardMain);
    TrainerCardMain_FreeSprites(&cardMain->sprites);
    Heap_Free(cardMain->frontCharFile);
    Heap_Free(cardMain->badgeFrameScreenFile);
    Heap_Free(cardMain->frontScreenFile);
    TrainerCardMain_DestroyWindows(cardMain);
    TrainerCardMain_FreeBgLayers(cardMain->bgConfig);
    GF_TouchpadStopSampling();
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    TextFlags_SetCanABSpeedUpPrint(FALSE);
    MenuInputStateMgr_SetState(cardMain->args->unk66C.menuInputStatePtr, (MenuInputState)cardMain->inTouchMode);
    OverlayManager_FreeData(manager);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_25);
    GF_SndHandleSetPlayerVolume(1, 0x7f);
    return TRUE;
}

void TrainerCardMain_LoadStrings(TrainerCardMain *cardMain) {
    int i;

    FontID_Alloc(4, HEAP_ID_25);
    cardMain->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0727_bin, HEAP_ID_25);
    cardMain->unk_33C8 = String_New(4, HEAP_ID_25);
    cardMain->formattedString = String_New(0x20, HEAP_ID_25);
    cardMain->numberString = String_New(10, HEAP_ID_25);
    cardMain->colonString = String_New(5, HEAP_ID_25);
    ReadMsgDataIntoString(cardMain->msgData, msg_0727_00011, cardMain->colonString);
    for (i = 0; i < 3; i++) {
        cardMain->buttonStrings[i] = NewString_ReadMsgData(cardMain->msgData, i + 14);
    }
    for (i = 0; i < 2; i++) {
        cardMain->promptStrings[i] = NewString_ReadMsgData(cardMain->msgData, i + 17);
    }
    for (i = 0; i < 14; i++) {
        cardMain->labelStrings[i] = NewString_ReadMsgData(cardMain->msgData, i);
    }
}

void TrainerCardMain_FreeStrings(TrainerCardMain *cardMain) {
    int i;

    for (i = 0; i < 14; i++) {
        String_Delete(cardMain->labelStrings[i]);
    }
    String_Delete(cardMain->promptStrings[1]);
    String_Delete(cardMain->promptStrings[0]);
    String_Delete(cardMain->buttonStrings[2]);
    String_Delete(cardMain->buttonStrings[1]);
    String_Delete(cardMain->buttonStrings[0]);
    String_Delete(cardMain->colonString);
    String_Delete(cardMain->numberString);
    String_Delete(cardMain->formattedString);
    String_Delete(cardMain->unk_33C8);
    DestroyMsgData(cardMain->msgData);
    FontID_Release(4);
}

void TrainerCardMain_SetGfxBanks(void) {
    GraphicsBanks banks = sTrainerCardGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

#pragma force_active on

void TrainerCardMain_LoadCardColorPalette(u32 stars, BOOL pokedexObtained, u8 version) {
    u32 fileId;
    NNSG2dPaletteData *plttData;
    void *file;
    u8 *colors;

    if ((u8)(version + 249u) <= 1) {
        if (pokedexObtained) {
            TrainerCardPaletteTable table = sTrainerCardHGSSPalettes;
            if (stars > 5) {
                GF_AssertFail();
                fileId = *(volatile u32 *)&table.members[0];
            } else {
                fileId = table.members[stars];
            }
        } else {
            fileId = 6;
        }
    } else if (version == VERSION_DIAMOND) {
        TrainerCardPaletteTable table = sTrainerCardDiamondPalettes;
        if (stars > 5) {
            GF_AssertFail();
            fileId = *(volatile u32 *)&table.members[0];
        } else {
            fileId = table.members[stars];
        }
    } else if (version == VERSION_PEARL) {
        TrainerCardPaletteTable table = sTrainerCardPearlPalettes;
        if (stars > 5) {
            GF_AssertFail();
            fileId = *(volatile u32 *)&table.members[0];
        } else {
            fileId = table.members[stars];
        }
    } else if (version == VERSION_PLATINUM) {
        TrainerCardPaletteTable table = sTrainerCardPlatinumPalettes;
        if (stars > 5) {
            GF_AssertFail();
            fileId = *(volatile u32 *)&table.members[0];
        } else {
            fileId = table.members[stars];
        }
    } else {
        GF_AssertFail();
        fileId = 0;
    }
    file = GfGfxLoader_GetPlttData(NARC_a_0_4_9, fileId, &plttData, HEAP_ID_25);
    DC_FlushRange(plttData->pRawData, 512);
    colors = plttData->pRawData;
    GXS_LoadBGPltt(colors + 32, 32, 256);
    GXS_LoadBGPltt(colors + 480, 480, 32);
    Heap_Free(file);
}

#pragma force_active reset

void TrainerCardMain_LoadCasePalette(int palette) {
    NNSG2dPaletteData *plttData;
    void *file = GfGfxLoader_GetPlttData(NARC_a_0_4_9, 0x3c, &plttData, HEAP_ID_25);
    u8 *colors = (u8 *)plttData->pRawData + palette * 0x20;
    DC_FlushRange(colors, 0x20);
    GXS_LoadBGPltt(colors, 0x80, 0x20);
    Heap_Free(file);
}

void TrainerCardMain_InitBgLayers(BgConfig *bgConfig) {
    GraphicsModes modes;
    BgTemplate bg7;
    TrainerCardBgPair bg45;
    BgTemplate bg6;
    BgTemplate bg2;
    BgTemplate bg3;

    reg_GX_POWCNT |= 0x8000;
    modes = sTrainerCardGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    bg7 = sTrainerCardSubBg3Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &bg7, GF_BG_TYPE_256x16PLTT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
    bg45 = sTrainerCardSubBg01Templates;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &bg45.templates[0], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &bg45.templates[1], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    bg6 = sTrainerCardSubBg2Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &bg6, GF_BG_TYPE_256x16PLTT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    bg2 = sTrainerCardMainBg2Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &bg2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    bg3 = sTrainerCardMainBg3Template;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &bg3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
}

#pragma force_active on

void TrainerCardMain_LoadBgGraphics(TrainerCardMain *cardMain) {
    NNSG2dPaletteData *plttData;
    NNSG2dPaletteData *mainPlttData;
    void *file;

    file = GfGfxLoader_GetPlttData(NARC_a_0_4_9, 0, &plttData, HEAP_ID_25);
    DC_FlushRange(plttData->pRawData, 512);
    GXS_LoadBGPltt(plttData->pRawData, 0, 512);
    Heap_Free(file);
    TrainerCardMain_LoadCardColorPalette(cardMain->card->stars, cardMain->card->pokedexObtained, cardMain->card->version);
    file = GfGfxLoader_GetPlttData(NARC_a_0_4_9, 28, &mainPlttData, HEAP_ID_25);
    DC_FlushRange(mainPlttData->pRawData, 512);
    GX_LoadBGPltt(mainPlttData->pRawData, 0, 512);
    Heap_Free(file);
    if (cardMain->card->caseVariant == 255) {
        cardMain->frontCharFile = GfGfxLoader_LoadFromNarc(NARC_a_0_4_9, 44, FALSE, HEAP_ID_25, FALSE);
        GF_ASSERT(cardMain->frontCharFile != NULL);
        GF_ASSERT(NNS_G2dGetUnpackedBGCharacterData(cardMain->frontCharFile, &cardMain->frontCharData));
        if (cardMain->card->gender == 0) {
            cardMain->frontScreenFile = GfGfxLoader_GetScrnData(NARC_a_0_4_9, 54, FALSE, &cardMain->frontScreen, HEAP_ID_25);
        } else {
            cardMain->frontScreenFile = GfGfxLoader_GetScrnData(NARC_a_0_4_9, 55, FALSE, &cardMain->frontScreen, HEAP_ID_25);
        }
    } else {
        u8 variant = cardMain->card->caseVariant;
        u32 fileId = sTrainerCardCaseCharFileIds[variant];
        cardMain->frontCharFile = GfGfxLoader_LoadFromNarc(NARC_a_0_4_9, fileId, FALSE, HEAP_ID_25, FALSE);
        GF_ASSERT(cardMain->frontCharFile != NULL);
        GF_ASSERT(NNS_G2dGetUnpackedBGCharacterData(cardMain->frontCharFile, &cardMain->frontCharData));
        cardMain->frontScreenFile = GfGfxLoader_GetScrnData(NARC_a_0_4_9, 61, FALSE, &cardMain->frontScreen, HEAP_ID_25);
        TrainerCardMain_LoadCasePalette(cardMain->card->caseVariant);
    }
    TrainerCardMain_DrawFrontPicture(cardMain);
    GfGfxLoader_LoadCharData(NARC_a_0_4_9, 41, cardMain->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, HEAP_ID_25);
    GfGfxLoader_LoadScrnData(NARC_a_0_4_9, 47, cardMain->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, HEAP_ID_25);
    GfGfxLoader_LoadCharData(NARC_a_0_4_9, 42, cardMain->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_25);
    GfGfxLoader_LoadScrnData(NARC_a_0_4_9, 49, cardMain->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_25);
    GfGfxLoader_LoadCharData(NARC_a_0_4_9, 43, cardMain->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, HEAP_ID_25);
    cardMain->badgeFrameScreenFile = GfGfxLoader_GetScrnData(NARC_a_0_4_9, 53, FALSE, &cardMain->badgeFrameScreen, HEAP_ID_25);
    if (cardMain->showKantoBadges) {
        GfGfxLoader_LoadScrnData(NARC_a_0_4_9, 52, cardMain->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, HEAP_ID_25);
    } else {
        GfGfxLoader_LoadScrnData(NARC_a_0_4_9, 51, cardMain->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, HEAP_ID_25);
    }
    GfGfxLoader_LoadScrnData(NARC_a_0_4_9, 50, cardMain->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_25);
    if (!cardMain->showKantoBadges && cardMain->card->badges >= 255) {
        CopyToBgTilemapRect(cardMain->bgConfig, GF_BG_LYR_MAIN_2, 0, 7, 7, 9, cardMain->badgeFrameScreen->rawData, 0, 0, (u8)(cardMain->badgeFrameScreen->screenWidth >> 3), (u8)(cardMain->badgeFrameScreen->screenHeight >> 3));
    } else if (cardMain->card->badges == 65535) {
        CopyToBgTilemapRect(cardMain->bgConfig, GF_BG_LYR_MAIN_2, 0, 14, 7, 9, cardMain->badgeFrameScreen->rawData, 7, 0, (u8)(cardMain->badgeFrameScreen->screenWidth >> 3), (u8)(cardMain->badgeFrameScreen->screenHeight >> 3));
    }
    BgCommitTilemapBufferToVram(cardMain->bgConfig, GF_BG_LYR_MAIN_2);
    TrainerCardMain_UnpackSignature(cardMain->card->signature, cardMain->signaturePixels);
}

#pragma force_active reset

void TrainerCardMain_FreeBgLayers(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_ALL, FALSE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_ALL, FALSE);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
    Heap_Free(bgConfig);
}

void TrainerCardMain_LoadSideLayout(TrainerCardMain *cardMain) {
    if (cardMain->showingBack == FALSE) {
        GfGfxLoader_LoadScrnData(NARC_a_0_4_9, 0x2f, cardMain->bgConfig, (GFBgLayer)6, 0, 0, FALSE, HEAP_ID_25);
        TrainerCardMain_ClearWindows(cardMain->windows, 7, 10);
        BgClearTilemapBufferAndCommit(cardMain->bgConfig, GF_BG_LYR_SUB_3);
    } else {
        GfGfxLoader_LoadScrnData(NARC_a_0_4_9, 0x30, cardMain->bgConfig, (GFBgLayer)6, 0, 0, FALSE, HEAP_ID_25);
        TrainerCardMain_ClearWindows(cardMain->windows, 0, 6);
        TrainerCardMain_ClearBackSideRect(cardMain);
        BgClearTilemapBufferAndCommit(cardMain->bgConfig, GF_BG_LYR_SUB_3);
    }
}

void TrainerCardMain_DrawCard(TrainerCardMain *cardMain) {
    TrainerCardMainCard *card;

    TrainerCardMain_LoadSideLayout(cardMain);
    TrainerCardMain_SetButtonSpriteAnim(&cardMain->sprites, 0, 0, 1);
    if (cardMain->showingBack == FALSE) {
        TrainerCardMain_DrawFrontPicture(cardMain);
        TrainerCardMain_PrintActionButton(cardMain, 1);
    } else {
        TrainerCardMain_DrawSignature(cardMain->bgConfig, 7, cardMain->signaturePixels);
        TrainerCardMain_PrintActionButton(cardMain, 0);
    }
    card = cardMain->card;
    TrainerCardMain_PrintFront(cardMain, cardMain->windows, card);
    card = cardMain->card;
    TrainerCardMain_PrintBack(cardMain, cardMain->windows, card);
    TrainerCardMain_CopySideWindowsToVram(cardMain, cardMain->windows);
}

int TrainerCardMain_RunSignPrompt(TrainerCardMain *cardMain) {
    switch (cardMain->subState) {
    case 0:
        TrainerCardMain_PrintSignPrompt(cardMain, 0);
        cardMain->subState++;
        break;
    case 1:
        if (TextPrinterCheckActive(cardMain->textPrinterId)) {
            return 0;
        }
        TrainerCardMain_ShowYesNoPrompt(cardMain, 0);
        TrainerCardMain_SetButtonsHidden(cardMain, 1);
        cardMain->subState++;
        break;
    case 2: {
        int result = TrainerCardMain_HandleYesNoPrompt(cardMain, FALSE);
        if (result < 0) {
            break;
        }
        if (result == 0) {
            TrainerCardMain_SetButtonsHidden(cardMain, 0);
            return 1;
        }
        if (cardMain->card->signatureExists) {
            TrainerCardMain_PrintSignPrompt(cardMain, 1);
        } else {
            return 2;
        }
        cardMain->subState++;
        break;
    }
    case 3:
        if (TextPrinterCheckActive(cardMain->textPrinterId)) {
            return 0;
        }
        TrainerCardMain_ShowYesNoPrompt(cardMain, 1);
        cardMain->subState++;
        break;
    case 4: {
        int result = TrainerCardMain_HandleYesNoPrompt(cardMain, TRUE);
        switch (result) {
        case 0:
            TrainerCardMain_SetButtonsHidden(cardMain, 0);
            return 1;
        case 1:
            return 2;
        default:
            break;
        }
        break;
    }
    }
    return 0;
}

int TrainerCardMain_RunFlip(TrainerCardMain *cardMain) {
    int done = 0;

    switch (cardMain->subState) {
    case 0:
        cardMain->flipSpeed = 8;
        cardMain->scaleX = 8 << 9;
        cardMain->scaleY = 8 << 9;
        cardMain->scaleX += 0x80;
        cardMain->scaleY += 0x80;
        PlaySE(SEQ_SE_DP_CARD5);
        cardMain->subState++;
        break;
    case 1: {
        int speed = cardMain->flipSpeed;
        cardMain->scaleX -= 2 << (12 - speed);
        if (cardMain->scaleX <= 0) {
            cardMain->scaleX = 0x24;
            cardMain->subState++;
        }
        cardMain->flipSpeed--;
        if (cardMain->flipSpeed <= 1) {
            cardMain->flipSpeed = 1;
        }
        break;
    }
    case 2:
        cardMain->showingBack ^= 1;
        TrainerCardMain_LoadSideLayout(cardMain);
        cardMain->subState++;
        break;
    case 3:
        if (cardMain->showingBack == FALSE) {
            TrainerCardMain_DrawFrontPicture(cardMain);
            TrainerCardMain_PrintActionButton(cardMain, 1);
        } else {
            TrainerCardMain_DrawSignature(cardMain->bgConfig, 7, cardMain->signaturePixels);
            if (cardMain->isConnected == FALSE) {
                TrainerCardMain_PrintActionButton(cardMain, 0);
            }
        }
        cardMain->subState++;
        break;
    case 4:
        TrainerCardMain_CopySideWindowsToVram(cardMain, cardMain->windows);
        cardMain->subState++;
        break;
    case 5:
        cardMain->flipSpeed++;
        if (cardMain->flipSpeed > 8) {
            cardMain->flipSpeed = 8;
        }
        cardMain->scaleX += 2 << (12 - cardMain->flipSpeed);
        if (cardMain->scaleX >= 0x1000) {
            cardMain->scaleX = 0x1000;
            cardMain->scaleX = 0x1000;
            cardMain->scaleY = 0x1000;
            done = 1;
        }
        break;
    }
    cardMain->scaleChanged = TRUE;
    return done;
}

void TrainerCardMain_ApplyFlipScale(TrainerCardMain *cardMain) {
    MtxFx22 matrix;

    matrix._00 = FX_Inv(cardMain->scaleX);
    matrix._01 = 0;
    matrix._10 = 0;
    matrix._11 = FX_Inv(cardMain->scaleY);
    OS_WaitVBlankIntr();
    G2S_SetBG2Affine(&matrix, 0x80, 0x60, 0, 0);
    G2S_SetBG3Affine(&matrix, 0x80, 0x60, 0, 0);
    cardMain->scaleChanged = FALSE;
}

int TrainerCardMain_HandleKeys(TrainerCardMain *cardMain) {
    if (gSystem.newKeys & PAD_BUTTON_A) {
        if (cardMain->showingBack != FALSE && cardMain->isConnected == FALSE) {
            PlaySE(SEQ_SE_DP_SELECT);
            return 4;
        }
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return 5;
    }
    if (gSystem.newKeys & (PAD_KEY_LEFT | PAD_KEY_RIGHT)) {
        return 3;
    }
    return 0;
}

int TrainerCardMain_HandleTouch(TrainerCardMain *cardMain, int *touched) {
    if (!System_GetTouchNew()) {
        return 0;
    }
    if (TouchscreenHitbox_TouchNewIsIn(&sTrainerCardCancelHitbox)) {
        *touched = 1;
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return 5;
    }
    if (cardMain->showingBack != FALSE && cardMain->isConnected == FALSE && TouchscreenHitbox_TouchNewIsIn(&sTrainerCardSignHitbox)) {
        *touched = 1;
        PlaySE(SEQ_SE_DP_SELECT);
        return 4;
    }
    if (TouchscreenHitbox_TouchNewIsIn(&sTrainerCardCardHitbox)) {
        cardMain->touchX = gSystem.touchX;
        cardMain->touchY = gSystem.touchY;
        *touched = 1;
        return 3;
    }
    return 0;
}

#pragma force_active on

int TrainerCardMain_HandleInput(TrainerCardMain *cardMain) {
    int touched = 0;
    cardMain->unk_3108 = -1;
    {
        int result = TrainerCardMain_HandleTouch(cardMain, &touched);
        if (touched) {
            cardMain->inTouchMode = TRUE;
            return result;
        }
        result = TrainerCardMain_HandleKeys(cardMain);
        if (result) {
            cardMain->inTouchMode = FALSE;
        }
        return result;
    }
}

#pragma force_active reset

void TrainerCardMain_VBlankCB(TrainerCardMain *cardMain) {
    int pos = -cardMain->scrollOffset;
    BgSetPosTextAndCommit(cardMain->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_X, pos);
    BgSetPosTextAndCommit(cardMain->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, pos);
    BgSetPosTextAndCommit(cardMain->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, pos);
    BgSetPosTextAndCommit(cardMain->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, pos);
    if (cardMain->scaleChanged) {
        TrainerCardMain_ApplyFlipScale(cardMain);
    }
    DoScheduledBgGpuUpdates(cardMain->bgConfig);
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    *(vu32 *)((u8 *)OS_IRQTable + 0x3ff8) |= OS_IE_VBLANK; // OS_SetIrqCheckFlag, addressed from OS_IRQTable
}

#pragma force_active on

void TrainerCardMain_ResetBgAffine(void) {
    MtxFx22 matrix;
    int x = FX_Inv(FX32_ONE);
    int y = FX_Inv(FX32_ONE);
    matrix._00 = x;
    matrix._01 = 0;
    matrix._10 = 0;
    matrix._11 = y;
    OS_WaitVBlankIntr();
    G2S_SetBG2Affine(&matrix, 128, 96, 0, 0);
    G2S_SetBG3Affine(&matrix, 128, 96, 0, 0);
    OS_WaitVBlankIntr();
    G2_SetBG3Affine(&matrix, 128, 0, 0, 0);
}

void TrainerCardMain_DrawFrontPicture(TrainerCardMain *cardMain) {
    NNSG2dScreenData *screen;
    BG_LoadCharTilesData(cardMain->bgConfig, 7, cardMain->frontCharData->pRawData, cardMain->frontCharData->szByte, 0);
    screen = cardMain->frontScreen;
    CopyToBgTilemapRect(cardMain->bgConfig, GF_BG_LYR_SUB_3, 21, 5, 10, 11, screen->rawData, 21, 5, (u8)(screen->screenWidth >> 3), (u8)(screen->screenHeight >> 3));
    BgCommitTilemapBufferToVram(cardMain->bgConfig, GF_BG_LYR_SUB_3);
}

#pragma force_active reset

void TrainerCardMain_ClearBackSideRect(TrainerCardMain *cardMain) {
    FillBgTilemapRect(cardMain->bgConfig, GF_BG_LYR_SUB_3, 0, 0x14, 6, 6, 9, TILEMAP_FILL_KEEP_PAL);
}

#pragma force_active on

void TrainerCardMain_UnpackSignature(u8 *signature, u8 *pixels) {
    int count = 0x3000;
    int i = 0;
    do {
        u8 col = (i / 8) % 8;
        u8 bit = i % 8;
        pixels[i] = (signature[(i / 64) * 8 + col] >> bit) & 1;
        i++;
    } while (i < count);
}

void TrainerCardMain_DrawSignature(BgConfig *bgConfig, int bgId, u8 *pixels) {
    u16 *tilemap;
    u8 x;
    u8 y;
    u16 tile;

    BG_LoadCharTilesData(bgConfig, (u8)bgId, pixels, 12288, 1);
    tilemap = GetBgTilemapBuffer(bgConfig, (u8)bgId);
    tile = 0;
    y = 0;
    do {
        x = 0;
        do {
            tile++;
            tilemap[(y + 12) * 32 + x + 4] = tile;
            x++;
        } while (x < 24);
        y++;
    } while (y < 8);
    ScheduleBgTilemapBufferTransfer(bgConfig, (u8)bgId);
}

#pragma force_active reset

void TrainerCardMain_UpdatePlayTime(TrainerCardMain *cardMain, int update) {
    if (update != 0) {
        if (cardMain->showingBack == FALSE) {
            if (cardMain->colonBlinkTimer == 0) {
                TrainerCardMain_PrintLivePlayTime(cardMain->windows, cardMain->card, cardMain->unk_33C8);
                TrainerCardMain_DrawPlayTimeColon(&cardMain->windows[5], TRUE, cardMain->colonString);
            } else if (cardMain->colonBlinkTimer == 15) {
                TrainerCardMain_DrawPlayTimeColon(&cardMain->windows[5], FALSE, cardMain->colonString);
            }
        }
        cardMain->colonBlinkTimer = (cardMain->colonBlinkTimer + 1) % 30;
    }
}

void TrainerCardMain_SetButtonsHidden(TrainerCardMain *cardMain, int hide) {
    if (hide == 1) {
        TrainerCardMain_SetButtonSpriteAnim(&cardMain->sprites, 1, 2, 0);
        TrainerCardMain_SetButtonSpriteAnim(&cardMain->sprites, 0, 0, 0);
    } else {
        TrainerCardMain_SetButtonSpriteAnim(&cardMain->sprites, 1, 2, 1);
        TrainerCardMain_SetButtonSpriteAnim(&cardMain->sprites, 0, 0, 1);
    }
    TrainerCardMain_SetButtonWindowsHidden(cardMain, hide);
}

void TrainerCardMain_CreateWindows(TrainerCardMain *cardMain) {
    const WindowTemplate *templates = sTrainerCardWindowTemplates;
    u8 i = 0;
    do {
        AddWindow(cardMain->bgConfig, &cardMain->windows[i], &templates[i]);
        i++;
    } while (i < 14);
    BG_FillCharDataRange(cardMain->bgConfig, (GFBgLayer)7, 0, 1, 0);
    BG_FillCharDataRange(cardMain->bgConfig, (GFBgLayer)4, 0, 1, 0);
    LoadUserFrameGfx2(cardMain->bgConfig, (GFBgLayer)4, 0x3e1, 13, cardMain->frame, HEAP_ID_25);
    LoadFontPal0((enum GFPalLoadLocation)4, (enum GFPalSlotOffset)0x1c0, HEAP_ID_25);
    cardMain->yesNoPrompt = YesNoPrompt_Create(HEAP_ID_25);
    ScheduleBgTilemapBufferTransfer(cardMain->bgConfig, GF_BG_LYR_SUB_3);
    ScheduleBgTilemapBufferTransfer(cardMain->bgConfig, GF_BG_LYR_SUB_0);
}

void TrainerCardMain_DestroyWindows(TrainerCardMain *cardMain) {
    u16 i;
    YesNoPrompt_Destroy(cardMain->yesNoPrompt);
    for (i = 0; i < 14; i++) {
        RemoveWindow(&cardMain->windows[i]);
    }
}

#pragma force_active on

void TrainerCardMain_PrintFront(TrainerCardMain *cardMain, Window *windows, TrainerCardMainCard *card) {
    MsgData *msgData = cardMain->msgData;
    u8 i = 0;
    String *template;
    String *formatted;
    MessageFormat *format;
    String *number;
    int x;

    for (i = 0; i < 7; i++) {
        FillWindowPixelBuffer(&windows[i], 0);
        if (i != 3 || (i == 3 && card->pokedexObtained)) {
            AddTextPrinterParameterizedWithColor(&windows[i], 0, cardMain->labelStrings[i], 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        }
    }
    template = String_New(32, HEAP_ID_25);
    formatted = cardMain->formattedString;
    format = MessageFormat_New_Custom(6, 32, HEAP_ID_25);
    number = cardMain->numberString;
    TrainerCardMain_PrintNumberRightAligned(&windows[0], 96, 0, 0, number, card->trainerId, 5, 2, 255);
    CopyU16ArrayToString(cardMain->formattedString, card->name);
    TrainerCardMain_PrintStringRightAligned(&windows[1], 104, 0, 0, cardMain->formattedString);
    BufferIntegerAsString(format, 5, card->money, 6, PRINTING_MODE_LEFT_ALIGN, TRUE);
    ReadMsgDataIntoString(msgData, msg_0727_00019, template);
    StringExpandPlaceholders(format, formatted, template);
    x = 136 - FontID_String_GetWidth(0, formatted, 0);
    AddTextPrinterParameterizedWithColor(&windows[2], 0, formatted, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    if (card->pokedexObtained) {
        BufferIntegerAsString(format, 5, card->pokedexCount, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
        ReadMsgDataIntoString(msgData, msg_0727_00026, template);
        StringExpandPlaceholders(format, formatted, template);
        x = 136 - FontID_String_GetWidth(0, formatted, 0);
        AddTextPrinterParameterizedWithColor(&windows[3], 0, formatted, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    }
    TrainerCardMain_PrintNumberRightAligned(&windows[4], 136, 0, 0, number, card->score, 9, 1, 255);
    if (card->liveIgt) {
        BufferIntegerAsString(format, 0, GetIGTHours(card->igt), 3, PRINTING_MODE_RIGHT_ALIGN, TRUE);
        BufferIntegerAsString(format, 1, GetIGTMinutes(card->igt), 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
        ReadMsgDataIntoString(msgData, msg_0727_00021, template);
    } else {
        BufferIntegerAsString(format, 0, card->playHours, 3, PRINTING_MODE_RIGHT_ALIGN, TRUE);
        BufferIntegerAsString(format, 1, card->playMinutes, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
        ReadMsgDataIntoString(msgData, msg_0727_00020, template);
    }
    StringExpandPlaceholders(format, formatted, template);
    x = 224 - FontID_String_GetWidth(0, formatted, 0);
    AddTextPrinterParameterizedWithColor(&windows[5], 0, formatted, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    BufferIntegerAsString(format, 2, card->startYear, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BufferMonthNameAbbr(format, 3, card->startMonth);
    BufferIntegerAsString(format, 4, card->startDay, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    ReadMsgDataIntoString(msgData, msg_0727_00022, template);
    StringExpandPlaceholders(format, formatted, template);
    x = 224 - FontID_String_GetWidth(0, formatted, 0);
    AddTextPrinterParameterizedWithColor(&windows[6], 0, formatted, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(template);
    MessageFormat_Delete(format);
}

#pragma force_active reset

void TrainerCardMain_CopySideWindowsToVram(TrainerCardMain *cardMain, Window *windows) {
    int i;
    if (cardMain->showingBack != FALSE) {
        for (i = 7; i < 11; i++) {
            CopyWindowToVram(&windows[i]);
        }
    } else {
        for (i = 0; i < 7; i++) {
            CopyWindowToVram(&windows[i]);
        }
    }
}

void TrainerCardMain_PrintBack(TrainerCardMain *cardMain, Window *windows, TrainerCardMainCard *card) {
    String *template;
    String *formatted;
    MessageFormat *format;
    MsgData *msgData;
    u8 i;
    int x;

    msgData = cardMain->msgData;
    for (i = 7; i < 11; i++) {
        FillWindowPixelBuffer(&windows[i], 0);
        AddTextPrinterParameterizedWithColor(&windows[i], 0, cardMain->labelStrings[i], 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    }
    template = String_New(0x20, HEAP_ID_25);
    formatted = cardMain->formattedString;
    format = MessageFormat_New_Custom(6, 0x20, HEAP_ID_25);
    if (card->clearMonth != 0) {
        BufferIntegerAsString(format, 2, card->clearYear, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
        BufferMonthNameAbbr(format, 3, card->clearMonth);
        BufferIntegerAsString(format, 4, card->clearDay, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
        BufferIntegerAsString(format, 0, card->clearHours, 3, PRINTING_MODE_RIGHT_ALIGN, TRUE);
        BufferIntegerAsString(format, 1, card->clearMinutes, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
        ReadMsgDataIntoString(msgData, msg_0727_00022, template);
        StringExpandPlaceholders(format, formatted, template);
    } else {
        ReadMsgDataIntoString(msgData, msg_0727_00012, template);
        BufferString(format, 0, template, 0, 0, 2);
        BufferString(format, 1, template, 0, 0, 2);
        ReadMsgDataIntoString(msgData, msg_0727_00025, formatted);
    }
    x = 0xE0 - FontID_String_GetWidth(0, formatted, 0);
    AddTextPrinterParameterizedWithColor(&windows[7], 0, formatted, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    ReadMsgDataIntoString(msgData, msg_0727_00020, template);
    StringExpandPlaceholders(format, formatted, template);
    x = 0xE0 - FontID_String_GetWidth(0, formatted, 0);
    AddTextPrinterParameterizedWithColor(&windows[7], 0, formatted, x, 0x10, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    BufferIntegerAsString(format, 5, card->linkCount, 6, PRINTING_MODE_LEFT_ALIGN, TRUE);
    ReadMsgDataIntoString(msgData, msg_0727_00027, template);
    StringExpandPlaceholders(format, formatted, template);
    x = 0xE0 - FontID_String_GetWidth(0, formatted, 0);
    AddTextPrinterParameterizedWithColor(&windows[8], 0, formatted, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    ReadMsgDataIntoString(msgData, msg_0727_00023, formatted);
    AddTextPrinterParameterizedWithColor(&windows[9], 0, formatted, 0x70, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    TrainerCardMain_PrintNumberRightAligned(&windows[9], 0xE0, 0, 0, formatted, card->linkBattleLosses, 4, 1, 0xFF);
    ReadMsgDataIntoString(msgData, msg_0727_00024, formatted);
    AddTextPrinterParameterizedWithColor(&windows[9], 0, formatted, 0xB0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    TrainerCardMain_PrintNumberRightAligned(&windows[9], 0xE0, 0x40, 0, formatted, card->linkBattleWins, 4, 1, 0xFF);
    BufferIntegerAsString(format, 5, card->linkTrades, 6, PRINTING_MODE_LEFT_ALIGN, TRUE);
    ReadMsgDataIntoString(msgData, msg_0727_00027, template);
    StringExpandPlaceholders(format, formatted, template);
    x = 0xE0 - FontID_String_GetWidth(0, formatted, 0);
    AddTextPrinterParameterizedWithColor(&windows[10], 0, formatted, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(template);
    MessageFormat_Delete(format);
}

void TrainerCardMain_ClearWindows(Window *windows, u8 first, u8 last) {
    u8 i;
    for (i = first; i <= last; i++) {
        ClearWindowTilemapAndCopyToVram(&windows[i]);
    }
}

void TrainerCardMain_PrintNumberRightAligned(Window *window, int right, int gap, int y, String *string, int value, u8 digits, int mode, int unused) {
    int width;
    String16_FormatInteger(string, value, digits, (PrintingMode)mode, TRUE);
    width = FontID_String_GetWidth(0, string, 0);
    AddTextPrinterParameterizedWithColor(window, 0, string, right - (width + gap), y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
}

void TrainerCardMain_PrintStringRightAligned(Window *window, int right, int gap, int y, String *string) {
    int width = FontID_String_GetWidth(0, string, 0);
    AddTextPrinterParameterizedWithColor(window, 0, string, right - (width + gap), y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
}

#pragma force_active on

void TrainerCardMain_PrintLivePlayTime(Window *windows, TrainerCardMainCard *card, String *unused) {
    int x;
    MessageFormat *format;
    String *formatted;
    MsgData *msgData;
    String *template;

    GF_ASSERT(card->igt != NULL);
    GetIGTHours(card->igt);
    FillWindowPixelRect(&windows[5], 0, 184, 0, 40, 16);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0727_bin, HEAP_ID_25);
    formatted = String_New(32, HEAP_ID_25);
    template = String_New(32, HEAP_ID_25);
    format = MessageFormat_New_Custom(2, 32, HEAP_ID_25);
    BufferIntegerAsString(format, 0, GetIGTHours(card->igt), 3, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    BufferIntegerAsString(format, 1, GetIGTMinutes(card->igt), 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    ReadMsgDataIntoString(msgData, msg_0727_00021, template);
    StringExpandPlaceholders(format, formatted, template);
    x = 224 - FontID_String_GetWidth(0, formatted, 0);
    AddTextPrinterParameterizedWithColor(&windows[5], 0, formatted, x, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    DestroyMsgData(msgData);
    String_Delete(formatted);
    String_Delete(template);
    MessageFormat_Delete(format);
}

#pragma force_active reset

void TrainerCardMain_DrawPlayTimeColon(Window *window, BOOL show, String *string) {
    if (show != FALSE) {
        AddTextPrinterParameterizedWithColor(window, 0, string, 0xcd, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    } else {
        FillWindowPixelRect(window, 0, 0xcd, 0, 5, 0x10);
        CopyWindowToVram(window);
    }
}

void TrainerCardMain_PrintCancelButton(TrainerCardMain *cardMain, int unused) {
    int x = 0x30 - FontID_String_GetWidth(4, cardMain->buttonStrings[0], 0);
    FillWindowPixelBuffer(&cardMain->windows[13], 0);
    AddTextPrinterParameterizedWithColor(&cardMain->windows[13], 4, cardMain->buttonStrings[0], (u32)x >> 1, 4, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(5, 4, 0), NULL);
}

void TrainerCardMain_PrintActionButton(TrainerCardMain *cardMain, int mode) {
    Window *window = &cardMain->windows[12];
    if (mode == 0) {
        if (cardMain->isConnected) {
            mode = 1;
        }
    }
    switch (mode) {
    case 0: {
        int width;
        FillWindowPixelBuffer(window, 0);
        width = 0x60 - FontID_String_GetWidth(4, cardMain->buttonStrings[2], 0);
        AddTextPrinterParameterizedWithColor(window, 4, cardMain->buttonStrings[2], (u32)width >> 1, 4, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(5, 4, 0), NULL);
        TrainerCardMain_SetButtonSpriteAnim(&cardMain->sprites, 1, 2, 1);
        break;
    }
    case 1:
        FillWindowPixelBuffer(window, 0);
        AddTextPrinterParameterizedWithColor(window, 0, cardMain->buttonStrings[1], 0, 4, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(5, 4, 0), NULL);
        TrainerCardMain_SetButtonSpriteAnim(&cardMain->sprites, 1, 4, 1);
        break;
    }
}

void TrainerCardMain_PrintSignPrompt(TrainerCardMain *cardMain, int message) {
    if (message == 0) {
        DrawFrameAndWindow2(&cardMain->windows[11], TRUE, 0x3e1, 0xd);
    }
    FillWindowPixelBuffer(&cardMain->windows[11], 0xf);
    cardMain->textPrinterId = AddTextPrinterParameterizedWithColor(&cardMain->windows[11], 1, cardMain->promptStrings[message], 0, 0, cardMain->textSpeed, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    ScheduleBgTilemapBufferTransfer(cardMain->bgConfig, GF_BG_LYR_SUB_0);
}

void TrainerCardMain_ShowYesNoPrompt(TrainerCardMain *cardMain, int unused) {
    YesNoPromptTemplate template;
    MI_CpuFill8(&template, 0, sizeof(template));
    template.bgConfig = cardMain->bgConfig;
    template.bgId = 4;
    template.tileStart = 0x3A5;
    template.plttSlot = 0xB;
    template.x = 0x1A;
    template.y = 6;
    template.ignoreTouchFlag = cardMain->inTouchMode;
    template.initialCursorPos = 0;
    YesNoPrompt_InitFromTemplate(cardMain->yesNoPrompt, &template);
    ScheduleBgTilemapBufferTransfer(cardMain->bgConfig, GF_BG_LYR_SUB_0);
}

int TrainerCardMain_HandleYesNoPrompt(TrainerCardMain *cardMain, BOOL keepFrame) {
    int answer;
    int result = YesNoPrompt_HandleInput(cardMain->yesNoPrompt);
    switch (result) {
    case 1:
        answer = 1;
        break;
    case 2:
        answer = 0;
        break;
    default:
        return -1;
    }
    cardMain->inTouchMode = YesNoPrompt_IsInTouchMode(cardMain->yesNoPrompt);
    YesNoPrompt_Reset(cardMain->yesNoPrompt);
    if (!answer || keepFrame) {
        ClearFrameAndWindow2(&cardMain->windows[11], FALSE);
        ScheduleBgTilemapBufferTransfer(cardMain->bgConfig, GF_BG_LYR_SUB_0);
    }
    return answer;
}

void TrainerCardMain_SetButtonWindowsHidden(TrainerCardMain *cardMain, int hide) {
    if (hide == 1) {
        ClearWindowTilemapAndScheduleTransfer(&cardMain->windows[13]);
        ClearWindowTilemapAndScheduleTransfer(&cardMain->windows[12]);
    } else {
        ScheduleWindowCopyToVram(&cardMain->windows[13]);
        ScheduleWindowCopyToVram(&cardMain->windows[12]);
    }
}

#pragma force_active on

void TrainerCardMain_InitSprites(TrainerCardMainSprites *sprites) {
    TrainerCardBadgePalettes paletteFiles;
    TrainerCardObjVramTypes vramTypes;
    TrainerCardObjFiles files;
    NARC *narc;
    int i;
    u8 j;

    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_128K);
    GXS_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
    TrainerCardMain_InitObjTransfer();
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 128, 0, 32, 0, 128, 0, 32, HEAP_ID_25);
    sprites->spriteList = G2dRenderer_Init(33, &sprites->renderer, HEAP_ID_25);
    G2dRenderer_SetSubSurfaceCoords(&sprites->renderer, 0, FX32_CONST(224));
    for (i = 0; i < 4; i++) {
        sprites->resourceManagers[0][i] = Create2DGfxResObjMan(sTrainerCardResourceCounts[i], (GfGfxResType)i, HEAP_ID_25);
        sprites->resourceManagers[1][i] = Create2DGfxResObjMan(sTrainerCardResourceCounts[i], (GfGfxResType)i, HEAP_ID_25);
    }
    i = 0;
    vramTypes = sTrainerCardObjVramTypes;
    files = sTrainerCardObjFileIds;
    for (; i < 2; i++) {
        sprites->resourceObjects[i][0] = AddCharResObjFromNarc(sprites->resourceManagers[i][0], NARC_a_0_4_9, files.fileIds[i][0], FALSE, i, vramTypes.types[i], HEAP_ID_25);
        sprites->resourceObjects[i][1] = AddPlttResObjFromNarc(sprites->resourceManagers[i][1], NARC_a_0_4_9, files.fileIds[i][1], FALSE, i, vramTypes.types[i], 16, HEAP_ID_25);
        sprites->resourceObjects[i][2] = AddCellOrAnimResObjFromNarc(sprites->resourceManagers[i][2], NARC_a_0_4_9, files.fileIds[i][2], FALSE, i, GF_GFX_RES_TYPE_CELL, HEAP_ID_25);
        sprites->resourceObjects[i][3] = AddCellOrAnimResObjFromNarc(sprites->resourceManagers[i][3], NARC_a_0_4_9, files.fileIds[i][3], FALSE, i, GF_GFX_RES_TYPE_ANIM, HEAP_ID_25);
        SpriteTransfer_CreateCharTransferTask(sprites->resourceObjects[i][0]);
        SpriteTransfer_CreateExtPlttTransferTask(sprites->resourceObjects[i][1]);
    }
    paletteFiles = sTrainerCardBadgePaletteFileIds;
    narc = NARC_New(NARC_a_0_4_9, HEAP_ID_25);
    for (j = 0; j < 16; j++) {
        sprites->paletteFiles[j] = NARC_AllocAndReadWholeMember(narc, paletteFiles.fileIds[j], HEAP_ID_25);
        if (sprites->paletteFiles[j] != NULL) {
            if (!NNS_G2dGetUnpackedPaletteData(sprites->paletteFiles[j], &sprites->palettes[j])) {
                Heap_Free(sprites->palettes[j]);
                GF_AssertFail();
            }
        } else {
            GF_AssertFail();
        }
    }
    NARC_Delete(narc);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

#pragma force_active reset

void TrainerCardMain_CreateBadgeSprites(TrainerCardMainSprites *sprites, const u8 *badgeFlags, int showKantoBadges) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    int yOffset;
    int i;
    TrainerCardMainSprites *cursor; // advanced by one Sprite pointer per badge
    const TrainerCardSpritePosition *position;

    CreateSpriteResourcesHeader(&header, 0, 0, 0, 0, -1, -1, 0, 1, sprites->resourceManagers[0][0], sprites->resourceManagers[0][1], sprites->resourceManagers[0][2], sprites->resourceManagers[0][3], NULL, NULL);
    template.spriteList = sprites->spriteList;
    template.header = &header;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = 2;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = HEAP_ID_25;
    if (showKantoBadges == 0) {
        yOffset = 40;
    } else {
        yOffset = 0;
    }
    cursor = sprites;
    position = sTrainerCardBadgeSpritePositions;
    for (i = 0; i < 16; i++, position++, cursor = (TrainerCardMainSprites *)((u8 *)cursor + sizeof(Sprite *))) {
        template.position.x = position->x << FX32_SHIFT;
        template.position.y = (yOffset + position->y) << FX32_SHIFT;
        cursor->badgeSprites[0] = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(cursor->badgeSprites[0], TRUE);
        Sprite_SetAnimCtrlSeq(cursor->badgeSprites[0], i);
        if (badgeFlags[i] == 0 || (showKantoBadges == 0 && i > 7)) {
            Sprite_SetDrawFlag(cursor->badgeSprites[0], FALSE);
        }
    }
}

#pragma force_active on

void TrainerCardMain_CreateButtonSprites(TrainerCardMainSprites *sprites) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    int i;

    CreateSpriteResourcesHeader(&header, 1, 1, 1, 1, -1, -1, 0, 1, sprites->resourceManagers[1][0], sprites->resourceManagers[1][1], sprites->resourceManagers[1][2], sprites->resourceManagers[1][3], NULL, NULL);
    template.spriteList = sprites->spriteList;
    template.header = &header;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = 0;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    template.heapID = HEAP_ID_25;
    for (i = 0; i < 2; i++) {
        template.position.x = sTrainerCardButtonSpritePositions[i].x *FX32_ONE;
        template.position.y = sTrainerCardButtonSpritePositions[i].y *FX32_ONE + FX32_CONST(224);
        sprites->buttonSprites[i] = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(sprites->buttonSprites[i], TRUE);
        Sprite_SetAnimCtrlSeq(sprites->buttonSprites[i], sTrainerCardButtonSpriteAnims[i]);
        Sprite_SetDrawPriority(sprites->buttonSprites[i], 2 - i);
        Sprite_SetDrawFlag(sprites->buttonSprites[i], FALSE);
    }
}

#pragma force_active reset

void TrainerCardMain_FreeSprites(TrainerCardMainSprites *sprites) {
    u8 i;
    for (i = 0; i < 16; i++) {
        Heap_Free(sprites->paletteFiles[i]);
    }
    SpriteTransfer_DeleteCharTransferTask(sprites->resourceObjects[0][0]);
    SpriteTransfer_DeleteCharTransferTask(sprites->resourceObjects[1][0]);
    SpriteTransfer_DeletePlttTransferTask(sprites->resourceObjects[0][1]);
    SpriteTransfer_DeletePlttTransferTask(sprites->resourceObjects[1][1]);
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(sprites->resourceManagers[0][i]);
        Destroy2DGfxResObjMan(sprites->resourceManagers[1][i]);
    }
    G2dRenderer_SetSubSurfaceCoords(&sprites->renderer, 0, FX32_CONST(192));
    SpriteList_Delete(sprites->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
}

void TrainerCardMain_SetButtonSpriteAnim(TrainerCardMainSprites *sprites, int button, int sequence, int draw) {
    Sprite_SetAnimCtrlSeq(sprites->buttonSprites[button], sequence);
    Sprite_ResetAnimCtrlState(sprites->buttonSprites[button]);
    Sprite_SetDrawFlag(sprites->buttonSprites[button], draw);
}

void TrainerCardMain_InitObjTransfer(void) {
    ObjCharTransferTemplate template = sTrainerCardObjCharTransferTemplate;
    ObjCharTransfer_Init(&template);
    ObjPlttTransfer_Init(2, HEAP_ID_25);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    thunk_ClearMainOAM(HEAP_ID_25);
    thunk_ClearSubOAM(HEAP_ID_25);
}

void TrainerCardMain_InitUnk33A0Pairs(u8 *pairs) {
    int i;
    for (i = 0; i < 4; i++) {
        pairs[0] = i;
        pairs[1] = 0xff;
        pairs += 2;
    }
}
