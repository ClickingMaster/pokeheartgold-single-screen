#include "global.h"

#include "constants/game_stats.h"
#include "constants/sndseq.h"

#include "battle/battle_setup.h"
#include "frontier/frontier.h"
#include "frontier/frontier_brain_intro.h"
#include "frontier/frontier_map.h"
#include "frontier/frontier_script_context.h"
#include "frontier/frontier_system.h"
#include "frontier/frontier_transition.h"
#include "frontier/frontier_particles.h"
#include "msgdata/msg.naix"

#include "battle_record.h"
#include "battle_recording.h"
#include "bg_window.h"
#include "brightness.h"
#include "communication_player_state.h"
#include "friend_roster.h"
#include "frontier_data.h"
#include "game_stats.h"
#include "heap.h"
#include "launch_application.h"
#include "link_synchronization.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "naming_screen.h"
#include "palette.h"
#include "party.h"
#include "player_avatar.h"
#include "player_data.h"
#include "render_window.h"
#include "save.h"
#include "save_arrays.h"
#include "save_vars_flags.h"
#include "screen_fade.h"
#include "sound_system.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "comm_wireless.h"
#include "communication_core.h"
#include "comm_state.h"
#include "communication_club_link.h"
#include "use_item_on_mon.h"
#include "wifi_connection.h"

// Work of the screen transitions of FrtCmd_BattleTransition (the frontier app's data while it runs).
typedef struct FrontierRowScrollLine {
    s16 start;
    s16 end;
    s16 speedX;
    s16 speedY;
    s16 x;
    s16 y;
    u32 unk_0C;
} FrontierRowScrollLine;

// Rows of the two affine backgrounds (BG2/BG3) that drift apart (transitions 3 and 4).
typedef struct FrontierRowScroll {
    FrontierRowScrollLine lines[96];
    FrontierHBlankDma *dma; // 0x600
    u32 mode;               // 0x604, 1 or 2
} FrontierRowScroll;

typedef struct FrontierTransitionWork {
    FrontierMap *map;          // 0x00
    u32 state;                 // 0x04
    u32 transition;            // 0x08
    u32 flashDone;             // 0x0C
    u32 finished;              // 0x10
    void *prevData;            // 0x14, the frontier app's data before the transition
    FrontierScanline scanline; // 0x18
    FrontierRowScroll *rows;   // 0x24
    Window *window;            // 0x28
    FrontierWipe *wipe;        // 0x2C
} FrontierTransitionWork;

typedef struct FrontierCameraPos {
    s16 y;
    s16 x;
} FrontierCameraPos;

extern BOOL (*const gFrontierTransitionFuncs[])(FrontierTransitionWork *work);

void *Frontier_GetMap(void *frontierWork);
BOOL FrontierLinkLobby_SendValue74(u8 *data, u16 value);
BOOL BattleRecord_Load(SaveData *saveData, enum HeapID heapId, int *result, BattleSetup *setup, int index);
void FrontierMap_SetSpriteAnimActive(FrontierMap *map, u32 idx, u32 on);
void *FrontierParticles_CreateSystem(void *manager, u32 index, u32 member, u32 camera);
void FrontierParticles_DestroySystem(void *manager, u32 index);
BOOL FrontierParticles_AreAllDone(void *manager);
void FrontierMap_CreateMonSprite(FrontierMap *map, Pokemon *mon, int heapId, int id, int x, int y, int drawPriority, int bgPriority, int blend, int blendColor);
void FrontierMap_DeleteMonSprite(FrontierMap *map, u16 id);
void FrontierMap_ShakeTask(SysTask *task, FrontierMapShake *shake);
void FrontierMap_WindowTask(SysTask *task, FrontierMapWindowTask *data);
void FrontierMenu_SetAlignRight(void *menu, u8 value);
void FrontierMenu_SetAlignBottom(void *menu, u8 value);

