#include "battle_arcade_game_board.h"

#include "global.h"

#include "constants/sndseq.h"

#include "frontier/battle_arcade_setup.h"
#include "msgdata/msg.naix"

#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "math_util.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "palette.h"
#include "party.h"
#include "party_menu.h"
#include "poke_overlay.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "screen_fade.h"
#include "sprite_transfer.h"
#include "system.h"
#include "touchscreen.h"
#include "sound_playback.h"
#include "unk_02009D48.h"
#include "sprite_resource.h"
#include "oam_manager.h"
#include "frontier_save_access.h"
#include "unk_02035900.h"
#include "link_synchronization.h"
#include "wireless_status_icon.h"
#include "unk_02096910.h"
#include "vram_transfer_manager.h"

FS_EXTERN_OVERLAY(OVY_80);

static u8 s_0223FA20[2];

static BOOL BattleArcadeGameBoard_Init(GAME_BOARD_WORK *work);
static BOOL BattleArcadeGameBoard_MoveCursor(GAME_BOARD_WORK *work);
static BOOL BattleArcadeGameBoard_StopCursorMulti(GAME_BOARD_WORK *work);
static void BattleArcadeGameBoard_Delete(GAME_BOARD_WORK *work);
static void BattleArcadeGameBoard_InitSub();
static void BattleArcadeGameBoard_InitObjects(GAME_BOARD_WORK *work);
static void BattleArcadeGameBoard_InitBg(GAME_BOARD_WORK *work);
static void BattleArcadeGameBoard_InitSprites(GAME_BOARD_WORK *work);
static void BattleArcadeGameBoard_FreeBg(BgConfig *config);
static void BattleArcade_VBlank(void *work);
static void BattleArcade_SetVramBanks(void);
static void BattleArcadeGameBoard_InitBgLayers(BgConfig *config);
static void BattleArcadeGameBoard_LoadBoardBg(GAME_BOARD_WORK *work, GFBgLayer layer);
static void ov84_0223EB08(GAME_BOARD_WORK *work, GFBgLayer layer);
static void BattleArcadeGameBoard_LoadBgPalette(void);
static void BattleArcadeGameBoard_LoadSubScreenBg(GAME_BOARD_WORK *work, GFBgLayer layer);
static void BattleArcadeGameBoard_SetState(GAME_BOARD_WORK *work, int *state, int a2);
static void BattleArcadeGameBoard_UpdateCursor(GAME_BOARD_WORK *work, int key);
static void BattleArcadeGameBoard_SetCursorPos(GAME_BOARD_WORK *work, u8 cursorPos);
static void BattleArcadeGameBoard_StartCursorSlowdown(GAME_BOARD_WORK *work);
static u8 BattleArcadeGameBoard_WrapPos(u8 area, u8 a2);
static u16 BattleArcade_GetWinStreak(GAME_BOARD_WORK *work);
static BOOL BattleArcadeGameBoard_SendCommand(GAME_BOARD_WORK *work, u16 type, u16 a2);
static void BattleArcadeGameBoard_PackBoardLayout(GAME_BOARD_WORK *work, u16 type);
static void BattleArcadeGameBoard_PackStopPos(GAME_BOARD_WORK *work, u16 type, u16 a2);
static u16 BattleArcade_Random(GAME_BOARD_WORK *work);
static void BattleArcadeGameBoard_PackCursorPos(GAME_BOARD_WORK *work, u16 type);
static void BattleArcadeGameBoard_StopCursor(GAME_BOARD_WORK *work, u8 type);
static void BattleArcadeGameBoard_BuildEventPool(GAME_BOARD_WORK *work);
static void BattleArcadeGameBoard_CountEventColors(GAME_BOARD_WORK *work);
static void BattleArcadeGameBoard_FillPanels(GAME_BOARD_WORK *work);
static u8 BattleArcadeGameBoard_PickEventColor(GAME_BOARD_WORK *, u8);
static void BattleArcadeGameBoard_UpdateItemIcons(GAME_BOARD_WORK *work);
static BOOL BattleArcadeGameBoard_CheckButtonPress(GAME_BOARD_WORK *work);
static void BattleArcadeGameBoard_PressButton(GAME_BOARD_WORK *work);
static void GameBoard_InitSpriteGfx(GAME_BOARD_SUB_3E8 *work, Party *playerParty, Party *opponentParty, u8 type);
static Sprite *GameBoard_CreateSprite(GAME_BOARD_SUB_3E8 *work, u32 chara, u32 pal, u32 cell, u32 anim, u32 prio, int bgPrio, u8 display);
static void GameBoard_FreeSpriteGfx(GAME_BOARD_SUB_3E8 *work);
static void GameBoard_InitCharPlttTransfer(void);
static void GameBoard_LoadEventGraphics(GAME_BOARD_SUB_3E8 *work);
static void GameBoard_LoadItemIconGraphics(GAME_BOARD_SUB_3E8 *work);
static void GameBoard_LoadMonIconGraphics(GAME_BOARD_SUB_3E8 *work, Party *playerParty, Party *opponentParty, u8 type);
static void GameBoard_LoadButtonGraphics(GAME_BOARD_SUB_3E8 *work);
static BATTLE_ARCADE_OBJECT *BattleArcadeObject_Create(GAME_BOARD_SUB_3E8 *work, u32 chara, u32 pal, u32 cell, u32 anim, u16 x, u16 y, u32 priority, int bgPrio, u8 display);
static void *BattleArcadeObj_Delete(BATTLE_ARCADE_OBJECT *obj);
static void BattleArcadeObj_SetVisible(BATTLE_ARCADE_OBJECT *obj, int flag);
static void BattleArcadeObj_SetPos(BATTLE_ARCADE_OBJECT *obj, u16 x, u16 y);
static void BattleArcadeObj_SetAnimation(BATTLE_ARCADE_OBJECT *obj, u32 val);
static void BattleArcadeObj_SetMonIconPalette(BATTLE_ARCADE_OBJECT *obj, Pokemon *mon);
static void BattleArcadeObj_SetAnimActive(BATTLE_ARCADE_OBJECT *obj, int flag);
static void BattleArcadeGameBoard_AddWindows(BgConfig *config, Window *window);
static void BattleArcadeGameBoard_RemoveWindows(Window *window);
static BOOL BattleArcadeGameBoard_EndMulti(GAME_BOARD_WORK *work);
static BOOL BattleArcadeGameBoard_End(GAME_BOARD_WORK *work);

