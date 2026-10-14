#include "global.h"

#include "constants/game_stats.h"
#include "constants/global_fieldmap.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0040.h"

#include "battle/battle_setup.h"
#include "bg_window.h"
#include "dialog_box.h"
#include "encounter.h"
#include "field_effects.h"
#include "field_system.h"
#include "follow_mon.h"
#include "game_stats.h"
#include "gear_phone_ring_manager.h"
#include "heap.h"
#include "map_object.h"
#include "map_object_movement_commands.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_01.h"
#include "player_avatar_actions.h"
#include "field_emote_mark.h"
#include "party.h"
#include "player_avatar.h"
#include "player_data.h"
#include "pokemon.h"
#include "render_window.h"
#include "save.h"
#include "script_pokemon_util.h"
#include "sound_playback.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "system.h"
#include "task.h"

// The fishing rod: Task_OverworldFish runs the encounter around the rod animation, a SysTask that casts, waits for a
// bite and for the player to reel in with A, and prints the result.

enum FishingRodState {
    FISHING_ROD_STATE_INIT,
    FISHING_ROD_STATE_START_CAST,
    FISHING_ROD_STATE_CAST,
    FISHING_ROD_STATE_SETUP_BITE,
    FISHING_ROD_STATE_WAIT_BITE,
    FISHING_ROD_STATE_WAIT_REEL,
    FISHING_ROD_STATE_HOOKED,
    FISHING_ROD_STATE_SHOW_LANDED,
    FISHING_ROD_STATE_WAIT_LANDED_MESSAGE,
    FISHING_ROD_STATE_LANDED,
    FISHING_ROD_STATE_REELED_TOO_SOON,
    FISHING_ROD_STATE_GOT_AWAY,
    FISHING_ROD_STATE_SETUP_NO_BITE,
    FISHING_ROD_STATE_WAIT_NO_BITE,
    FISHING_ROD_STATE_WAIT_FAIL_MESSAGE,
    FISHING_ROD_STATE_PUT_AWAY,
    FISHING_ROD_STATE_WAIT_PUT_AWAY,
    FISHING_ROD_STATE_FINISH,
};

// The player's fishing animations (MapObject_SetAnimation).
enum FishingRodAnim {
    FISHING_ROD_ANIM_IDLE,
    FISHING_ROD_ANIM_CAST,
    FISHING_ROD_ANIM_BITE,
    FISHING_ROD_ANIM_REEL,
};

// The rod animation SysTask's data.
typedef struct FishingRod {
    BOOL hooked;      // a wild Pokemon will bite
    BOOL finished;
    BOOL landed;
    u32 state;
    s32 timer;
    s32 biteDelay;    // frames until the bite
    s32 reelWindow;   // frames the player has to press A after the bite
    u32 rodType;
    FieldSystem *fieldSystem;
    int emoteMark;
    u8 printerId;
    String *string;
    String *template;
    MessageFormat *msgFmt;
    Window window;
    MsgData *msgData;
} FishingRod;

// The environment of Task_OverworldFish.
struct FishingRodWork {
    u32 state;
    u32 rodItemId;
    BOOL hooked;
    u32 rodType;
    BattleSetup *battleSetup;
    SysTask *rodTask;
};

SysTask *FishingRod_Start(FieldSystem *fieldSystem, u32 rodType, BOOL hooked);
BOOL FishingRod_IsFinished(SysTask *task);
BOOL FishingRod_HasLandedMon(SysTask *task);
void FishingRod_Delete(SysTask *task);
void Task_FishingRod(SysTask *task, void *data);
int FishingRodState_Init(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object);
int FishingRodState_StartCast(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object);
int FishingRodState_Cast(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object);
int FishingRodState_SetupBite(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object);
int FishingRodState_WaitBite(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object);
int FishingRodState_WaitReel(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object);
int FishingRodState_Hooked(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object);
int FishingRodState_ShowLanded(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object);
int FishingRodState_WaitLandedMessage(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object);
int FishingRodState_Landed(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object);
int FishingRodState_ReeledTooSoon(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object);
int FishingRodState_GotAway(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object);
int FishingRodState_SetupNoBite(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object);
int FishingRodState_WaitNoBite(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object);
int FishingRodState_WaitFailMessage(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object);
int FishingRodState_PutAway(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object);
int FishingRodState_WaitPutAway(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object);
int FishingRodState_Finish(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object);
void *FishingRod_AllocZeroed(u32 size);
BOOL FishingRod_IsAPressed(void);
BOOL FishingRod_IsAOrBPressed(void);
void FishingRod_InitMessages(FishingRod *rod);
void FishingRod_FreeMessages(FishingRod *rod);
void FishingRod_OpenMessageBox(FishingRod *rod);
void FishingRod_PrintMessage(FishingRod *rod, u32 msgId);
BOOL FishingRod_IsMessageDismissed(FishingRod *rod);
u32 GetRodItemId(u32 rodType);
BOOL FishingRod_FollowMonNoticesBite(FishingRod *rod);
BOOL FishingRod_RollFriendshipNotice(u8 friendship);
u8 FishingRod_GetMoodReelBonus(FishingRod *rod);