u16 *FrontierScript_ReadVarPtr(FrontierScriptContext *ctx);
u16 FrontierScript_ReadVar(FrontierScriptContext *ctx);
BOOL FrtCmd_DeleteSprite(FrontierScriptContext *ctx);
BOOL FrtCmd_SetSpriteDrawFlag(FrontierScriptContext *ctx);
BOOL FrtCmd_SetSpriteFlip(FrontierScriptContext *ctx);
BOOL FrtCmd_SetSpriteTranslucent(FrontierScriptContext *ctx);
BOOL FrtCmd_StartSpriteAnim(FrontierScriptContext *ctx);
BOOL FrtCmd_StopSpriteAnim(FrontierScriptContext *ctx);
BOOL FrtCmd_WaitSpriteAnim(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitSpriteAnim(FrontierScriptContext *ctx);
void FrontierScript_SetObjectMovement(FrontierScriptContext *ctx, int unused, int count);
BOOL FrtCmd_ClearObjectMovement(FrontierScriptContext *ctx);
BOOL FrtCmd_SetObjectLookAround(FrontierScriptContext *ctx);
BOOL FrtCmd_LaunchNamingScreen(FrontierScriptContext *ctx);
void FrontierScript_FreeNamingScreenArgs(void *args);
BOOL FrtCmd_CreateBattleRecord(FrontierScriptContext *ctx);
BOOL FrtCmd_SaveBattleRecord(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitSaveBattleRecord(FrontierScriptContext *ctx);
int FrontierScript_GetBattleRecordType(int facility, int mode);
BOOL FrtCmd_PlayBattleRecord(FrontierScriptContext *ctx);
BOOL FrtCmd_DeleteBattleRecord(FrontierScriptContext *ctx);
BOOL FrtCmd_CheckBattleRecordSlotValid(FrontierScriptContext *ctx);
BOOL FrtCmd_SaveGameNormal(FrontierScriptContext *ctx);
BOOL FrtCmd_SaveGameAsync(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitSave(FrontierScriptContext *ctx);
BOOL FrtCmd_SaveWipeExtraChunks(FrontierScriptContext *ctx);
BOOL FrtCmd_CheckExtraSaveChunksExist(FrontierScriptContext *ctx);
BOOL FrtCmd_AddWaitingIcon(FrontierScriptContext *ctx);
BOOL FrtCmd_RemoveWaitingIcon(FrontierScriptContext *ctx);
BOOL FrtCmd_BufferItemName(FrontierScriptContext *ctx);
BOOL FrtCmd_BufferInt(FrontierScriptContext *ctx);
u32 FrontierScript_CountDigits(u32 value);
BOOL FrtCmd_BufferPlayersName(FrontierScriptContext *ctx);
BOOL FrtCmd_BufferPartnersName(FrontierScriptContext *ctx);
BOOL FrtCmd_BufferMoveName(FrontierScriptContext *ctx);
BOOL FrtCmd_BufferSpeciesName(FrontierScriptContext *ctx);
String *FrontierScript_ReadString(u32 msgId, enum HeapID heapId);
BOOL FrtCmd_BufferTypeName(FrontierScriptContext *ctx);
BOOL FrtCmd_BufferRivalsName(FrontierScriptContext *ctx);
BOOL FrtCmd_GetBattlePoints(FrontierScriptContext *ctx);
BOOL FrtCmd_GiveBattlePoints(FrontierScriptContext *ctx);
BOOL FrtCmd_TakeBattlePoints(FrontierScriptContext *ctx);
BOOL FrtCmd_LinkSync(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitLinkSync(FrontierScriptContext *ctx);
BOOL FrtCmd_ClearLinkSyncPackets(FrontierScriptContext *ctx);
BOOL FrtCmd_ExitClubLink(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitCommEnd(void);
BOOL FrtCmd_Random(FrontierScriptContext *ctx);
BOOL FrtCmd_HealParty(FrontierScriptContext *ctx);
BOOL FrtCmd_WaitButton(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitButton(FrontierScriptContext *ctx);
BOOL FrtCmd_WaitButtonOrDelay(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitButtonOrFrames(FrontierScriptContext *ctx);
BOOL FrtCmd_Dummy060(FrontierScriptContext *ctx);
BOOL FrtCmd_SetSaveVar(FrontierScriptContext *ctx);
BOOL FrtCmd_GetSaveVar(FrontierScriptContext *ctx);
BOOL FrontierTransition_Flash(FrontierTransitionWork *work);
BOOL FrontierTransition_FlashWipeHorizontal(FrontierTransitionWork *work);
BOOL FrontierTransition_FlashWipeVertical(FrontierTransitionWork *work);
u32 FrontierScript_PackHalfWords(u32 low, u32 high);
void FrontierTransition_RowScrollTask(SysTask *task, FrontierTransitionWork *work);
BOOL FrontierTransition_RowScroll1(FrontierTransitionWork *work);
BOOL FrontierTransition_RowScroll2(FrontierTransitionWork *work);
BOOL FrontierTransition_Wave(FrontierTransitionWork *work);
BOOL FrtCmd_BattleTransition(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitTransition(FrontierScriptContext *ctx);
BOOL FrtCmd_GetPlayerSpriteId(FrontierScriptContext *ctx);
BOOL FrtCmd_CreateParticleSystem(FrontierScriptContext *ctx);
BOOL FrtCmd_DestroyParticleSystem(FrontierScriptContext *ctx);
BOOL FrtCmd_CreateParticleEmitter(FrontierScriptContext *ctx);
BOOL FrtCmd_WaitParticles(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitMapEffect(FrontierScriptContext *ctx);
BOOL FrtCmd_ShakeMap(FrontierScriptContext *ctx);
BOOL FrtCmd_WaitShakeMap(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitShake(FrontierScriptContext *ctx);
BOOL FrtCmd_SetWindowDelayed(FrontierScriptContext *ctx);
BOOL FrtCmd_WaitWindowDelayed(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitWindowTask(FrontierScriptContext *ctx);
BOOL FrtCmd_Dummy150(FrontierScriptContext *ctx);
BOOL FrtCmd_CreatePartyMonSprite(FrontierScriptContext *ctx);
BOOL FrtCmd_DeleteMonSprite(FrontierScriptContext *ctx);
BOOL FrtCmd_BrainIntro(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitBrainIntro(FrontierScriptContext *ctx);
BOOL FrtCmd_IncGameStat(FrontierScriptContext *ctx);
BOOL FrtCmd_AddGameStat(FrontierScriptContext *ctx);
BOOL FrtCmd_AddGameScore(FrontierScriptContext *ctx);
BOOL FrtCmd_CheckPlayerOrPartnerVersion0(FrontierScriptContext *ctx);
BOOL FrtCmd_CheckBattleRecordAiFlagsValid(FrontierScriptContext *ctx);
BOOL FrtCmd_UpdateWifiFriendDate(FrontierScriptContext *ctx);
BOOL FrtCmd_200(FrontierScriptContext *ctx);
BOOL FrtCmd_201(FrontierScriptContext *ctx);
BOOL FrtCmd_LinkSyncOrAbort(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitLinkSyncOrAbort(FrontierScriptContext *ctx);
BOOL FrtCmd_GetSyncAborted(FrontierScriptContext *ctx);
BOOL FrtCmd_ClearSyncAborted(FrontierScriptContext *ctx);
BOOL FrtCmd_MenuSetAlignRight(FrontierScriptContext *ctx);
BOOL FrtCmd_MenuSetAlignBottom(FrontierScriptContext *ctx);

static inline u8 ReadScriptByte(FrontierScriptContext *ctx) {
    const u8 *ptr = ctx->scriptPtr;
    ctx->scriptPtr = ptr + 1;
    return *ptr;
}

BOOL FrtCmd_DeleteSprite(FrontierScriptContext *ctx) {
    FrontierMap *map = Frontier_GetMap(ctx->frontierSystem->frontierWork);
    FrontierMap_DeleteSprite(map, FrontierScript_ReadVar(ctx));
    return FALSE;
}

BOOL FrtCmd_SetSpriteDrawFlag(FrontierScriptContext *ctx) {
    FrontierMap *map = Frontier_GetMap(ctx->frontierSystem->frontierWork);
    u16 index = FrontierScript_ReadVar(ctx);
    u8 draw = ReadScriptByte(ctx);
    ManagedSprite *sprite = FrontierMap_GetSprite(map, index);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    ManagedSprite_SetDrawFlag(sprite, draw);
    return FALSE;
}

BOOL FrtCmd_SetSpriteFlip(FrontierScriptContext *ctx) {
    FrontierMap *map = Frontier_GetMap(ctx->frontierSystem->frontierWork);
    ManagedSprite *sprite = FrontierMap_GetSprite(map, FrontierScript_ReadVar(ctx));
    if (sprite == NULL) {
        GF_AssertFail();
    }
    thunk_Sprite_SetFlipMode(sprite->sprite, 1);
    return FALSE;
}

BOOL FrtCmd_SetSpriteTranslucent(FrontierScriptContext *ctx) {
    FrontierMap *map = Frontier_GetMap(ctx->frontierSystem->frontierWork);
    u16 index = FrontierScript_ReadVar(ctx);
    u16 mode = FrontierScript_ReadVar(ctx);
    ManagedSprite *sprite = FrontierMap_GetSprite(map, index);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    if (mode == 0) {
        thunk_Sprite_SetOamMode(sprite->sprite, GX_OAM_MODE_NORMAL);
    } else {
        thunk_Sprite_SetOamMode(sprite->sprite, GX_OAM_MODE_XLU);
    }
    return FALSE;
}

BOOL FrtCmd_StartSpriteAnim(FrontierScriptContext *ctx) {
    u16 index = FrontierScript_ReadVar(ctx);
    u16 anim = FrontierScript_ReadVar(ctx);
    FrontierMap *map = Frontier_GetMap(ctx->frontierSystem->frontierWork);
    ManagedSprite_SetAnim(FrontierMap_GetSprite(map, index), anim);
    FrontierMap_SetSpriteAnimActive(map, index, TRUE);
    return FALSE;
}

BOOL FrtCmd_StopSpriteAnim(FrontierScriptContext *ctx) {
    u16 index = FrontierScript_ReadVar(ctx);
    FrontierMap_SetSpriteAnimActive(Frontier_GetMap(ctx->frontierSystem->frontierWork), index, FALSE);
    return FALSE;
}

BOOL FrtCmd_WaitSpriteAnim(FrontierScriptContext *ctx) {
    ctx->data[0] = FrontierScript_ReadVar(ctx);
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitSpriteAnim);
    return TRUE;
}

BOOL FrontierScriptNative_WaitSpriteAnim(FrontierScriptContext *ctx) {
    FrontierMap *map;
    ManagedSprite *sprite;
    u16 *index;
    map = Frontier_GetMap(ctx->frontierSystem->frontierWork);
    index = &ctx->data[0];
    sprite = FrontierMap_GetSprite(map, *index);
    if (!FrontierMap_IsSpriteAnimActive(map, *index) || !ManagedSprite_IsAnimated(sprite)) {
        return TRUE;
    }
    return FALSE;
}

void FrontierScript_SetObjectMovement(FrontierScriptContext *ctx, int unused, int count) {
    FrontierSystem *system;
    s16 *params;
    u16 id;
    int i;
    system = ctx->frontierSystem;
    if (count > 7) {
        GF_AssertFail();
    }
    params = Heap_Alloc(system->heapId, count * 2);
    id = FrontierScript_ReadVar(ctx);
    for (i = 0; i < 3; i++) {
        params[i] = FrontierScript_ReadVar(ctx);
    }
    FrontierMap_SetObjectMovement(system->frontierWork, id, 1, params, 3);
    Heap_Free(params);
}

BOOL FrtCmd_ClearObjectMovement(FrontierScriptContext *ctx) {
    u16 id = FrontierScript_ReadVar(ctx);
    FrontierMap_SetObjectMovement(ctx->frontierSystem->frontierWork, id, 0, NULL, 0);
    return FALSE;
}

BOOL FrtCmd_SetObjectLookAround(FrontierScriptContext *ctx) {
    FrontierScript_SetObjectMovement(ctx, 1, 3);
    return FALSE;
}

BOOL FrtCmd_LaunchNamingScreen(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    NamingScreenArgs *namingArgs = NamingScreen_CreateArgs(HEAP_ID_FIELD2, NAME_SCREEN_PLAYER, 0, 8, args->options, NULL);
    Frontier_LaunchApplication(ctx->frontierSystem->frontierWork, &gOverlayTemplate_NamingScreen, namingArgs, 0, FrontierScript_FreeNamingScreenArgs);
    return TRUE;
}

void FrontierScript_FreeNamingScreenArgs(void *args) {
    NamingScreen_DeleteArgs(args);
}

BOOL FrtCmd_CreateBattleRecord(FrontierScriptContext *ctx) {
    u32 out;
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    BattleRecord_Create(args->saveData, HEAP_ID_FIELD2, &out);
    return FALSE;
}

// Saves the battle record of the facility / mode (amount up to 9999), the result goes to the given variable.
BOOL FrtCmd_SaveBattleRecord(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    u32 facility = FrontierScriptContext_ReadHalfWord(ctx);
    u32 mode = FrontierScript_ReadVar(ctx);
    u32 amount = FrontierScript_ReadVar(ctx);
    u16 *result = FrontierScript_ReadVarPtr(ctx);
    if (amount > 9999) {
        amount = 9999;
    }
    system->recordSaveState = 0;
    system->recordWriteState = 0;
    system->recordType = FrontierScript_GetBattleRecordType(facility, mode);
    system->recordAmount = amount;
    system->recordResult = result;
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitSaveBattleRecord);
    return TRUE;
}

BOOL FrontierScriptNative_WaitSaveBattleRecord(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(system->frontierWork);
    int result = BattleRecord_Save(args->saveData, system->recordType, system->recordAmount, 0, &system->recordSaveState, &system->recordWriteState);
    if ((u32)(result - 2) <= 1) {
        *system->recordResult = result == 2;
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on
int FrontierScript_GetBattleRecordType(int facility, int mode) {
    int type = 15;
    switch (facility) {
    case 0:
        break;
    case 1:
        switch (mode) {
        case 0:
            type = 15;
            break;
        case 1:
            type = 16;
            break;
        case 2:
        case 3:
        case 6:
            type = 17;
            break;
        case 4:
        case 5:
            type = 15;
            break;
        default:
            GF_AssertFail();
            break;
        }
        break;
    case 2:
        switch (mode) {
        case 0:
            type = 18;
            break;
        case 1:
            type = 19;
            break;
        case 2:
        case 3:
            type = 20;
            break;
        default:
            GF_AssertFail();
            break;
        }
        break;
    case 3:
        switch (mode) {
        case 0:
            type = 21;
            break;
        case 1:
            type = 22;
            break;
        case 2:
        case 3:
            type = 23;
            break;
        default:
            GF_AssertFail();
            break;
        }
        break;
    case 4:
        switch (mode) {
        case 0:
            type = 27;
            break;
        case 1:
            type = 28;
            break;
        case 2:
        case 3:
            type = 29;
            break;
        default:
            GF_AssertFail();
            break;
        }
        break;
    case 5:
        switch (mode) {
        case 0:
            type = 24;
            break;
        case 1:
            type = 25;
            break;
        case 2:
        case 3:
            type = 26;
            break;
        default:
            GF_AssertFail();
            break;
        }
        break;
    case 6:
        switch (mode) {
        case 0:
            type = 30;
            break;
        case 1:
            type = 31;
            break;
        case 2:
        case 3:
            type = 32;
            break;
        default:
            GF_AssertFail();
            break;
        }
        break;
    default:
        break;
    }
    return type;
}
#pragma force_active reset

// Plays the recorded battle.
BOOL FrtCmd_PlayBattleRecord(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    BattleSetup *setup = Heap_Alloc(HEAP_ID_FIELD2, sizeof(BattleSetup));
    int result;
    MI_CpuFill8(setup, 0, sizeof(BattleSetup));
    BattleRecord_Load(args->saveData, HEAP_ID_FIELD2, &result, setup, 0);
    Sound_SetSceneAndPlayBGM(5, SEQ_GS_VS_TRAINER, 1);
    Frontier_LaunchApplication(ctx->frontierSystem->frontierWork, &gOverlayTemplate_Battle, setup, 1, NULL);
    return TRUE;
}

BOOL FrtCmd_DeleteBattleRecord(FrontierScriptContext *ctx) {
    BattleRecord_Delete();
    return FALSE;
}

BOOL FrtCmd_CheckBattleRecordSlotValid(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    u16 *result = FrontierScript_ReadVarPtr(ctx);
    int out;
    *result = BattleRecord_IsSlotValid(args->saveData, HEAP_ID_FIELD2, &out, 0);
    return FALSE;
}

BOOL FrtCmd_SaveGameNormal(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    FrontierScript_ReadVarPtr(ctx);
    SaveGameNormal(args->saveData);
    return FALSE;
}

#pragma force_active on
BOOL FrtCmd_SaveGameAsync(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(system->frontierWork);
    FrontierScript_ReadVarPtr(ctx);
    Sys_SetSoftResetDisableFlag(4);
    Sys_SetAsyncSaveMarker(system->heapId);
    Save_PrepareForAsyncWrite(args->saveData, 2);
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitSave);
    return TRUE;
}

BOOL FrontierScriptNative_WaitSave(FrontierScriptContext *ctx) {
    int result = Save_WriteFileAsync(Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork)->saveData);
    if (result == 2) {
        Sys_ClearAsyncSaveMarker();
        Sys_ClearSoftResetDisableFlag(4);
        return TRUE;
    }
    if (result == 3) {
        Sys_ClearAsyncSaveMarker();
        Sys_ClearSoftResetDisableFlag(4);
        return TRUE;
    }
    return FALSE;
}
#pragma force_active reset

BOOL FrtCmd_SaveWipeExtraChunks(FrontierScriptContext *ctx) {
    Save_WipeExtraChunks(Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork)->saveData);
    return FALSE;
}

BOOL FrtCmd_CheckExtraSaveChunksExist(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    u16 *result = FrontierScript_ReadVarPtr(ctx);
    *result = Save_CheckExtraChunksExist(args->saveData);
    return FALSE;
}

BOOL FrtCmd_AddWaitingIcon(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    system->waitingIcon = WaitingIcon_New(&system->messageWindow, 0x3E2);
    return FALSE;
}

BOOL FrtCmd_RemoveWaitingIcon(FrontierScriptContext *ctx) {
    WaitingIcon_Delete(ctx->frontierSystem->waitingIcon);
    return FALSE;
}

BOOL FrtCmd_BufferItemName(FrontierScriptContext *ctx) {
    MessageFormat *msgFormat;
    u8 field;
    u16 item;
    field = ReadScriptByte(ctx);
    item = FrontierScript_ReadVar(ctx);
    msgFormat = ctx->frontierSystem->msgFormat;
    BufferItemName(msgFormat, field, item);
    return FALSE;
}

BOOL FrtCmd_BufferInt(FrontierScriptContext *ctx) {
    u8 field;
    u16 value;
    u32 digits;
    field = ReadScriptByte(ctx);
    value = FrontierScript_ReadVar(ctx);
    digits = FrontierScript_CountDigits(value);
    BufferIntegerAsString(ctx->frontierSystem->msgFormat, field, value, digits, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    return FALSE;
}

u32 FrontierScript_CountDigits(u32 value) {
    if (value / 10 == 0) {
        return 1;
    }
    if (value / 100 == 0) {
        return 2;
    }
    if (value / 1000 == 0) {
        return 3;
    }
    if (value / 10000 == 0) {
        return 4;
    }
    if (value / 100000 == 0) {
        return 5;
    }
    if (value / 1000000 == 0) {
        return 6;
    }
    if (value / 10000000 == 0) {
        return 7;
    }
    if (value / 100000000 == 0) {
        return 8;
    }
    return 1;
}

BOOL FrtCmd_BufferPlayersName(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    u8 field;
    PlayerProfile *profile;
    field = ReadScriptByte(ctx);
    profile = Save_PlayerData_GetProfile(args->saveData);
    BufferPlayersName(ctx->frontierSystem->msgFormat, field, profile);
    return FALSE;
}

BOOL FrtCmd_BufferPartnersName(FrontierScriptContext *ctx) {
    Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    u8 field = ReadScriptByte(ctx);
    PlayerProfile *profile = CommPlayerInfo_GetProfile(Comm_GetLocalNetId() ^ 1);
    BufferPlayersName(ctx->frontierSystem->msgFormat, field, profile);
    return FALSE;
}

BOOL FrtCmd_BufferMoveName(FrontierScriptContext *ctx) {
    u8 field;
    u16 move;
    field = ReadScriptByte(ctx);
    move = FrontierScript_ReadVar(ctx);
    BufferMoveName(ctx->frontierSystem->msgFormat, field, move);
    return FALSE;
}

BOOL FrtCmd_BufferSpeciesName(FrontierScriptContext *ctx) {
    u8 field;
    u16 msgId;
    u16 a3;
    u8 a4;
    String *string;
    field = ReadScriptByte(ctx);
    msgId = FrontierScript_ReadVar(ctx);
    a3 = FrontierScriptContext_ReadHalfWord(ctx);
    a4 = ReadScriptByte(ctx);
    string = FrontierScript_ReadString(msgId, HEAP_ID_FIELD2);
    BufferString(ctx->frontierSystem->msgFormat, field, string, a3, a4, 2);
    String_Delete(string);
    return FALSE;
}

String *FrontierScript_ReadString(u32 msgId, enum HeapID heapId) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0237_bin, heapId);
    String *string = NewString_ReadMsgData(msgData, msgId);
    DestroyMsgData(msgData);
    return string;
}

BOOL FrtCmd_BufferTypeName(FrontierScriptContext *ctx) {
    u8 field;
    u16 type;
    field = ReadScriptByte(ctx);
    type = FrontierScript_ReadVar(ctx);
    BufferTypeName(ctx->frontierSystem->msgFormat, field, type);
    return FALSE;
}

BOOL FrtCmd_BufferRivalsName(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    BufferRivalsName(ctx->frontierSystem->msgFormat, ReadScriptByte(ctx), args->saveData);
    return FALSE;
}

BOOL FrtCmd_GetBattlePoints(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    u16 *result = FrontierScript_ReadVarPtr(ctx);
    *result = FrontierData_BattlePointAction(Save_FrontierData_Get(args->saveData), 0, 0);
    return FALSE;
}

BOOL FrtCmd_GiveBattlePoints(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    u16 points = FrontierScript_ReadVar(ctx);
    GameStats_Add(Save_GameStats_Get(args->saveData), GAME_STAT_BATTLE_POINTS_RECEIVED, points);
    FrontierData_BattlePointAction(Save_FrontierData_Get(args->saveData), points, 5);
    return FALSE;
}

BOOL FrtCmd_TakeBattlePoints(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    u16 points = FrontierScript_ReadVar(ctx);
    GameStats_Add(Save_GameStats_Get(args->saveData), GAME_STAT_BATTLE_POINTS_SPENT, points);
    FrontierData_BattlePointAction(Save_FrontierData_Get(args->saveData), points, 6);
    return FALSE;
}

BOOL FrtCmd_LinkSync(FrontierScriptContext *ctx) {
    u16 syncId = FrontierScript_ReadVar(ctx);
    ctx->data[0] = syncId;
    LinkSync_StartSync((u8)syncId);
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitLinkSync);
    return TRUE;
}

BOOL FrontierScriptNative_WaitLinkSync(FrontierScriptContext *ctx) {
    if (Comm_CountConnected() < 2) {
        return TRUE;
    }
    return LinkSync_IsSynced((u8)ctx->data[0]);
}

BOOL FrtCmd_ClearLinkSyncPackets(FrontierScriptContext *ctx) {
    LinkSync_ClearPackets();
    return FALSE;
}

BOOL FrtCmd_ExitClubLink(FrontierScriptContext *ctx) {
    CommClubLink_Exit();
    FrontierScriptContext_Pause(ctx, (FrontierShouldResumeScriptFunc)FrontierScriptNative_WaitCommEnd);
    return TRUE;
}

BOOL FrontierScriptNative_WaitCommEnd(void) {
    if (CommState_Exists() != TRUE && CommWireless_Exists() != TRUE) {
        return TRUE;
    }
    return FALSE;
}

BOOL FrtCmd_Random(FrontierScriptContext *ctx) {
    u16 *result = FrontierScript_ReadVarPtr(ctx);
    u16 max = FrontierScript_ReadVar(ctx);
    *result = LCRandom() % max;
    return TRUE;
}

BOOL FrtCmd_HealParty(FrontierScriptContext *ctx) {
    HealParty(SaveArray_Party_Get(Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork)->saveData));
    return FALSE;
}

BOOL FrtCmd_WaitButton(FrontierScriptContext *ctx) {
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitButton);
    return TRUE;
}

BOOL FrontierScriptNative_WaitButton(FrontierScriptContext *ctx) {
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B) || System_GetTouchNew()) {
        return TRUE;
    }
    return FALSE;
}

BOOL FrtCmd_WaitButtonOrDelay(FrontierScriptContext *ctx) {
    ctx->data[0] = FrontierScript_ReadVar(ctx);
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitButtonOrFrames);
    return TRUE;
}

BOOL FrontierScriptNative_WaitButtonOrFrames(FrontierScriptContext *ctx) {
    if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) || System_GetTouchNew()) {
        return TRUE;
    }
    ctx->data[0]--;
    if (ctx->data[0] == 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL FrtCmd_Dummy060(FrontierScriptContext *ctx) {
    FrontierScript_ReadVarPtr(ctx);
    return TRUE;
}

BOOL FrtCmd_SetSaveVar(FrontierScriptContext *ctx) {
    u16 varId = FrontierScriptContext_ReadHalfWord(ctx);
    u16 value = FrontierScript_ReadVar(ctx);
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    *Save_VarsFlags_GetVarAddr(Save_VarsFlags_Get(args->saveData), varId) = value;
    return FALSE;
}

BOOL FrtCmd_GetSaveVar(FrontierScriptContext *ctx) {
    u16 varId = FrontierScriptContext_ReadHalfWord(ctx);
    u16 *result = FrontierScript_ReadVarPtr(ctx);
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    *result = *Save_VarsFlags_GetVarAddr(Save_VarsFlags_Get(args->saveData), varId);
    return FALSE;
}

BOOL FrontierTransition_Flash(FrontierTransitionWork *work) {
    switch (work->state) {
    case 0:
        FrontierTransition_StartFlash(1, 16, -16, &work->flashDone, 2);
        work->state++;
        break;
    case 1:
        if (work->flashDone != 0) {
            ResetVisibleHardwareWindows(PM_LCD_TOP);
            ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)32, (enum FadeType)32, RGB_BLACK, 12, 1, HEAP_ID_FIELD2);
            work->state++;
        }
        break;
    default:
        if (IsPaletteFadeFinished() == TRUE) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL FrontierTransition_FlashWipeHorizontal(FrontierTransitionWork *work) {
    BOOL done;
    switch (work->state) {
    case 0:
        FrontierTransition_StartFlash(1, 16, -16, &work->flashDone, 2);
        work->state++;
        break;
    case 1:
        if (work->flashDone == 0) {
            break;
        }
        work->window = AllocWindows(HEAP_ID_FIELD2, 1);
        AddWindowParameterized(work->map->bgConfig, work->window, GF_BG_LYR_MAIN_1, 0, 0, 32, 32, 0, 0);
        PaletteData_FillPaletteInBuffer(work->map->paletteData, PLTTBUF_MAIN_BG, PLTTSEL_BOTH, 0, 0, 16);
        FillWindowPixelBuffer(work->window, 0);
        ScheduleWindowCopyToVram(work->window);
        work->wipe = FrontierWipe_New(HEAP_ID_FIELD2);
        work->state++;
    case 2:
        FrontierWipe_Start(work->wipe, 1, 1, work->window, 15);
        work->state++;
        break;
    case 3:
        done = FrontierWipe_UpdateHorizontal(work->wipe);
        ScheduleWindowCopyToVram(work->window);
        if (done != FALSE) {
            work->state++;
        }
        break;
    default:
        if (IsPaletteFadeFinished() == TRUE) {
            FrontierWipe_Free(work->wipe);
            ClearWindowTilemapAndCopyToVram(work->window);
            RemoveWindow(work->window);
            WindowArray_Delete(work->window, 1);
            SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
            SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
            BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, HEAP_ID_FIELD2);
            BgClearTilemapBufferAndCommit(work->map->bgConfig, GF_BG_LYR_MAIN_1);
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL FrontierTransition_FlashWipeVertical(FrontierTransitionWork *work) {
    BOOL done;
    switch (work->state) {
    case 0:
        FrontierTransition_StartFlash(1, 16, -16, &work->flashDone, 2);
        work->state++;
        break;
    case 1:
        if (work->flashDone == 0) {
            break;
        }
        work->window = AllocWindows(HEAP_ID_FIELD2, 1);
        AddWindowParameterized(work->map->bgConfig, work->window, GF_BG_LYR_MAIN_1, 0, 0, 32, 32, 0, 0);
        PaletteData_FillPaletteInBuffer(work->map->paletteData, PLTTBUF_MAIN_BG, PLTTSEL_BOTH, 0, 0, 16);
        FillWindowPixelBuffer(work->window, 0);
        ScheduleWindowCopyToVram(work->window);
        work->wipe = FrontierWipe_New(HEAP_ID_FIELD2);
        work->state++;
    case 2:
        FrontierWipe_Start(work->wipe, 1, 1, work->window, 15);
        work->state++;
        break;
    case 3:
        done = FrontierWipe_UpdateVertical(work->wipe);
        ScheduleWindowCopyToVram(work->window);
        if (done != FALSE) {
            work->state++;
        }
        break;
    default:
        if (IsPaletteFadeFinished() == TRUE) {
            FrontierWipe_Free(work->wipe);
            ClearWindowTilemapAndCopyToVram(work->window);
            RemoveWindow(work->window);
            WindowArray_Delete(work->window, 1);
            SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
            SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
            BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, HEAP_ID_FIELD2);
            BgClearTilemapBufferAndCommit(work->map->bgConfig, GF_BG_LYR_MAIN_1);
            return FALSE;
        }
        break;
    }
    return TRUE;
}

u32 FrontierScript_PackHalfWords(u32 low, u32 high) {
    return low | (high << 16);
}

#pragma force_active on
void FrontierTransition_RowScrollTask(SysTask *task, FrontierTransitionWork *work) {
    int i1;
    u8 *buffer1;
    int bg2X, bg2Y, bg3X, bg3Y;
    u8 *buffer2;
    int i2;
    MtxFx22 matrix;
    FrontierRowScroll *rows = work->rows;
    bg2X = GetBgHOffset(work->map->bgConfig, GF_BG_LYR_MAIN_2);
    bg2Y = GetBgHOffset(work->map->bgConfig, GF_BG_LYR_MAIN_2);
    bg3X = GetBgHOffset(work->map->bgConfig, GF_BG_LYR_MAIN_3);
    bg3Y = GetBgHOffset(work->map->bgConfig, GF_BG_LYR_MAIN_3);

    if (work->finished == TRUE) {
        FrontierRowScrollLine *line;
        buffer1 = FrontierHBlankDma_GetBuffer(rows->dma);
        i1 = 0;
        line = rows->lines;
        for (; i1 < 96; i1++, line++) {
            int j;
            line->x = 0;
            line->y = 0;
            j = line->start;
            if (j < line->end) {
                u8 *dest = buffer1 + j * 32;
                do {
                    MTX22_2DAffine(&matrix, 0, FX32_ONE, FX32_ONE, 0);
                    {
                        s16 x = bg2X + line->x, y = bg2Y + line->y;
                        if (rows->mode == 2) {
                            x %= 256;
                            y %= 256;
                        } else if (rows->mode == 1) {
                            if (x < 0) {
                                x += 256;
                            }
                            if (y < 0) {
                                y = -y;
                            }
                            x %= 256;
                        }
                        G2x_SetBGyAffine_((u32)dest, &matrix, 0, 0, x, y);
                    }
                    {
                        s16 x = bg3X + line->x, y = bg3Y + line->y;
                        if (rows->mode == 2) {
                            x %= 256;
                            y %= 256;
                        } else if (rows->mode == 1) {
                            if (x < 0) {
                                x += 256;
                            }
                            if (y < 0) {
                                y = -y;
                            }
                            x %= 256;
                        }
                        G2x_SetBGyAffine_((u32)dest + 16, &matrix, 0, 0, x, y);
                    }
                    j++;
                    dest += 32;
                } while (j < line->end);
            }
        }
        FrontierHBlankDma_Delete(rows->dma);
        Heap_Free(rows);
        SysTask_Destroy(task);
    } else {
        FrontierRowScrollLine *line;
        buffer2 = FrontierHBlankDma_GetBuffer(rows->dma);
        i2 = 0;
        line = rows->lines;
        for (; i2 < 96; i2++, line++) {
            int j;
            line->x += line->speedX;
            line->y += line->speedY;
            j = line->start;
            if (j < line->end) {
                u8 *dest = buffer2 + j * 32;
                do {
                    MTX22_2DAffine(&matrix, 0, FX32_ONE, FX32_ONE, 0);
                    {
                        s16 x = bg2X + line->x, y = bg2Y + line->y;
                        if (rows->mode == 2) {
                            x %= 256;
                            y %= 256;
                        } else if (rows->mode == 1) {
                            if (x < 0) {
                                x += 256;
                            }
                            if (y < 0) {
                                y = -y;
                            }
                            x %= 256;
                        }
                        G2x_SetBGyAffine_((u32)dest, &matrix, 0, 0, x, y);
                    }
                    {
                        s16 x = bg3X + line->x, y = bg3Y + line->y;
                        if (rows->mode == 2) {
                            x %= 256;
                            y %= 256;
                        } else if (rows->mode == 1) {
                            if (x < 0) {
                                x += 256;
                            }
                            if (y < 0) {
                                y = -y;
                            }
                            x %= 256;
                        }
                        G2x_SetBGyAffine_((u32)dest + 16, &matrix, 0, 0, x, y);
                    }
                    j++;
                    dest += 32;
                } while (j < line->end);
            }
        }
    }
}
#pragma force_active reset

BOOL FrontierTransition_RowScroll1(FrontierTransitionWork *work) {
    switch (work->state) {
    case 0:
        FrontierTransition_StartFlash(1, 16, -16, &work->flashDone, 2);
        work->state++;
        break;
    case 1:
        if (work->flashDone != 0) {
            FrontierScanlineRegs initial;
            int i;
            int start;
            int offset;
            MI_CpuFill8(&initial, 0, sizeof(initial));
            work->finished = FALSE;
            work->rows = Heap_Alloc(HEAP_ID_101, sizeof(FrontierRowScroll));
            work->rows->mode = 1;
            work->rows->dma = FrontierHBlankDma_New((void *)REG_BG2PA_ADDR, &initial, HEAP_ID_101);
            i = 0;
            start = 0;
            offset = 0;
            for (; i < 96; i++) {
                int distance;
                *(s16 *)((u8 *)work->rows + offset) = start;
                *(s16 *)((u8 *)work->rows + offset + 2) = *(s16 *)((u8 *)work->rows + offset) + 2;
                distance = 49 - i;
                *(s16 *)((u8 *)work->rows + offset + 4) = distance % 8;
                *(s16 *)((u8 *)work->rows + offset + 6) = distance / 4;
                *(s16 *)((u8 *)work->rows + offset + 8) = 0;
                *(s16 *)((u8 *)work->rows + offset + 10) = 0;
                *(u32 *)((u8 *)work->rows + offset + 12) = FrontierScript_PackHalfWords(0, 0);
                start += 2;
                offset += sizeof(FrontierRowScrollLine);
            }
            StartBrightnessTransition(40, -16, 0, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ), 1);
            SysTask_CreateOnMainQueue((SysTaskFunc)FrontierTransition_RowScrollTask, work, 0x1000);
            work->state++;
        }
        break;
    case 2:
        if (IsBrightnessTransitionActive(1)) {
            work->finished = TRUE;
            ToggleBgLayer(GF_BG_LYR_MAIN_3, GF_PLANE_TOGGLE_OFF);
            BgSetPosTextAndCommit(work->map->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
            BgSetPosTextAndCommit(work->map->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
            work->state++;
        }
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

BOOL FrontierTransition_RowScroll2(FrontierTransitionWork *work) {
    switch (work->state) {
    case 0:
        FrontierTransition_StartFlash(1, 16, -16, &work->flashDone, 2);
        work->state++;
        break;
    case 1:
        if (work->flashDone != 0) {
            FrontierScanlineRegs initial;
            int i;
            int start;
            int offset;
            MI_CpuFill8(&initial, 0, sizeof(initial));
            work->finished = FALSE;
            work->rows = Heap_Alloc(HEAP_ID_101, sizeof(FrontierRowScroll));
            work->rows->mode = 2;
            work->rows->dma = FrontierHBlankDma_New((void *)REG_BG2PA_ADDR, &initial, HEAP_ID_101);
            i = 0;
            start = 0;
            offset = 0;
            for (; i < 96; i++) {
                int distance;
                int speed;
                *(s16 *)((u8 *)work->rows + offset) = start;
                *(s16 *)((u8 *)work->rows + offset + 2) = *(s16 *)((u8 *)work->rows + offset) + 2;
                distance = 48 - i;
                speed = distance / 8 + 1;
                *(s16 *)((u8 *)work->rows + offset + 4) = speed;
                if (i % 2 != 0) {
                    *(s16 *)((u8 *)work->rows + offset + 4) *= -1;
                }
                if (i < 48) {
                    *(s16 *)((u8 *)work->rows + offset + 6) = i;
                } else {
                    *(s16 *)((u8 *)work->rows + offset + 6) = 96 - i;
                }
                *(s16 *)((u8 *)work->rows + offset + 8) = 0;
                *(s16 *)((u8 *)work->rows + offset + 10) = 0;
                *(u32 *)((u8 *)work->rows + offset + 12) = FrontierScript_PackHalfWords(0, 0);
                start += 2;
                offset += sizeof(FrontierRowScrollLine);
            }
            StartBrightnessTransition(40, -16, 0, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ), 1);
            SysTask_CreateOnMainQueue((SysTaskFunc)FrontierTransition_RowScrollTask, work, 0x1000);
            work->state++;
        }
        break;
    case 2:
        if (IsBrightnessTransitionActive(1)) {
            work->finished = TRUE;
            ToggleBgLayer(GF_BG_LYR_MAIN_3, GF_PLANE_TOGGLE_OFF);
            BgSetPosTextAndCommit(work->map->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
            BgSetPosTextAndCommit(work->map->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
            work->state++;
        }
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

BOOL FrontierTransition_Wave(FrontierTransitionWork *work) {
    switch (work->state) {
    case 0:
        FrontierScanline_Init(&work->scanline, HEAP_ID_101);
        work->state++;
        break;
    case 1:
        FrontierScanline_StartWave(&work->scanline, 0, 191, 0x2AA, 0x12000, 400, (void *)REG_BG3HOFS_ADDR, 0, 4);
        work->state++;
        break;
    }
    return TRUE;
}

// Runs screen transition var (table gFrontierTransitionFuncs) while the frontier app's data points to its work.
BOOL FrtCmd_BattleTransition(FrontierScriptContext *ctx) {
    FrontierTransitionWork *work;
    Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    ctx->data[0] = FrontierScript_ReadVar(ctx);
    Sound_SetSceneAndPlayBGM(5, SEQ_GS_VS_TRAINER, 1);
    work = Heap_Alloc(HEAP_ID_FIELD2, sizeof(FrontierTransitionWork));
    work->prevData = Frontier_GetData(ctx->frontierSystem->frontierWork);
    work->state = 0;
    work->transition = ctx->data[0];
    work->map = FrontierSystem_GetFrontierMap(ctx->frontierSystem);
    Frontier_SetData(ctx->frontierSystem->frontierWork, (u32)work);
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitTransition);
    PaletteData_FillPaletteInBuffer(work->map->paletteData, PLTTBUF_MAIN_BG, PLTTSEL_BOTH, 0, 0, 1);
    return TRUE;
}

BOOL FrontierScriptNative_WaitTransition(FrontierScriptContext *ctx) {
    FrontierTransitionWork *work;
    BOOL running;
    work = Frontier_GetData(ctx->frontierSystem->frontierWork);
    running = gFrontierTransitionFuncs[work->transition](work);
    if (running == FALSE) {
        SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
        SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
        Frontier_SetData(ctx->frontierSystem->frontierWork, (u32)work->prevData);
        Heap_Free(work);
    }
    return running == FALSE;
}

BOOL FrtCmd_GetPlayerSpriteId(FrontierScriptContext *ctx) {
    u16 *result = FrontierScript_ReadVarPtr(ctx);
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    PlayerProfile *profile = Save_PlayerData_GetProfile(args->saveData);
    if (PlayerProfile_GetTrainerGender(profile) == PLAYER_GENDER_MALE) {
        *result = 0;
    } else {
        *result = 0x61;
    }
    return FALSE;
}

BOOL FrtCmd_CreateParticleSystem(FrontierScriptContext *ctx) {
    FrontierMap *map = Frontier_GetMap(ctx->frontierSystem->frontierWork);
    u16 index = FrontierScript_ReadVar(ctx);
    u16 member = FrontierScript_ReadVar(ctx);
    u16 camera = FrontierScript_ReadVar(ctx);
    FrontierParticles_CreateSystem((void *)map->particleManager, index, member, camera);
    return FALSE;
}

BOOL FrtCmd_DestroyParticleSystem(FrontierScriptContext *ctx) {
    FrontierMap *map = Frontier_GetMap(ctx->frontierSystem->frontierWork);
    FrontierParticles_DestroySystem((void *)map->particleManager, FrontierScript_ReadVar(ctx));
    return FALSE;
}

BOOL FrtCmd_CreateParticleEmitter(FrontierScriptContext *ctx) {
    FrontierMap *map = Frontier_GetMap(ctx->frontierSystem->frontierWork);
    u16 index = FrontierScript_ReadVar(ctx);
    u16 resource = FrontierScript_ReadVar(ctx);
    ParticleSystem_CreateEmitterWithParam(FrontierParticles_GetSystem(map->particleManager, index), resource, NULL, NULL);
    return FALSE;
}

BOOL FrtCmd_WaitParticles(FrontierScriptContext *ctx) {
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitMapEffect);
    return TRUE;
}

BOOL FrontierScriptNative_WaitMapEffect(FrontierScriptContext *ctx) {
    return FrontierParticles_AreAllDone((void *)((FrontierMap *)Frontier_GetMap(ctx->frontierSystem->frontierWork))->particleManager) == TRUE;
}

// Shakes the map: offset x / y, flipped every interval + 1 frames, count times.
BOOL FrtCmd_ShakeMap(FrontierScriptContext *ctx) {
    u16 x;
    u16 y;
    u16 interval;
    u16 count;
    FrontierMapShake *shake = &((FrontierMap *)Frontier_GetMap(ctx->frontierSystem->frontierWork))->shake;
    if (shake->task != NULL) {
        GF_AssertFail();
        SysTask_Destroy(shake->task);
    }
    x = FrontierScript_ReadVar(ctx);
    y = FrontierScript_ReadVar(ctx);
    interval = FrontierScript_ReadVar(ctx);
    count = FrontierScript_ReadVar(ctx);
    MI_CpuFill8(shake, 0, sizeof(FrontierMapShake));
    shake->x = x;
    shake->y = y;
    shake->interval = interval;
    shake->count = count;
    shake->task = SysTask_CreateOnMainQueue((SysTaskFunc)FrontierMap_ShakeTask, shake, 70000);
    return FALSE;
}

BOOL FrtCmd_WaitShakeMap(FrontierScriptContext *ctx) {
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitShake);
    return TRUE;
}

BOOL FrontierScriptNative_WaitShake(FrontierScriptContext *ctx) {
    return ((FrontierMap *)Frontier_GetMap(ctx->frontierSystem->frontierWork))->shake.task == NULL;
}

#pragma force_active on
// Sets the main screen's window 0 (or turns it off) after a delay.
BOOL FrtCmd_SetWindowDelayed(FrontierScriptContext *ctx) {
    FrontierMap *map = Frontier_GetMap(ctx->frontierSystem->frontierWork);
    FrontierMapWindowTask *data = &map->windowTask;
    if (data->task) {
        GF_AssertFail();
        SysTask_Destroy(data->task);
    }
    MI_CpuFill8(data, 0, sizeof(FrontierMapWindowTask));
    data->left = FrontierScript_ReadVar(ctx);
    data->top = FrontierScript_ReadVar(ctx);
    data->right = FrontierScript_ReadVar(ctx);
    data->bottom = FrontierScript_ReadVar(ctx);
    data->delay = FrontierScript_ReadVar(ctx);
    data->enable = FrontierScript_ReadVar(ctx);
    data->task = SysTask_CreateOnMainQueue((SysTaskFunc)FrontierMap_WindowTask, data, 300);
    return FALSE;
}
#pragma force_active reset

BOOL FrtCmd_WaitWindowDelayed(FrontierScriptContext *ctx) {
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitWindowTask);
    return TRUE;
}

BOOL FrontierScriptNative_WaitWindowTask(FrontierScriptContext *ctx) {
    return ((FrontierMap *)Frontier_GetMap(ctx->frontierSystem->frontierWork))->windowTask.task == NULL;
}

BOOL FrtCmd_Dummy150(FrontierScriptContext *ctx) {
    FrontierScriptContext_ReadHalfWord(ctx);
    return FALSE;
}

#pragma force_active on
BOOL FrtCmd_CreatePartyMonSprite(FrontierScriptContext *ctx) {
    FrontierMap *map = FrontierSystem_GetFrontierMap(ctx->frontierSystem);
    u32 slot = FrontierScript_ReadVar(ctx);
    int x = FrontierScript_ReadVar(ctx), y = FrontierScript_ReadVar(ctx);
    int id = FrontierScriptContext_ReadHalfWord(ctx);
    Pokemon *mon;
    FrontierScriptContext_ReadHalfWord(ctx);
    FrontierScriptContext_ReadHalfWord(ctx);
    mon = Party_GetMonByIndex(SaveArray_Party_Get(Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork)->saveData), slot);
    FrontierMap_CreateMonSprite(map, mon, HEAP_ID_FIELD2, id, x, y, 0, 0, 8, 0);
    return FALSE;
}
#pragma force_active reset

BOOL FrtCmd_DeleteMonSprite(FrontierScriptContext *ctx) {
    FrontierMap *map = FrontierSystem_GetFrontierMap(ctx->frontierSystem);
    FrontierMap_DeleteMonSprite(map, FrontierScriptContext_ReadHalfWord(ctx));
    return FALSE;
}

#pragma force_active on
BOOL FrtCmd_BrainIntro(FrontierScriptContext *ctx) {
    FrontierMap *map = FrontierSystem_GetFrontierMap(ctx->frontierSystem);
    u16 brain = FrontierScriptContext_ReadHalfWord(ctx);
    FrontierCameraPos pos;
    FrontierMap_GetCameraPos(map, (u16 *)&pos.x, (u16 *)&pos.y);
    FrontierBrainIntro_Start(brain, map->bgConfig, map->spriteSystem, map->spriteManager, map->paletteData, &ctx->data[0], pos.x, pos.y);
    Sound_SetSceneAndPlayBGM(5, SEQ_GS_BA_BRAIN, 1);
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitBrainIntro);
    return TRUE;
}
#pragma force_active reset

BOOL FrontierScriptNative_WaitBrainIntro(FrontierScriptContext *ctx) {
    Frontier_GetMap(ctx->frontierSystem->frontierWork);
    return ctx->data[0] == TRUE;
}

BOOL FrtCmd_IncGameStat(FrontierScriptContext *ctx) {
    u16 stat = FrontierScriptContext_ReadHalfWord(ctx);
    GameStats_Inc(Save_GameStats_Get(Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork)->saveData), stat);
    return FALSE;
}

BOOL FrtCmd_AddGameStat(FrontierScriptContext *ctx) {
    u16 stat = FrontierScriptContext_ReadHalfWord(ctx);
    u16 value = FrontierScript_ReadVar(ctx);
    GameStats_Add(Save_GameStats_Get(Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork)->saveData), stat, value);
    return FALSE;
}

BOOL FrtCmd_AddGameScore(FrontierScriptContext *ctx) {
    u16 score = FrontierScriptContext_ReadHalfWord(ctx);
    GameStats_AddScore(Save_GameStats_Get(Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork)->saveData), score);
    return FALSE;
}

BOOL FrtCmd_CheckPlayerOrPartnerVersion0(FrontierScriptContext *ctx) {
    u16 *result = FrontierScript_ReadVarPtr(ctx);
    *result = Save_IsPlayerOrPartnerVersion0(Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork)->saveData);
    return FALSE;
}