BOOL BattleArcadeGameBoard_InitOverlay(OverlayManager *man, int *state) {
    int i;
    GAME_BOARD_WORK *work;
    GAME_BOARD_ARGS *args;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_80), OVY_LOAD_ASYNC);
    BattleArcadeGameBoard_InitSub();
    Heap_Create(HEAP_ID_3, HEAP_ID_GAME_BOARD, 0x20000);
    work = OverlayManager_CreateAndGetData(man, sizeof(GAME_BOARD_WORK), HEAP_ID_GAME_BOARD);
    memset(work, 0, sizeof(GAME_BOARD_WORK));

    work->bgConfig = BgConfig_Alloc(HEAP_ID_GAME_BOARD);
    work->man = man;
    args = (GAME_BOARD_ARGS *)OverlayManager_GetArgs(man);
    work->saveData = args->saveData;
    work->arcadeSaveData = Save_Frontier_GetArcadeChallenge(work->saveData);
    work->arcadeScoreSaveData = Save_Frontier_GetArcadeFlags(work->saveData);
    work->type = args->type;
    work->round = args->round;
    work->winStreak = args->winStreak;
    work->multiWinStreak = args->multiWinStreak;
    work->unk12 = args->bpGain;
    work->returnWork = &args->returnWork;
    work->options = Save_PlayerData_GetOptionsAddr(work->saveData);
    work->playerParty = args->playerParty;
    work->opponentParty = args->opponentParty;
    work->stopPos = 0xff;
    work->frontierSaveData = Save_Frontier_GetStatic(work->saveData);
    work->weather = args->weather;
    work->cursorSpeed = args->cursorSpeed;
    work->eventId = args->eventId;
    work->eventRandom = args->eventRandom;
    work->autoStopTimer = 900;
    work->randomFlag = args->randomFlag;

    for (i = 0; i < 32; i++) {
        work->eventPool[i] = 32;
    }

    work->boardHeight = 4;
    work->boardWidth = 4;
    work->boardArea = 16;

    work->multiCursorPos = 0;

    *work->eventRandom = BattleArcade_Random(work);
    work->cursorPosStart = LCRandom() % 16;

    BattleArcadeGameBoard_InitObjects(work);

    if (BattleArcade_MultiplayerCheck(work->type) == TRUE) {
        sub_02096910(work);
    }

    *state = 0;
    s_0223FA20[0] = 0;
    s_0223FA20[1] = 0;

    return TRUE;
}

BOOL BattleArcadeGameBoard_Main(OverlayManager *man, int *state) {
    GAME_BOARD_WORK *work = OverlayManager_GetData(man);

    if (work->stopPos != 0xff && *state == 1) {
        BattleArcadeGameBoard_PressButton(work);
        work->stopRequestPos = BattleArcadeGameBoard_WrapPos(work->boardArea, work->stopPos);
        BattleArcadeGameBoard_SetState(work, state, 2);
    }

    switch (*state) {
    case 0:
        if (BattleArcadeGameBoard_Init(work) == TRUE) {
            BattleArcadeGameBoard_SetState(work, state, 1);
        }
        break;
    case 1:
        if (BattleArcadeGameBoard_MoveCursor(work) == TRUE) {
            if (work->stopRequested == 1) {
                BattleArcadeGameBoard_SetState(work, state, 2);
            } else if (BattleArcade_MultiplayerCheck(work->type) == TRUE) {
                BattleArcadeGameBoard_SetState(work, state, 3);
            } else {
                BattleArcadeGameBoard_SetState(work, state, 4);
            }
        }
        break;
    case 2:
        if (BattleArcadeGameBoard_StopCursorMulti(work) == TRUE) {
            BattleArcadeGameBoard_SetState(work, state, 3);
        }
        break;
    case 3:
        if (BattleArcadeGameBoard_EndMulti(work) == TRUE) {
            BattleArcadeGameBoard_SetState(work, state, 4);
        }
        break;
    case 4:
        if (BattleArcadeGameBoard_End(work) == TRUE) {
            return TRUE;
        }
        break;
    }

    work->unk13_1++;
    if (work->unk13_1 >= 4) {
        work->unk13_1 = 0;
        work->unk13_0 ^= 1;
    }

    SpriteList_RenderAndAnimateSprites(work->unk3E8.spriteList);

    return FALSE;
}

BOOL BattleArcadeGameBoard_Exit(OverlayManager *man, int *state) {
    GAME_BOARD_WORK *work = OverlayManager_GetData(man);

    *work->returnWork = work->cursorPos;

    GF_DestroyVramTransferManager();

    BattleArcadeGameBoard_Delete(work);

    OverlayManager_FreeData(man);

    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_GAME_BOARD);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_80));

    return TRUE;
}