// field/encounter_check.h declares rodType as u8; this caller was built against a full-width parameter (no narrowing).
BOOL FieldSystem_PerformFishEncounterCheck(FieldSystem *fieldSystem, u32 rodType, BattleSetup **pBattleSetup);

// Frames to press A after the bite, per rod.
static const u32 sReelWindow[3] = { 45, 30, 15 };

// Extra reel frames from the following Pokemon's mood, per mood bucket and rod.
static const u32 sMoodReelBonus[5][3] = {
    { 0,  0,  0  },
    { 9,  6,  3  },
    { 15, 12, 6  },
    { 21, 18, 9  },
    { 30, 24, 12 },
};

// Returns nonzero to run the next state in the same frame.
static int (*const sFishingRodStates[])(FishingRod *, PlayerAvatar *, LocalMapObject *) = {
    FishingRodState_Init,
    FishingRodState_StartCast,
    FishingRodState_Cast,
    FishingRodState_SetupBite,
    FishingRodState_WaitBite,
    FishingRodState_WaitReel,
    FishingRodState_Hooked,
    FishingRodState_ShowLanded,
    FishingRodState_WaitLandedMessage,
    FishingRodState_Landed,
    FishingRodState_ReeledTooSoon,
    FishingRodState_GotAway,
    FishingRodState_SetupNoBite,
    FishingRodState_WaitNoBite,
    FishingRodState_WaitFailMessage,
    FishingRodState_PutAway,
    FishingRodState_WaitPutAway,
    FishingRodState_Finish,
};

struct FishingRodWork *CreateFishingRodTaskEnv(FieldSystem *fieldSystem, enum HeapID heapID, int rod) {
    struct FishingRodWork *work;
    u32 n;
    u8 *p;

    work = Heap_AllocAtEnd(heapID, sizeof(struct FishingRodWork));
    p = (u8 *)work;
    n = sizeof(struct FishingRodWork);
    do {
        *p = 0;
        p++;
        n--;
    } while (n != 0);
    work->rodType = rod;
    work->rodItemId = GetRodItemId(rod);
    return work;
}