BOOL FrtCmd_CheckBattleRecordAiFlagsValid(FrontierScriptContext *ctx) {
    u16 *result = FrontierScript_ReadVarPtr(ctx);
    *result = BattleRecord_AreAiFlagsValid();
    return FALSE;
}

BOOL FrtCmd_UpdateWifiFriendDate(FrontierScriptContext *ctx) {
    FriendRoster *roster = Save_FriendRoster_Get(Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork)->saveData);
    FriendRoster_UpdateDate(roster, WifiConnection_GetTargetFriendIndex());
    return FALSE;
}

BOOL FrtCmd_200(FrontierScriptContext *ctx) {
    Save_Block23_Get(Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork)->saveData);
    FrontierScript_ReadVar(ctx);
    return FALSE;
}

BOOL FrtCmd_201(FrontierScriptContext *ctx) {
    Comm_SetWifiContinuousSend(FrontierScript_ReadVar(ctx));
    return FALSE;
}

BOOL FrtCmd_LinkSyncOrAbort(FrontierScriptContext *ctx) {
    u16 syncId = FrontierScript_ReadVar(ctx);
    ctx->data[0] = syncId;
    LinkSync_StartSync((u8)syncId);
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitLinkSyncOrAbort);
    return TRUE;
}

// As FrontierScriptNative_WaitLinkSync, but also stops waiting (system->syncAborted) when the partner's data says so.
BOOL FrontierScriptNative_WaitLinkSyncOrAbort(FrontierScriptContext *ctx) {
    BOOL result;
    u8 *data;
    if (Comm_CountConnected() < 2) {
        result = TRUE;
    } else {
        result = LinkSync_IsSynced((u8)ctx->data[0]);
    }
    if (!result) {
        data = Frontier_GetData(ctx->frontierSystem->frontierWork);
        if (data != NULL && data[0x6F] != 0) {
            if (*(u16 *)(data + 0x74) == 1 || *(u16 *)(data + 0x74) == 0xFFFE) {
                ctx->frontierSystem->syncAborted = TRUE;
                result = TRUE;
            } else if (*(u16 *)(data + 0x74) == 0) {
                if (PlayerProfile_GetVersion(CommPlayerInfo_GetProfile(Comm_GetLocalNetId() ^ 1)) == VERSION_PLATINUM) {
                    data[0x6F] = 0;
                    FrontierLinkLobby_SendValue74(data, 0);
                }
            }
        }
    }
    return result;
}

BOOL FrtCmd_GetSyncAborted(FrontierScriptContext *ctx) {
    u16 *result = FrontierScript_ReadVarPtr(ctx);
    *result = ctx->frontierSystem->syncAborted;
    return TRUE;
}

BOOL FrtCmd_ClearSyncAborted(FrontierScriptContext *ctx) {
    ctx->frontierSystem->syncAborted = FALSE;
    return TRUE;
}

BOOL FrtCmd_MenuSetAlignRight(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    FrontierMenu_SetAlignRight(system->menu, ReadScriptByte(ctx));
    return TRUE;
}

BOOL FrtCmd_MenuSetAlignBottom(FrontierScriptContext *ctx) {
    FrontierSystem *system = ctx->frontierSystem;
    FrontierMenu_SetAlignBottom(system->menu, ReadScriptByte(ctx));
    return TRUE;
}