static BOOL BattleArcadeGameBoard_Init(GAME_BOARD_WORK *work) {
    switch (work->substate) {
    case 0:
        if (BattleArcade_MultiplayerCheck(work->type) == TRUE) {
            LinkSync_ClearPackets();
            LinkSync_StartSync(199);
            work->substate++;
        } else {
            work->substate++;
        }
        break;
    case 1:
        if (BattleArcade_MultiplayerCheck(work->type) == TRUE) {
            if (LinkSync_IsSynced(199) == TRUE) {
                LinkSync_ClearPackets();
                work->substate++;
            }
        } else {
            work->substate++;
        }
        break;
    case 2:
        BattleArcadeGameBoard_BuildEventPool(work);
        work->substate++;
        break;
    case 3:
        BattleArcadeGameBoard_CountEventColors(work);
        work->substate++;
        break;
    case 4:
        BattleArcadeGameBoard_FillPanels(work);
        work->substate++;
        break;
    case 5:
        if (BattleArcade_MultiplayerCheck(work->type) == TRUE) {
            LinkSync_ClearPackets();
            LinkSync_StartSync(201);
            work->substate++;
        } else {
            work->substate++;
        }
        break;
    case 6:
        if (BattleArcade_MultiplayerCheck(work->type) == TRUE) {
            if (LinkSync_IsSynced(201) == TRUE) {
                LinkSync_ClearPackets();
                work->substate++;
            }
        } else {
            work->substate++;
        }
        break;
    case 7:
        if (BattleArcade_MultiplayerCheck(work->type) == TRUE) {
            if (BattleArcadeGameBoard_SendCommand(work, 30, 0) == TRUE) {
                work->substate++;
            }
        } else {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 3, HEAP_ID_GAME_BOARD);
            work->substate++;
        }
        break;
    case 8:
        if (BattleArcade_MultiplayerCheck(work->type) == TRUE) {
            if (work->linkMessageCount >= 2) {
                work->linkMessageCount = 0;
                BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 3, HEAP_ID_GAME_BOARD);
                work->substate++;
            }
        } else {
            work->substate++;
        }
        break;
    case 9:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static BOOL BattleArcadeGameBoard_MoveCursor(GAME_BOARD_WORK *work) {
    int i;
    switch (work->substate) {
    case 0:
        work->wait = 0;
        work->cursorPos = work->cursorPosStart;
        BattleArcadeGameBoard_SetCursorPos(work, work->cursorPos);
        work->substate = 1;
        break;
    case 1:
        work->wait++;
        if (work->wait >= 10) {
            work->wait = 0;
            work->substate = 2;
        }
        break;
    case 2:
        for (i = 0; i < 16; i++) {
            if (work->panel[i] != NULL) {
                BattleArcadeObj_SetAnimation(work->panel[i], 32);
                BattleArcadeObj_SetVisible(work->panel[i], 1);
            }
        }
        ov84_0223EB08(work, GF_BG_LYR_MAIN_3);
        PlaySE(SEQ_SE_DP_WIN_OPEN2);
        work->wait = 24;
        work->substate = 3;
        break;
    case 3:
        work->wait--;
        if (work->wait > 0) {
            break;
        }
        for (i = 0; i < 16; i++) {
            if (work->panel[i] != NULL) {
                BattleArcadeObj_SetAnimation(work->panel[i], 33);
            }
        }
        PlaySE(SEQ_SE_DP_WIN_OPEN2);
        work->wait = 24;
        work->substate = 4;
        break;
    case 4:
        work->wait--;
        if (work->wait > 0) {
            break;
        }
        for (i = 0; i < 16; i++) {
            if (work->panel[i] != NULL) {
                BattleArcadeObj_SetAnimation(work->panel[i], 34);
            }
        }
        PlaySE(SEQ_SE_DP_WIN_OPEN2);
        work->wait = 24;
        work->substate = 5;
        break;
    case 5:
        work->wait--;
        if (work->wait > 0) {
            break;
        }
        for (i = 0; i < 16; i++) {
            if (work->panel[i] != NULL) {
                BattleArcadeObj_SetAnimation(work->panel[i], work->panelEvents[i]);
            }
        }
        BattleArcadeObj_SetVisible(work->cursor, 1);
        PlaySE(SEQ_SE_DP_UG_020);
        BattleArcadeObj_SetAnimation(work->button, 0);
        work->substate = 6;
        break;
    case 6:
        BattleArcadeGameBoard_UpdateCursor(work, gSystem.newKeys);
        if (Comm_GetLocalNetId() == 0) {
            if (work->autoStopTimer > 0) {
                work->autoStopTimer--;
            }
        }
        if (BattleArcadeGameBoard_CheckButtonPress(work) == TRUE || work->autoStopTimer == 0) {
            BattleArcadeGameBoard_StartCursorSlowdown(work);
            if (BattleArcade_MultiplayerCheck(work->type) == FALSE) {
                work->substate = 7;
            } else {
                work->stopRequested = 1;
                return TRUE;
            }
        }
        break;
    case 7:
        BattleArcadeGameBoard_StopCursor(work, work->cursorPos);
        work->wait = 30;
        work->substate = 8;
        break;
    case 8:
        work->wait--;
        if (work->wait == 0) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static BOOL BattleArcadeGameBoard_StopCursorMulti(GAME_BOARD_WORK *work) {
    switch (work->substate) {
    case 0:
        if (BattleArcadeGameBoard_SendCommand(work, 31, work->stopRequestPos) == TRUE) {
            work->stopRequested = 0;
            work->substate++;
        }
        break;
    case 1:
        work->substate++;
        break;
    case 2:
        if (work->stopPos != 255) {
            work->linkMessageCount = 0;
            BattleArcadeGameBoard_StopCursor(work, work->stopPos);
            work->substate++;
        }
        break;
    case 3:
        LinkSync_ClearPackets();
        LinkSync_StartSync(151);
        work->substate++;
        break;
    case 4:
        if (LinkSync_IsSynced(151) == TRUE) {
            LinkSync_ClearPackets();
            LinkSync_Init(HEAP_ID_GAME_BOARD);
            work->stopPos = 255;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static BOOL BattleArcadeGameBoard_EndMulti(GAME_BOARD_WORK *work) {
    switch (work->substate) {
    case 0:
        work->wait = 15;
        work->substate++;
        break;
    case 1:
        if (work->wait > 0) {
            work->wait--;
        }
        if (work->wait == 0) {
            LinkSync_ClearPackets();
            LinkSync_StartSync(152);
            work->substate++;
        }
        break;
    case 2:
        if (LinkSync_IsSynced(152) == TRUE) {
            LinkSync_ClearPackets();
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static BOOL BattleArcadeGameBoard_End(GAME_BOARD_WORK *work) {
    switch (work->substate) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_GAME_BOARD);
        work->substate++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static void BattleArcadeGameBoard_Delete(GAME_BOARD_WORK *work) {
    u8 max;
    int i;
    if (work->cursor != NULL) {
        BattleArcadeObj_Delete(work->cursor);
    }
    if (work->button != NULL) {
        BattleArcadeObj_Delete(work->button);
    }

    for (i = 0; i < 16; i++) {
        if (work->panel[i] != NULL) {
            BattleArcadeObj_Delete(work->panel[i]);
        }
    }

    max = BattleArcade_GetMonCount(work->type, 1);
    for (i = 0; i < max; i++) {
        if (work->iconA[i] != NULL) {
            BattleArcadeObj_Delete(work->iconA[i]);
        }
        if (work->iconB[i] != NULL) {
            BattleArcadeObj_Delete(work->iconB[i]);
        }
        if (work->itemA[i] != NULL) {
            BattleArcadeObj_Delete(work->itemA[i]);
        }
        if (work->itemB[i] != NULL) {
            BattleArcadeObj_Delete(work->itemB[i]);
        }
    }

    WirelessStatusIcon_Destroy();

    PaletteData_FreeBuffers(work->paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(work->paletteData, PLTTBUF_MAIN_BG);

    PaletteData_Free(work->paletteData);
    work->paletteData = NULL;

    GameBoard_FreeSpriteGfx(&work->unk3E8);

    DestroyMsgData(work->msgData);
    MessageFormat_Delete(work->msgFmt);
    String_Delete(work->unk70);
    String_Delete(work->unk74);

    for (i = 0; i < 2; i++) {
        String_Delete(work->unk78[i]);
    }

    BattleArcadeGameBoard_RemoveWindows(work->window);
    BattleArcadeGameBoard_FreeBg(work->bgConfig);
    NARC_Delete(work->narc);
}

static void BattleArcadeGameBoard_InitSub() {
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
}

static void BattleArcadeGameBoard_InitObjects(GAME_BOARD_WORK *work) {
    u8 max;
    u16 yOffset, yOffset2;
    int i;

    work->narc = NARC_New(NARC_a_1_8_3, HEAP_ID_GAME_BOARD);
    BattleArcadeGameBoard_InitBg(work);
    BattleArcadeGameBoard_InitSprites(work);

    work->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0425_bin, HEAP_ID_GAME_BOARD);
    work->msgFmt = MessageFormat_New(HEAP_ID_GAME_BOARD);
    work->unk70 = String_New(600, HEAP_ID_GAME_BOARD);
    work->unk74 = String_New(600, HEAP_ID_GAME_BOARD);

    for (i = 0; i < 2; i++) {
        work->unk78[i] = String_New(32, HEAP_ID_GAME_BOARD);
    }

    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_GAME_BOARD);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_GAME_BOARD);
    BattleArcadeGameBoard_AddWindows(work->bgConfig, work->window);
    work->cursor = BattleArcadeObject_Create(&work->unk3E8, 1, 1, 1, 35, 68, 36, 0, 2, 0);
    BattleArcadeObj_SetVisible(work->cursor, 0);

    for (i = 0; i < 16; i++) {
        work->panel[i] = BattleArcadeObject_Create(&work->unk3E8, 1, 1, 1, 32, 68 + 40 * (i % 4), 36 + 40 * (i / 4), 1, 2, 0);
        BattleArcadeObj_SetVisible(work->panel[i], 0);
    }

    max = BattleArcade_GetMonCount(work->type, 1);

    if (BattleArcade_MultiplayerCheck(work->type) == FALSE) {
        yOffset = 46;
        yOffset2 = 50;
    } else {
        yOffset = 22;
        yOffset2 = 26;
    }

    for (i = 0; i < max; i++) {
        work->itemA[i] = BattleArcadeObject_Create(&work->unk3E8, 2, 2, 2, 0, 24, yOffset2 + i * 40, 0, 2, 0);
        work->itemB[i] = BattleArcadeObject_Create(&work->unk3E8, 2, 2, 2, 0, 246, yOffset2 + i * 40, 0, 2, 0);
        work->iconA[i] = BattleArcadeObject_Create(&work->unk3E8, 3 + i, 3, 3, 1, 16, yOffset + i * 40, 1, 2, 0);
        work->iconB[i] = BattleArcadeObject_Create(&work->unk3E8, 7 + i, 3, 3, 1, 238, yOffset + i * 40, 1, 2, 0);
        BattleArcadeObj_SetMonIconPalette(work->iconA[i], Party_GetMonByIndex(work->playerParty, i));
        BattleArcadeObj_SetMonIconPalette(work->iconB[i], Party_GetMonByIndex(work->opponentParty, i));
        BattleArcadeObj_SetAnimActive(work->iconA[i], 0);
        BattleArcadeObj_SetAnimActive(work->iconB[i], 0);
    }

    BattleArcadeGameBoard_UpdateItemIcons(work);

    work->button = BattleArcadeObject_Create(&work->unk3E8, 0, 0, 0, 2, 128, 96, 0, 0, 1);

    if (sub_02037474()) {
        G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
        G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
        WirelessStatusIcon_Create();
    }

    Main_SetVBlankIntrCB(BattleArcade_VBlank, work);
}

static void BattleArcadeGameBoard_InitBg(GAME_BOARD_WORK *work) {
    BattleArcade_SetVramBanks();
    BattleArcadeGameBoard_InitBgLayers(work->bgConfig);

    work->paletteData = PaletteData_Init(HEAP_ID_GAME_BOARD);

    PaletteData_AllocBuffers(work->paletteData, PLTTBUF_MAIN_OBJ, 0x200, HEAP_ID_GAME_BOARD);
    PaletteData_AllocBuffers(work->paletteData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_GAME_BOARD);

    BattleArcadeGameBoard_LoadBoardBg(work, GF_BG_LYR_MAIN_3);
    BattleArcadeGameBoard_LoadBgPalette();
    BattleArcadeGameBoard_LoadSubScreenBg(work, GF_BG_LYR_SUB_0);
}

static void BattleArcadeGameBoard_InitSprites(GAME_BOARD_WORK *work) {
    GameBoard_InitSpriteGfx(&work->unk3E8, work->playerParty, work->opponentParty, BattleArcade_MultiplayerCheck(work->type));
}

static void BattleArcadeGameBoard_FreeBg(BgConfig *config) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_ALL, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_ALL, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(config, 3);
    FreeBgTilemapBuffer(config, 1);
    FreeBgTilemapBuffer(config, 4);
    Heap_Free(config);
}

static void BattleArcade_VBlank(void *_work) {
    GAME_BOARD_WORK *work = _work;

    if (work->paletteData != NULL) {
        PaletteData_PushTransparentBuffers(work->paletteData);
    }

    DoScheduledBgGpuUpdates(work->bgConfig);
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

extern GraphicsBanks gBattleArcadeGameBoardGraphicsBanks;

static void BattleArcade_SetVramBanks() {
    GraphicsBanks config = gBattleArcadeGameBoardGraphicsBanks;
    GfGfx_SetBanks(&config);
}

extern GraphicsModes gBattleArcadeGameBoardGraphicsModes;
extern BgTemplate gBattleArcadeGameBoardBg1Template;
extern BgTemplate gBattleArcadeGameBoardBg3Template;
extern BgTemplate gBattleArcadeGameBoardSubBg0Template;

static void BattleArcadeGameBoard_InitBgLayers(BgConfig *config) {
    GraphicsModes bgModeSet = gBattleArcadeGameBoardGraphicsModes;
    SetBothScreensModesAndDisable(&bgModeSet);

    BgTemplate template = gBattleArcadeGameBoardBg1Template;
    InitBgFromTemplate(config, 1, &template, 0);
    BG_ClearCharDataRange(1, 32, 0, HEAP_ID_GAME_BOARD);
    BgClearTilemapBufferAndCommit(config, 1);

    BgTemplate template2 = gBattleArcadeGameBoardBg3Template;
    InitBgFromTemplate(config, 3, &template2, 0);
    BgClearTilemapBufferAndCommit(config, 3);

    BgTemplate template3 = gBattleArcadeGameBoardSubBg0Template;
    InitBgFromTemplate(config, 4, &template3, 0);
    BgClearTilemapBufferAndCommit(config, 4);

    G2_SetBG0Priority(0);

    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
}

static void BattleArcadeGameBoard_LoadBoardBg(GAME_BOARD_WORK *work, GFBgLayer layer) {
    u32 var;

    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 124, work->bgConfig, layer, 0, 0, 1, HEAP_ID_GAME_BOARD);

    if (BattleArcade_MultiplayerCheck(work->type) == FALSE) {
        var = 120;
    } else {
        var = 122;
    }

    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, var, work->bgConfig, layer, 0, 0, 1, HEAP_ID_GAME_BOARD);
}

static void ov84_0223EB08(GAME_BOARD_WORK *work, GFBgLayer layer) {
    u32 var;

    if (BattleArcade_MultiplayerCheck(work->type) == FALSE) {
        var = 121;
    } else {
        var = 123;
    }

    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, var, work->bgConfig, layer, 0, 0, 1, HEAP_ID_GAME_BOARD);
}

static void BattleArcadeGameBoard_LoadBgPalette(void) {
    void *buffer;
    NNSG2dPaletteData *dat;

    buffer = GfGfxLoader_GetPlttData(NARC_a_1_8_3, 186, &dat, HEAP_ID_GAME_BOARD);

    DC_FlushRange(dat->pRawData, 224);
    GX_LoadBGPltt(dat->pRawData, 0, 224);

    Heap_Free(buffer);
}

static void BattleArcadeGameBoard_LoadSubScreenBg(GAME_BOARD_WORK *work, GFBgLayer layer) {
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 126, work->bgConfig, layer, 0, 0, TRUE, HEAP_ID_GAME_BOARD);
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 187, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40, HEAP_ID_GAME_BOARD);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 125, work->bgConfig, layer, 0, 0, TRUE, HEAP_ID_GAME_BOARD);
}