BOOL Task_OverworldFish(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct FishingRodWork *work = TaskManager_GetEnvironment(taskManager);

    switch (work->state) {
    case 0:
        MapObjectManager_PauseAllMovement(fieldSystem->mapObjectManager);
        work->battleSetup = NULL;
        work->hooked = FieldSystem_PerformFishEncounterCheck(fieldSystem, work->rodType, &work->battleSetup);
        work->rodTask = FishingRod_Start(fieldSystem, work->rodType, work->hooked);
        work->state++;
        break;
    case 1:
        if (FishingRod_IsFinished(work->rodTask) == TRUE) {
            BOOL landed = FishingRod_HasLandedMon(work->rodTask);
            FishingRod_Delete(work->rodTask);
            if (landed == TRUE) {
                GearPhoneRingManager_ResetIfActive(FieldSystem_GetGearPhoneRingManager(fieldSystem));
                GameStats_Inc(Save_GameStats_Get(fieldSystem->saveData), GAME_STAT_FISH_LANDED);
                FieldSystem_StartForcedWildBattle(fieldSystem, taskManager, work->battleSetup);
                Heap_Free(work);
                return FALSE;
            }
            if (work->battleSetup != NULL) {
                BattleSetup_Delete(work->battleSetup);
            }
            MapObjectManager_UnpauseAllMovement(fieldSystem->mapObjectManager);
            Heap_Free(work);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

SysTask *FishingRod_Start(FieldSystem *fieldSystem, u32 rodType, BOOL hooked) {
    FishingRod *rod = FishingRod_AllocZeroed(sizeof(FishingRod));
    rod->fieldSystem = fieldSystem;
    rod->rodType = rodType;
    rod->hooked = hooked;
    return SysTask_CreateOnMainQueue(Task_FishingRod, rod, 0x80);
}

BOOL FishingRod_IsFinished(SysTask *task) {
    FishingRod *rod = SysTask_GetData(task);
    return rod->finished;
}

BOOL FishingRod_HasLandedMon(SysTask *task) {
    FishingRod *rod = SysTask_GetData(task);
    return rod->landed;
}

void FishingRod_Delete(SysTask *task) {
    Heap_Free(SysTask_GetData(task));
    SysTask_Destroy(task);
}

void Task_FishingRod(SysTask *task, void *data) {
    FishingRod *rod = data;
    PlayerAvatar *avatar = rod->fieldSystem->playerAvatar;
    LocalMapObject *object = PlayerAvatar_GetMapObject(avatar);

    while (sFishingRodStates[rod->state](rod, avatar, object) != 0) {
        ;
    }
}

int FishingRodState_Init(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object) {
    FishingRod_InitMessages(rod);
    MapObject_UnpauseMovement(object);
    rod->state = FISHING_ROD_STATE_START_CAST;
    return 1;
}

int FishingRodState_StartCast(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object) {
    if (MapObject_AreBitsSetForMovementScriptInit(object) == TRUE) {
        MapObject_ClearHeldMovementIfActive(object);
        Field_PlayerAvatar_OrrTransitionFlags(avatar, PLAYER_TRANSITION_FISHING);
        Field_PlayerAvatar_ApplyTransitionFlags(avatar);
        MapObject_SetAnimation(object, FISHING_ROD_ANIM_CAST);
        rod->state = FISHING_ROD_STATE_CAST;
    }
    return 0;
}

int FishingRodState_Cast(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object) {
    rod->timer++;
    if (rod->timer == 10) {
        PlaySE(SEQ_SE_DP_FW104);
    }
    if (rod->timer < 34) {
        return 0;
    }
    if (rod->hooked == TRUE) {
        rod->state = FISHING_ROD_STATE_SETUP_BITE;
    } else {
        rod->state = FISHING_ROD_STATE_SETUP_NO_BITE;
    }
    rod->timer = 0;
    return 1;
}

int FishingRodState_SetupBite(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object) {
    rod->biteDelay = (LCRandom() % 4 + 1) * 30;
    rod->reelWindow = sReelWindow[rod->rodType];
    rod->reelWindow += FishingRod_GetMoodReelBonus(rod);
    rod->state = FISHING_ROD_STATE_WAIT_BITE;
    return 1;
}

int FishingRodState_WaitBite(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object) {
    rod->biteDelay--;
    if (FishingRod_IsAPressed() == TRUE) {
        rod->state = FISHING_ROD_STATE_REELED_TOO_SOON;
        return 1;
    }
    if (rod->biteDelay > 0) {
        return 0;
    }
    MapObject_SetAnimation(object, FISHING_ROD_ANIM_BITE);
    if (FishingRod_FollowMonNoticesBite(rod)) {
        rod->emoteMark = EmoteMark_Spawn(FollowMon_GetMapObject(rod->fieldSystem), 0, 1);
    } else {
        rod->emoteMark = EmoteMark_Spawn(object, 0, 1);
    }
    rod->state = FISHING_ROD_STATE_WAIT_REEL;
    return 1;
}

int FishingRodState_WaitReel(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object) {
    rod->reelWindow--;
    if (FishingRod_IsAPressed() == TRUE) {
        rod->state = FISHING_ROD_STATE_HOOKED;
        return 1;
    }
    if (rod->reelWindow > 0) {
        return 0;
    }
    rod->state = FISHING_ROD_STATE_GOT_AWAY;
    return 0;
}

int FishingRodState_Hooked(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object) {
    EmoteMark_Hide(rod->emoteMark);
    MapObject_SetAnimation(object, FISHING_ROD_ANIM_REEL);
    rod->timer = 0;
    rod->state = FISHING_ROD_STATE_SHOW_LANDED;
    return 0;
}

int FishingRodState_ShowLanded(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object) {
    rod->timer++;
    if (rod->emoteMark != 0) {
        FieldEffect_Destroy(rod->emoteMark);
        rod->emoteMark = 0;
    }
    if (rod->timer > 15) {
        rod->timer = 0;
        rod->state = FISHING_ROD_STATE_WAIT_LANDED_MESSAGE;
        FishingRod_PrintMessage(rod, msg_0040_00052);
    }
    return 0;
}

int FishingRodState_WaitLandedMessage(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object) {
    if (FishingRod_IsMessageDismissed(rod) == FALSE) {
        return 0;
    }
    rod->state = FISHING_ROD_STATE_LANDED;
    return 1;
}

int FishingRodState_Landed(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object) {
    rod->landed = TRUE;
    rod->state = FISHING_ROD_STATE_PUT_AWAY;
    return 1;
}

int FishingRodState_ReeledTooSoon(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object) {
    MapObject_SetAnimation(object, FISHING_ROD_ANIM_IDLE);
    FishingRod_PrintMessage(rod, msg_0040_00051);
    rod->timer = 16;
    rod->state = FISHING_ROD_STATE_WAIT_FAIL_MESSAGE;
    return 1;
}

int FishingRodState_GotAway(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object) {
    MapObject_SetAnimation(object, FISHING_ROD_ANIM_IDLE);
    FishingRod_PrintMessage(rod, msg_0040_00050);
    rod->timer = 16;
    rod->state = FISHING_ROD_STATE_WAIT_FAIL_MESSAGE;
    GameStats_Inc(Save_GameStats_Get(rod->fieldSystem->saveData), GAME_STAT_FISH_GOT_AWAY);
    return 1;
}

int FishingRodState_SetupNoBite(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object) {
    rod->timer = 120;
    rod->state = FISHING_ROD_STATE_WAIT_NO_BITE;
    return 1;
}

int FishingRodState_WaitNoBite(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object) {
    rod->timer--;
    if (FishingRod_IsAPressed() == TRUE) {
        rod->state = FISHING_ROD_STATE_REELED_TOO_SOON;
        return 1;
    }
    if (rod->timer != 0) {
        return 0;
    }
    MapObject_SetAnimation(object, FISHING_ROD_ANIM_IDLE);
    FishingRod_PrintMessage(rod, msg_0040_00049);
    rod->timer = 16;
    rod->state = FISHING_ROD_STATE_WAIT_FAIL_MESSAGE;
    return 0;
}

int FishingRodState_WaitFailMessage(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object) {
    rod->timer++;
    if (rod->timer < 16) {
        return 0;
    }
    rod->timer = 16;
    if (FishingRod_IsMessageDismissed(rod) == FALSE) {
        return 0;
    }
    rod->state = FISHING_ROD_STATE_PUT_AWAY;
    return 1;
}

int FishingRodState_PutAway(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object) {
    if (rod->emoteMark != 0) {
        EmoteMark_Hide(rod->emoteMark);
    }
    FishingRod_FreeMessages(rod);
    Field_PlayerAvatar_OrrTransitionFlags(avatar, PlayerAvatar_GetTransitionBits(PlayerAvatar_GetState(avatar)));
    Field_PlayerAvatar_ApplyTransitionFlags(avatar);
    rod->timer = 0;
    rod->state = FISHING_ROD_STATE_WAIT_PUT_AWAY;
    return 1;
}

int FishingRodState_WaitPutAway(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object) {
    rod->timer++;
    if (rod->timer > 2) {
        rod->state = FISHING_ROD_STATE_FINISH;
    }
    return 0;
}

int FishingRodState_Finish(FishingRod *rod, PlayerAvatar *avatar, LocalMapObject *object) {
    if (rod->emoteMark != 0) {
        FieldEffect_Destroy(rod->emoteMark);
        rod->emoteMark = 0;
    }
    rod->finished = TRUE;
    return 0;
}

void *FishingRod_AllocZeroed(u32 size) {
    void *ptr = Heap_AllocAtEnd(HEAP_ID_FIELD1, size);
    if (ptr == NULL) {
        GF_AssertFail();
    }
    memset(ptr, 0, size);
    return ptr;
}

BOOL FishingRod_IsAPressed(void) {
    return (gSystem.newKeys & PAD_BUTTON_A) != 0;
}

BOOL FishingRod_IsAOrBPressed(void) {
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        return TRUE;
    }
    return FALSE;
}

void FishingRod_InitMessages(FishingRod *rod) {
    rod->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0040_bin, HEAP_ID_FIELD1);
    rod->string = String_New(1024, HEAP_ID_FIELD1);
    rod->template = String_New(1024, HEAP_ID_FIELD1);
    rod->msgFmt = MessageFormat_New_Custom(8, 64, HEAP_ID_FIELD1);
}

void FishingRod_FreeMessages(FishingRod *rod) {
    MessageFormat_Delete(rod->msgFmt);
    String_Delete(rod->string);
    String_Delete(rod->template);
    DestroyMsgData(rod->msgData);
}

void FishingRod_OpenMessageBox(FishingRod *rod) {
    FieldSystem *fieldSystem = rod->fieldSystem;
    DialogBox_AddWindowToLayer3(fieldSystem->bgConfig, &rod->window, GF_BG_LYR_MAIN_3);
    DialogBox_LoadFrame(&rod->window, Save_PlayerData_GetOptionsAddr(fieldSystem->saveData));
}

void FishingRod_PrintMessage(FishingRod *rod, u32 msgId) {
    FieldSystem *fieldSystem;
    FishingRod_OpenMessageBox(rod);
    fieldSystem = rod->fieldSystem;
    ReadMsgDataIntoString(rod->msgData, msgId, rod->template);
    StringExpandPlaceholders(rod->msgFmt, rod->string, rod->template);
    rod->printerId = DialogBox_PrintMessage(&rod->window, rod->string, Save_PlayerData_GetOptionsAddr(fieldSystem->saveData), TRUE);
}

BOOL FishingRod_IsMessageDismissed(FishingRod *rod) {
    if (DialogBox_IsPrintFinished(rod->printerId) == TRUE) {
        if (FishingRod_IsAOrBPressed() == TRUE) {
            ClearFrameAndWindow2(&rod->window, FALSE);
            RemoveWindow(&rod->window);
            return TRUE;
        }
    }
    return FALSE;
}

u32 GetRodItemId(u32 rodType) {
    switch (rodType) {
    default:
        GF_AssertFail();
    case ROD_TYPE_OLD:
        return ITEM_OLD_ROD;
    case ROD_TYPE_GOOD:
        return ITEM_GOOD_ROD;
    case ROD_TYPE_SUPER:
        return ITEM_SUPER_ROD;
    }
}

// Whether the following Pokemon shows the "!" instead of the player (not while surfing).
BOOL FishingRod_FollowMonNoticesBite(FishingRod *rod) {
    if (PlayerAvatar_GetState(rod->fieldSystem->playerAvatar) == PLAYER_STATE_SURFING) {
        return FALSE;
    }
    if (FollowMon_IsActive(rod->fieldSystem)) {
        u32 friendship = GetMonData(GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(rod->fieldSystem->saveData)), MON_DATA_FRIENDSHIP, NULL);
        if (FishingRod_RollFriendshipNotice((u8)friendship)) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL FishingRod_RollFriendshipNotice(u8 friendship) {
    s32 chance;
    if (friendship <= 99) {
        return FALSE;
    }
    if (friendship <= 149) {
        chance = 20;
    } else if (friendship <= 199) {
        chance = 30;
    } else if (friendship <= 249) {
        chance = 40;
    } else {
        chance = 50;
    }
    if (LCRandom() % 100 < chance) {
        return TRUE;
    }
    return FALSE;
}

u8 FishingRod_GetMoodReelBonus(FishingRod *rod) {
    s32 mood;
    u32 bucket;
    u8 rodType;

    if (FollowMon_IsActive(rod->fieldSystem)) {
        mood = FollowMonMood_Get(rod->fieldSystem->followMonMood);
        if (mood <= -10) {
            bucket = 0;
        } else if (mood >= -9 && mood <= 9) {
            bucket = 1;
        } else if (mood >= 10 && mood < 50) {
            bucket = 2;
        } else if (mood >= 50 && mood < 100) {
            bucket = 3;
        } else {
            bucket = 4;
        }
        rodType = (u8)rod->rodType;
        if (rodType > ROD_TYPE_SUPER) {
            GF_AssertFail();
            return 0;
        }
        return (u8)sMoodReelBonus[bucket][rodType];
    }
    return 0;
}