static void BattleArcadeGameBoard_SetState(GAME_BOARD_WORK *work, int *state, int a2) {
    work->substate = 0;
    *state = a2;
}

extern u8 gBattleArcadeCursorDelays[8][2];

static void BattleArcadeGameBoard_UpdateCursor(GAME_BOARD_WORK *work, int key) {
    u8 level = *work->cursorSpeed;
    work->cursorTimer++;

    if (work->cursorTimer >= gBattleArcadeCursorDelays[level][0]) {
        work->cursorTimer = 0;
        if (work->randomFlag == 1) {
            work->cursorPos = LCRandom() % 16;
        } else {
            work->cursorPos++;
        }

        PlaySE(SEQ_SE_DP_BUTTON3);

        if (work->cursorPos >= work->boardArea) {
            work->cursorPos = 0;
        }

        BattleArcadeGameBoard_SetCursorPos(work, work->cursorPos);
    }
}

static void BattleArcadeGameBoard_SetCursorPos(GAME_BOARD_WORK *work, u8 cursorPos) {
    BattleArcadeObj_SetPos(work->cursor, 68 + 40 * (cursorPos % 4), 36 + 40 * (cursorPos / 4));
}

static void BattleArcadeGameBoard_StartCursorSlowdown(GAME_BOARD_WORK *work) {
    work->stopRequestPos = work->cursorPos;
}

static u8 BattleArcadeGameBoard_WrapPos(u8 area, u8 a2) {
    if (a2 >= area) {
        return a2 - area;
    }
    return a2;
}

static u16 BattleArcade_GetWinStreak(GAME_BOARD_WORK *work) {
    u16 winStreak = work->winStreak;

    if (BattleArcade_MultiplayerCheck(work->type) == TRUE && work->multiWinStreak > work->winStreak) {
        winStreak = work->multiWinStreak;
    }

    return winStreak;
}

static BOOL BattleArcadeGameBoard_SendCommand(GAME_BOARD_WORK *work, u16 type, u16 a2) {
    int unkA;

    switch (type) {
    case 30:
        unkA = 71;
        BattleArcadeGameBoard_PackBoardLayout(work, type);
        break;
    case 31:
        unkA = 72;
        BattleArcadeGameBoard_PackStopPos(work, type, a2);
        break;
    case 32:
        unkA = 73;
        BattleArcadeGameBoard_PackCursorPos(work, type);
        break;
    }

    return Comm_SendCommand(unkA, work->sendBuffer, 40) == TRUE;
}

static void BattleArcadeGameBoard_PackBoardLayout(GAME_BOARD_WORK *work, u16 type) {
    int i, offset;

    Save_PlayerData_GetProfile(work->saveData);

    work->sendBuffer[0] = type;

    offset = 2;

    for (i = 0; i < 16; i++) {
        work->sendBuffer[offset + i] = work->panelEvents[i];
    }

    offset += 16;

    work->sendBuffer[offset] = work->cursorPosStart;
}

void BattleArcadeGameBoard_ReceiveBoardLayout(int a0, int size, void *data, void *_work) {
    int i, offset;
    GAME_BOARD_WORK *work = _work;
    const u16 *recv = data;

    work->linkMessageCount++;

    if (Comm_GetLocalNetId() == a0) {
        return;
    }

    if (Comm_GetLocalNetId() != 0) {
        offset = 2;

        for (i = 0; i < 16; i++) {
            work->panelEvents[i] = (u8)recv[offset + i];
        }

        offset += 16;

        work->cursorPosStart = recv[offset];
    }
}

static void BattleArcadeGameBoard_PackStopPos(GAME_BOARD_WORK *work, u16 type, u16 a2) {
    int offset = 0;

    work->sendBuffer[offset] = type;
    offset++;

    work->sendBuffer[offset] = a2;
    offset++;

    if (Comm_GetLocalNetId() == 0 && work->stopPos == 255) {
        work->stopPos = a2;
    }

    work->sendBuffer[offset] = work->stopPos;
    offset++;

    work->sendBuffer[offset] = *work->eventRandom;
}

static u16 BattleArcade_Random(GAME_BOARD_WORK *work) {
    return LCRandom();
}

void BattleArcadeGameBoard_ReceiveStopPos(int a0, int size, void *data, void *_work) {
    GAME_BOARD_WORK *work = _work;

    u16 *recv = data;

    work->linkMessageCount++;

    if (Comm_GetLocalNetId() == a0) {
        return;
    }

    work->multiSelectPos = recv[1];

    if (Comm_GetLocalNetId() == 0) {
        if (work->stopPos != 255) {
            work->multiSelectPos = 0;
        } else {
            work->stopPos = work->multiSelectPos + work->boardArea;
        }
    } else {
        work->stopPos = recv[2];
        *work->eventRandom = recv[3];
    }
}

static void BattleArcadeGameBoard_PackCursorPos(GAME_BOARD_WORK *work, u16 type) {
    work->sendBuffer[0] = type;
    work->sendBuffer[1] = work->cursorPos;
}

void BattleArcadeGameBoard_ReceiveCursorPos(int a0, int size, void *data, void *_work) {
    GAME_BOARD_WORK *work = _work;
    const u16 *recv = data;

    if (Comm_GetLocalNetId() == a0) {
        return;
    }

    work->multiCursorPos = (u8)recv[1];
}

static void BattleArcadeGameBoard_StopCursor(GAME_BOARD_WORK *work, u8 type) {
    int i;

    u8 offset = work->boardArea;
    u8 pos = BattleArcadeGameBoard_WrapPos(offset, type);
    u8 panelId = work->panelEvents[pos];

    *work->eventId = panelId;

    for (i = 0; i < 16; i++) {
        if (work->panel[i] != NULL) {
            BattleArcadeObj_SetAnimation(work->panel[i], panelId);
        }
    }

    BattleArcadeGameBoard_SetCursorPos(work, pos);
    PlaySE(SEQ_SE_DP_PIRORIRO2);
}

typedef struct _0223F99C {
    u8 flag0 : 1;
    u8 flag1 : 1;
    u8 flag2 : 1;
    u8 flag3 : 1;
    u8 flag4 : 1;
    u8 flag5 : 1;
    u8 flag6 : 1;
    u8 flag7 : 1;
} STRUCT_0223F99C;

extern const STRUCT_0223F99C gBattleArcadeEventRoundFlags[32];
extern const STRUCT_0223F99C gBattleArcadeEventStreakFlags[7];

static void BattleArcadeGameBoard_BuildEventPool(GAME_BOARD_WORK *work) {
    int i;
    u16 winStreak, var;
    u8 flag, challenge;

    for (i = 0; i < NELEMS(gBattleArcadeEventRoundFlags); i++) {
        switch (work->round) {
        case 0:
            flag = gBattleArcadeEventRoundFlags[i].flag0;
            break;
        case 1:
            flag = gBattleArcadeEventRoundFlags[i].flag1;
            break;
        case 2:
            flag = gBattleArcadeEventRoundFlags[i].flag2;
            break;
        case 3:
            flag = gBattleArcadeEventRoundFlags[i].flag3;
            break;
        case 4:
            flag = gBattleArcadeEventRoundFlags[i].flag4;
            break;
        case 5:
            flag = gBattleArcadeEventRoundFlags[i].flag5;
            break;
        default:
            flag = gBattleArcadeEventRoundFlags[i].flag6;
            break;
        }

        if (flag == 1) {
            challenge = 0xff;

            switch (i) {
            case 27:
                challenge = 0;
                break;
            case 24:
                challenge = 1;
                break;
            case 25:
                challenge = 2;
                break;
            case 26:
                challenge = 3;
                break;
            case 28:
                challenge = 4;
                break;
            case 29:
                challenge = 5;
                break;
            case 31:
                challenge = 6;
                break;
            }

            winStreak = BattleArcade_GetWinStreak(work);

            var = winStreak % 7;

            if (winStreak >= 9999) {
                var = 6;
            }

            if (challenge != 255) {
                switch (var) {
                case 0:
                    flag = gBattleArcadeEventStreakFlags[challenge].flag0;
                    break;
                case 1:
                    flag = gBattleArcadeEventStreakFlags[challenge].flag1;
                    break;
                case 2:
                    flag = gBattleArcadeEventStreakFlags[challenge].flag2;
                    break;
                case 3:
                    flag = gBattleArcadeEventStreakFlags[challenge].flag3;
                    break;
                case 4:
                    flag = gBattleArcadeEventStreakFlags[challenge].flag4;
                    break;
                case 5:
                    flag = gBattleArcadeEventStreakFlags[challenge].flag5;
                    break;
                case 6:
                case 7:
                    flag = gBattleArcadeEventStreakFlags[challenge].flag6;
                    break;
                default:
                    GF_ASSERT(FALSE);
                    flag = 1;
                    break;
                }
            } else {
                flag = 1;
            }

            if (flag == 1) {
                work->eventPool[work->eventPoolCount] = i;
                work->eventPoolCount++;
            }
        }
    }
}

extern const u8 _0223F900[4];

static void BattleArcadeGameBoard_CountEventColors(GAME_BOARD_WORK *work) {
    int i;

    for (i = 0; i < NELEMS(_0223F900); i++) {
        if (work->unk12 >= _0223F900[i]) {
            break;
        }
    }

    work->colorOddsRow = i;

    for (i = 0; i < work->eventPoolCount; i++) {
        if (BattleArcade_GetEventColor(work->eventPool[i]) == 0) {
            work->eventColorCounts[0]++;
        } else if (BattleArcade_GetEventColor(work->eventPool[i]) == 1) {
            work->eventColorCounts[1]++;
        } else if (BattleArcade_GetEventColor(work->eventPool[i]) == 2) {
            work->eventColorCounts[2]++;
        } else if (BattleArcade_GetEventColor(work->eventPool[i]) == 3) {
            work->eventColorCounts[3]++;
        }
    }
}

static void BattleArcadeGameBoard_FillPanels(GAME_BOARD_WORK *work) {
    int cnt, i, offset, range;
    u16 rand0;
    u8 rand, color;

    for (i = 0; i < 16; i++) {
        color = BattleArcadeGameBoard_PickEventColor(work, work->colorOddsRow);
        offset = 0;
        range = work->eventColorCounts[color];

        if (color == 0) {
            offset = 0;
        } else if (color == 1) {
            offset += work->eventColorCounts[0];
        } else if (color == 2) {
            offset += work->eventColorCounts[0];
            offset += work->eventColorCounts[1];
        } else if (color == 3) {
            offset += work->eventColorCounts[0];
            offset += work->eventColorCounts[1];
            offset += work->eventColorCounts[2];
        }

        rand0 = LCRandom() % range;
        rand0 += offset;
        rand = rand0;

        cnt = 0;

        while (TRUE) {
            if (cnt >= 50) {
                GF_ASSERT(FALSE);
                work->panelEvents[i] = 0;
                cnt = 0;
                break;
            }

            if (BattleArcade_GetEventColor(work->eventPool[rand]) == color) {
                work->panelEvents[i] = work->eventPool[rand];
                break;
            }

            rand++;
            cnt++;

            if (rand >= work->eventPoolCount) {
                rand = 0;
            }

            if (rand == rand0) {
                color++;
                if (color >= 4) {
                    color = 0;
                }
            }
        }
    }
}

extern const u8 gBattleArcadeEventColorOdds[4][4];

static u8 BattleArcadeGameBoard_PickEventColor(GAME_BOARD_WORK *work, u8 a1) {
    u8 cnt, i;
    u16 rand;

    cnt = 0;
    rand = LCRandom() % 100;

    for (i = 0; i < 4; i++) {
        cnt += gBattleArcadeEventColorOdds[a1][i];
        if (rand < cnt) {
            break;
        }
    }

    if (i >= 4) {
        GF_ASSERT(FALSE);
        i = 0;
    }

    return i;
}

static void BattleArcadeGameBoard_UpdateItemIcons(GAME_BOARD_WORK *work) {
    u8 partyCount, opponentPartyCount, i;
    Pokemon *mon;

    partyCount = BattleArcade_GetMonCount(work->type, 1);
    opponentPartyCount = BattleArcade_GetOpponentMonCount(work->type, 1);

    for (i = 0; i < partyCount; i++) {
        mon = Party_GetMonByIndex(work->playerParty, i);
        if (GetMonData(mon, 6, NULL) == 0) {
            BattleArcadeObj_SetVisible(work->itemA[i], 0);
        } else {
            BattleArcadeObj_SetVisible(work->itemA[i], 1);
        }
    }

    for (i = 0; i < opponentPartyCount; i++) {
        mon = Party_GetMonByIndex(work->opponentParty, i);
        if (GetMonData(mon, 6, NULL) == 0) {
            BattleArcadeObj_SetVisible(work->itemB[i], 0);
        } else {
            BattleArcadeObj_SetVisible(work->itemB[i], 1);
        }
    }
}

extern TouchscreenHitbox gBattleArcadeGameBoardButtonHitbox[2];

static BOOL BattleArcadeGameBoard_CheckButtonPress(GAME_BOARD_WORK *work) {
    int var = TouchscreenHitbox_FindHitboxAtTouchNew(gBattleArcadeGameBoardButtonHitbox);

    if (var == 0) {
        BattleArcadeGameBoard_PressButton(work);
        return TRUE;
    }

    return FALSE;
}

static void BattleArcadeGameBoard_PressButton(GAME_BOARD_WORK *work) {
    StopSE(SEQ_SE_DP_SELECT, 0);
    PlaySE(SEQ_SE_DP_BUTTON9);
    BattleArcadeObj_SetAnimation(work->button, 1);
}

extern u8 gBattleArcadeGameBoardResObjCounts[4];

static void GameBoard_InitSpriteGfx(GAME_BOARD_SUB_3E8 *work, Party *playerParty, Party *opponentParty, u8 type) {
    int i;

    GF_CreateVramTransferManager(32, HEAP_ID_GAME_BOARD);
    GameBoard_InitCharPlttTransfer();
    NNS_G2dInitOamManagerModule();

    OamManager_Create(0, 128, 0, 32, 0, 128, 0, 32, HEAP_ID_GAME_BOARD);

    work->spriteList = G2dRenderer_Init(34, &work->render, HEAP_ID_GAME_BOARD);

    for (i = 0; i < 4; i++) {
        work->resourceMan[i] = Create2DGfxResObjMan(gBattleArcadeGameBoardResObjCounts[i], (GfGfxResType)i, HEAP_ID_GAME_BOARD);
    }

    GameBoard_LoadButtonGraphics(work);
    GameBoard_LoadEventGraphics(work);
    GameBoard_LoadItemIconGraphics(work);
    GameBoard_LoadMonIconGraphics(work, playerParty, opponentParty, type);

    for (i = 0; i < 11; i++) {
        SpriteTransfer_CreateCharTransferTask(work->resourceObj[i][GF_GFX_RES_TYPE_CHAR]);
    }

    for (i = 0; i < 4; i++) {
        SpriteTransfer_CreateExtPlttTransferTask(work->resourceObj[i][GF_GFX_RES_TYPE_PLTT]);
    }

    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static Sprite *GameBoard_CreateSprite(GAME_BOARD_SUB_3E8 *work, u32 chara, u32 pal, u32 cell, u32 anim, u32 prio, int bgPrio, u8 display) {
    struct SpriteResourcesHeader resourceHeader;
    Sprite *sprite;

    CreateSpriteResourcesHeader(&resourceHeader, chara, pal, cell, cell, -1, -1, 0, bgPrio, work->resourceMan[0], work->resourceMan[1], work->resourceMan[2], work->resourceMan[3], NULL, NULL);

    {
        SpriteTemplate template;
        template.spriteList = work->spriteList;
        template.header = &resourceHeader;
        template.position.x = 0;
        template.position.y = 0;
        template.position.z = 0;
        template.scale.x = 1 * FX32_ONE;
        template.scale.y = 1 * FX32_ONE;
        template.scale.z = 1 * FX32_ONE;
        template.rotation = 0;
        template.drawPriority = prio;
        template.heapID = HEAP_ID_GAME_BOARD;

        if (display == 0) {
            template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
        } else {
            template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
            template.position.y += (GX_LCD_SIZE_Y * FX32_ONE);
        }

        sprite = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(sprite, 1);
        Sprite_SetAnimSpeed(sprite, 1 * FX32_ONE);
        Sprite_SetAnimCtrlSeq(sprite, anim);
    }

    return sprite;
}

static void GameBoard_FreeSpriteGfx(GAME_BOARD_SUB_3E8 *work) {
    u8 i;

    for (i = 0; i < 11; i++) {
        SpriteTransfer_DeleteCharTransferTask(work->resourceObj[i][GF_GFX_RES_TYPE_CHAR]);
    }

    for (i = 0; i < 4; i++) {
        SpriteTransfer_DeletePlttTransferTask(work->resourceObj[i][GF_GFX_RES_TYPE_PLTT]);
    }

    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(work->resourceMan[i]);
    }

    SpriteList_Delete(work->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}

extern ObjCharTransferTemplate gBattleArcadeGameBoardCharTransferTemplate;

static void GameBoard_InitCharPlttTransfer(void) {
    ObjCharTransferTemplate var = gBattleArcadeGameBoardCharTransferTemplate;
    ObjCharTransfer_InitEx(&var, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K);
    ObjPlttTransfer_Init(14, HEAP_ID_GAME_BOARD);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

static void GameBoard_LoadEventGraphics(GAME_BOARD_SUB_3E8 *work) {
    work->resourceObj[1][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(work->resourceMan[0], NARC_a_1_8_4, 18, TRUE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_GAME_BOARD);
    work->resourceObj[1][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(work->resourceMan[1], NARC_a_1_8_4, 56, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 8, HEAP_ID_GAME_BOARD);
    work->resourceObj[1][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(work->resourceMan[2], NARC_a_1_8_4, 20, TRUE, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_GAME_BOARD);
    work->resourceObj[1][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(work->resourceMan[3], NARC_a_1_8_4, 19, TRUE, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_GAME_BOARD);
}

static void GameBoard_LoadItemIconGraphics(GAME_BOARD_SUB_3E8 *work) {
    NARC *narc = NARC_New(NARC_graphic_plist_gra, HEAP_ID_GAME_BOARD);

    work->resourceObj[2][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(work->resourceMan[0], narc, sub_0207CA9C(), FALSE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_GAME_BOARD);
    work->resourceObj[2][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(work->resourceMan[1], NARC_graphic_plist_gra, sub_0207CAA0(), FALSE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 3, HEAP_ID_GAME_BOARD);
    work->resourceObj[2][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(work->resourceMan[2], narc, sub_0207CAA4(), FALSE, 2, GF_GFX_RES_TYPE_CELL, HEAP_ID_GAME_BOARD);
    work->resourceObj[2][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(work->resourceMan[3], narc, sub_0207CAA8(), FALSE, 2, GF_GFX_RES_TYPE_ANIM, HEAP_ID_GAME_BOARD);

    NARC_Delete(narc);
}

static void GameBoard_LoadMonIconGraphics(GAME_BOARD_SUB_3E8 *work, Party *playerParty, Party *opponentParty, u8 type) {
    u32 i;
    Pokemon *playerMon;
    Pokemon *opponentMon;
    NARC *narc = NARC_New(NARC_poketool_icongra_poke_icon, HEAP_ID_GAME_BOARD);

    work->resourceObj[3][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(work->resourceMan[1], NARC_poketool_icongra_poke_icon, GetMonIconPlttNaix(), FALSE, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 3, HEAP_ID_GAME_BOARD);
    work->resourceObj[3][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(work->resourceMan[2], narc, GetMonIconCell32KNaix(), FALSE, 3, GF_GFX_RES_TYPE_CELL, HEAP_ID_GAME_BOARD);
    work->resourceObj[3][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(work->resourceMan[3], narc, GetMonIconAnim32KNaix(), FALSE, 3, GF_GFX_RES_TYPE_ANIM, HEAP_ID_GAME_BOARD);

    for (i = 0; i < 4; i++) {
        if (i == 3) {
            if (type == FALSE) {
                playerMon = Party_GetMonByIndex(playerParty, 0);
                opponentMon = Party_GetMonByIndex(opponentParty, 0);
            } else {
                playerMon = Party_GetMonByIndex(playerParty, i);
                opponentMon = Party_GetMonByIndex(opponentParty, i);
            }
        } else {
            playerMon = Party_GetMonByIndex(playerParty, i);
            opponentMon = Party_GetMonByIndex(opponentParty, i);
        }

        work->resourceObj[3 + i][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(work->resourceMan[0], narc, Pokemon_GetIconNaix(playerMon), FALSE, 3 + i, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_GAME_BOARD);
        work->resourceObj[7 + i][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(work->resourceMan[0], narc, Pokemon_GetIconNaix(opponentMon), FALSE, 7 + i, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_GAME_BOARD);
    }

    NARC_Delete(narc);
}

static void GameBoard_LoadButtonGraphics(GAME_BOARD_SUB_3E8 *work) {
    work->resourceObj[0][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(work->resourceMan[0], NARC_a_1_8_4, 21, TRUE, 0, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_GAME_BOARD);
    work->resourceObj[0][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(work->resourceMan[1], NARC_a_1_8_4, 57, FALSE, 0, NNS_G2D_VRAM_TYPE_2DSUB, 2, HEAP_ID_GAME_BOARD);
    work->resourceObj[0][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(work->resourceMan[2], NARC_a_1_8_4, 23, TRUE, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_GAME_BOARD);
    work->resourceObj[0][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(work->resourceMan[3], NARC_a_1_8_4, 22, TRUE, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_GAME_BOARD);
}

static BATTLE_ARCADE_OBJECT *BattleArcadeObject_Create(GAME_BOARD_SUB_3E8 *work, u32 chara, u32 pal, u32 cell, u32 anim, u16 x, u16 y, u32 priority, int bgPrio, u8 display) {
    BATTLE_ARCADE_OBJECT *obj;
    VecFx32 vec;
    u32 i;

    obj = Heap_Alloc(HEAP_ID_GAME_BOARD, sizeof(BATTLE_ARCADE_OBJECT));

    u8 *temp = (u8 *)obj;
    for (i = sizeof(BATTLE_ARCADE_OBJECT); i; i--) {
        *temp++ = 0;
    }

    obj->sprite = GameBoard_CreateSprite(work, chara, pal, cell, anim, priority, bgPrio, display);

    obj->x0 = x;
    obj->y0 = y;
    obj->display = display;

    vec.x = x * FX32_ONE;
    vec.y = y * FX32_ONE;

    if (display == 1) {
        vec.y += (192 * FX32_ONE);
    }

    Sprite_SetMatrix(obj->sprite, &vec);

    return obj;
}

static void *BattleArcadeObj_Delete(BATTLE_ARCADE_OBJECT *obj) {
    Sprite_Delete(obj->sprite);
    Heap_Free(obj);
    return NULL;
}

static void BattleArcadeObj_SetVisible(BATTLE_ARCADE_OBJECT *obj, int flag) {
    Sprite_SetDrawFlag(obj->sprite, flag);
}

static void BattleArcadeObj_SetPos(BATTLE_ARCADE_OBJECT *obj, u16 x, u16 y) {
    VecFx32 vec;
    vec = *Sprite_GetMatrixPtr(obj->sprite);
    vec.x = x * FX32_ONE;
    vec.y = y * FX32_ONE;

    if (obj->display == 1) {
        vec.y += (192 * FX32_ONE);
    }

    Sprite_SetMatrix(obj->sprite, &vec);
}

static void BattleArcadeObj_SetAnimation(BATTLE_ARCADE_OBJECT *obj, u32 val) {
    Sprite_SetAnimSpeed(obj->sprite, FX32_ONE);
    Sprite_TryChangeAnimSeq(obj->sprite, val);
}

static void BattleArcadeObj_SetMonIconPalette(BATTLE_ARCADE_OBJECT *obj, Pokemon *mon) {
    Sprite_SetPalOffsetRespectVramOffset(obj->sprite, Pokemon_GetIconPalette(mon));
}

static void BattleArcadeObj_SetAnimActive(BATTLE_ARCADE_OBJECT *obj, int flag) {
    Sprite_SetAnimActiveFlag(obj->sprite, flag);
}

extern WindowTemplate gBattleArcadeGameBoardWindowTemplates[2];

static void BattleArcadeGameBoard_AddWindows(BgConfig *config, Window *window) {
    u8 i;
    WindowTemplate *dat = gBattleArcadeGameBoardWindowTemplates;

    for (i = 0; i < 2; i++) {
        AddWindow(config, &window[i], &dat[i]);
        FillWindowPixelBuffer(&window[i], 0);
    }
}

static void BattleArcadeGameBoard_RemoveWindows(Window *window) {
    u16 i;

    for (i = 0; i < 2; i++) {
        RemoveWindow(&window[i]);
    }
}
