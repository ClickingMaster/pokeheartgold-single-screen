#include "wifi_plaza_player_state.h"

#include <nitro/fx/fx_trig.h>
#include <string.h>

#include "global.h"

#include "assert.h"
#include "heap.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "math_util.h"
#include "sound_playback.h"
#include "system.h"
#include "wifi_plaza_camera.h"
#include "wifi_plaza_map.h"
#include "wifi_plaza_obj.h"

// Where and how the own player re-enters the room (room args, see ov49_02259FEC); entryMode picks the own
// player's first handler from sWiFiPlazaPlayerStateOwnEntries
typedef struct WiFiPlazaRoomEntry {
    u16 x; // cell
    u16 y;
    u16 direction;
    u16 entryMode;
    u16 param;
} WiFiPlazaRoomEntry;

// Work of WiFiPlazaPlayerState_OwnArrival
typedef struct WiFiPlazaOwnArrivalWork {
    u16 nextStep;
    u16 listValue;
    u32 timer;
    ListMenuTemplate menuTemplate;
    u16 firstChoice;
    u16 secondChoice;
} WiFiPlazaOwnArrivalWork;

// Work of ov49_02260428
typedef struct WiFiPlazaAttractionWork {
    int timer;
    u16 msgId;
    u16 msgBank;
    int mode;
} WiFiPlazaAttractionWork;

// Work of WiFiPlazaPlayerState_OwnRideMover; the WiFiPlazaPlayerRide_* helpers use it
typedef struct WiFiPlazaPlayerRide {
    int mover; // packed mover index / side (ov45_0222AE08)
    u16 nextStep;
    u16 choice;
    s16 timer;
    u8 resumeStep;
    u8 ticks;
    WiFiPlazaObj *obj;
    WiFiPlazaPlayerBounce bounce;
} WiFiPlazaPlayerRide;

// Work of ov49_02261930
typedef struct WiFiPlazaOtherRideWork {
    WiFiPlazaObj *obj;
    int mover;
    int side;
    fx32 startY;
    fx32 x;
    int frame;
    WiFiPlazaPlayerBounce bounce;
} WiFiPlazaOtherRideWork;

// Arg of ov49_02261B74 (written by ov49_02261DBC): another player's cell before it moves to a lobby activity
typedef struct WiFiPlazaPlacement {
    u8 x; // cell
    u8 y;
    u8 kind;            // lobby activity of the player (ov45_0222A920)
    u8 besideOwnPlayer; // placed next to the own player
    u8 justAppeared;    // the object was just created: drop it in without rising out first
} WiFiPlazaPlacement;

// Arg of ov49_022607C4's pushed handler (ov49_02262028)
typedef struct WiFiPlazaPlayerStateResult {
    u8 result;
    u8 unused;
    u8 param;
} WiFiPlazaPlayerStateResult;

void WiFiPlazaPlayerRide_FollowCamera(WiFiPlazaPlayerRide *ride, WiFiPlazaCamera *camera);
BOOL WiFiPlazaPlayerRide_IsMoverHere(WiFiPlazaPlayerRide *ride, void *models);
BOOL WiFiPlazaPlayerRide_JumpOn(WiFiPlazaPlayerRide *ride, void *models, WiFiPlazaCamera *camera);
BOOL WiFiPlazaPlayerRide_Land(WiFiPlazaPlayerRide *ride, void *models);
void WiFiPlazaPlayerRide_FollowMover(WiFiPlazaPlayerRide *ride, void *models, WiFiPlazaRoomWork *room);
void WiFiPlazaPlayerRide_FollowMoverBounce(WiFiPlazaPlayerRide *ride, void *models, WiFiPlazaRoomWork *room);
void WiFiPlazaPlayerState_InitArrivalMenu(WiFiPlazaOwnArrivalWork *work, WiFiPlazaRoomWork *room);
void WiFiPlazaPlayerState_FreeArrivalMenu(WiFiPlazaOwnArrivalWork *work, WiFiPlazaRoomWork *room);
BOOL ov49_02261DBC(WiFiPlazaPlacement *placement, WiFiPlazaWork *lobby, WiFiPlazaRoomWork *room, WiFiPlazaObj *obj, u8 kind, u32 justAppeared);

// The tile attribute tests of wifi_plaza_map.h, called with the attribute unnarrowed
typedef BOOL (*WiFiPlazaTileTest)(u32 attr);
// ov49_02258EAC called with the direction unnarrowed
typedef void (*WiFiPlazaObjMoveFunc)(WiFiPlazaObjManager *manager, WiFiPlazaObj *obj, u16 command, u32 direction);

extern const WiFiPlazaPlayerStateHandlers sWiFiPlazaPlayerStateOwnWalk;
extern const WiFiPlazaPlayerStateHandlers sWiFiPlazaPlayerStateOtherPresent;
extern const WiFiPlazaPlayerStateHandlers sWiFiPlazaPlayerStateOtherLeave;
extern const WiFiPlazaPlayerStateHandlers ov49_02269B50;
extern const WiFiPlazaPlayerStateHandlers ov49_02269B58;
extern const WiFiPlazaPlayerStateHandlers ov49_02269B60;
extern const WiFiPlazaPlayerStateHandlers ov49_02269B68;
extern const WiFiPlazaPlayerStateHandlers ov49_02269B70;
extern const WiFiPlazaPlayerStateHandlers sWiFiPlazaPlayerStateOtherWait;
extern const WiFiPlazaPlayerStateHandlers ov49_02269B80;
extern const WiFiPlazaPlayerStateHandlers ov49_02269B88;
extern const u32 ov49_02269B90[];
extern const u32 ov49_02269BA0[];
extern const u32 ov49_02269BB0[];
extern const u32 ov49_02269BC0[];
extern const u32 ov49_02269BD0[];
extern const WiFiPlazaPlayerStateHandlers sWiFiPlazaPlayerStateOwnEntries[4];
extern const ListMenuTemplate sWiFiPlazaArrivalMenuTemplate;
extern const u32 ov49_02269C20[];
extern const u32 ov49_02269C40[];
extern const WiFiPlazaTileAction sWiFiPlazaWalkUpTileActions[];
extern const WiFiPlazaTileAction sWiFiPlazaCheckTileActions[];
extern const WiFiPlazaTileAction sWiFiPlazaStandTileActions[];

WiFiPlazaPlayerStateManager *WiFiPlazaPlayerState_CreateManager(WiFiPlazaRoomWork *room, enum HeapID heapID) {
    int i;
    WiFiPlazaPlayerStateManager *manager = Heap_Alloc(heapID, sizeof(WiFiPlazaPlayerStateManager));
    memset(manager, 0, sizeof(WiFiPlazaPlayerStateManager));
    manager->heapID = heapID;
    manager->room = room;
    for (i = 0; i < WIFI_PLAZA_PLAYER_STATE_COUNT; i++) {
        WiFiPlazaPlayerState_Init(&manager->states[i], i, heapID);
    }
    WiFiPlazaPlayerState_Init(&manager->exclusive, 0, heapID);
    return manager;
}

void WiFiPlazaPlayerState_DeleteManager(WiFiPlazaPlayerStateManager *manager) {
    int i;
    for (i = 0; i < WIFI_PLAZA_PLAYER_STATE_COUNT; i++) {
        WiFiPlazaPlayerState_Clear(&manager->states[i]);
    }
    WiFiPlazaPlayerState_Clear(&manager->exclusive);
    Heap_Free(manager);
}

void WiFiPlazaPlayerState_Update(WiFiPlazaPlayerStateManager *manager) {
    WiFiPlazaPlayerState_RunAll(manager, 0);
}

void WiFiPlazaPlayerState_LateUpdate(WiFiPlazaPlayerStateManager *manager) {
    WiFiPlazaPlayerState_RunAll(manager, 1);
}

void *WiFiPlazaPlayerState_GetArg(WiFiPlazaPlayerState *state) {
    return state->current.arg;
}

void *WiFiPlazaPlayerState_AllocWork(WiFiPlazaPlayerState *state, u32 size) {
    GF_ASSERT(state->current.work == NULL);
    state->current.work = Heap_Alloc((enum HeapID)state->heapID, size);
    memset(state->current.work, 0, size);
    return state->current.work;
}

void WiFiPlazaPlayerState_FreeWork(WiFiPlazaPlayerState *state) {
    GF_ASSERT(state->current.work != NULL);
    Heap_Free(state->current.work);
    state->current.work = NULL;
}

void *WiFiPlazaPlayerState_GetWork(WiFiPlazaPlayerState *state) {
    return state->current.work;
}

u32 WiFiPlazaPlayerState_GetStep(WiFiPlazaPlayerState *state) {
    return state->current.step;
}

void WiFiPlazaPlayerState_SetStep(WiFiPlazaPlayerState *state, u32 step) {
    state->current.step = step;
}

void WiFiPlazaPlayerState_NextStep(WiFiPlazaPlayerState *state) {
    state->current.step++;
}

void WiFiPlazaPlayerState_Set(WiFiPlazaPlayerStateManager *manager, u32 index, const WiFiPlazaPlayerStateHandlers *handlers, void *arg) {
    if (manager == NULL) {
        GF_AssertFail();
    }
    if (index >= WIFI_PLAZA_PLAYER_STATE_COUNT) {
        GF_AssertFail();
    }
    WiFiPlazaPlayerState_SetFrame(&manager->states[index], handlers, arg);
}

void WiFiPlazaPlayerState_Push(WiFiPlazaPlayerStateManager *manager, u32 index, const WiFiPlazaPlayerStateHandlers *handlers, void *arg) {
    if (manager == NULL) {
        GF_AssertFail();
    }
    if (index >= WIFI_PLAZA_PLAYER_STATE_COUNT) {
        GF_AssertFail();
    }
    WiFiPlazaPlayerState_PushFrame(&manager->states[index], handlers, arg);
}

void WiFiPlazaPlayerState_SetActive(WiFiPlazaPlayerStateManager *manager, u32 index, u8 active) {
    if (manager == NULL) {
        GF_AssertFail();
    }
    if (index >= WIFI_PLAZA_PLAYER_STATE_COUNT) {
        GF_AssertFail();
    }
    WiFiPlazaPlayerState_SetActiveFlag(&manager->states[index], active);
}

void WiFiPlazaPlayerState_RunAll(WiFiPlazaPlayerStateManager *manager, u32 which) {
    int i;
    WiFiPlazaPlayerState *state;
    WiFiPlazaPlayerState *running;

    if (WiFiPlazaPlayerState_HasHandler(&manager->exclusive) == TRUE) {
        WiFiPlazaPlayerState_Run(&manager->exclusive, manager, which, 0);
        return;
    }
    state = manager->states;
    i = 0;
    running = state;
    for (; i < WIFI_PLAZA_PLAYER_STATE_COUNT; i++, state++, running++) {
        if (WiFiPlazaPlayerState_HasHandler(state) == TRUE) {
            WiFiPlazaPlayerState_Run(running, manager, which, i);
        }
    }
}

void WiFiPlazaPlayerState_Init(WiFiPlazaPlayerState *state, u32 index, u32 heapID) {
    state->heapID = heapID;
    state->active = TRUE;
    state->index = index;
}

void WiFiPlazaPlayerState_Clear(WiFiPlazaPlayerState *state) {
    if (state->current.work != NULL) {
        Heap_Free(state->current.work);
    }
    if (state->saved.work != NULL) {
        Heap_Free(state->saved.work);
    }
    memset(state, 0, sizeof(WiFiPlazaPlayerState));
}

void WiFiPlazaPlayerState_Run(WiFiPlazaPlayerState *state, WiFiPlazaPlayerStateManager *manager, u32 which, u32 index) {
    WiFiPlazaPlayerStateFunc func;

    if (state->active == FALSE) {
        return;
    }
    switch (which) {
    case 0:
        func = state->current.handlers->update;
        break;
    case 1:
        func = state->current.handlers->lateUpdate;
        break;
    default:
        GF_AssertFail();
        break;
    }
    if (func != NULL && func(state, manager->room, index) == TRUE) {
        WiFiPlazaPlayerState_PopFrame(state);
    }
}

void WiFiPlazaPlayerState_SetFrame(WiFiPlazaPlayerState *state, const WiFiPlazaPlayerStateHandlers *handlers, void *arg) {
    if (WiFiPlazaPlayerState_IsSavedFree(state) != TRUE) {
        GF_AssertFail();
    }
    if (state->current.work != NULL) {
        GF_AssertFail();
    }
    WiFiPlazaPlayerStateFrame_Init(&state->current, handlers, arg, 0, NULL);
}

void WiFiPlazaPlayerState_SetActiveFlag(WiFiPlazaPlayerState *state, u8 active) {
    state->active = active;
}

void WiFiPlazaPlayerState_PushFrame(WiFiPlazaPlayerState *state, const WiFiPlazaPlayerStateHandlers *handlers, void *arg) {
    GF_ASSERT(WiFiPlazaPlayerState_IsSavedFree(state) == TRUE);
    state->saved = state->current;
    WiFiPlazaPlayerStateFrame_Init(&state->current, handlers, arg, 0, NULL);
}

void WiFiPlazaPlayerState_PopFrame(WiFiPlazaPlayerState *state) {
    GF_ASSERT(state->current.work == NULL);
    state->current = state->saved;
    WiFiPlazaPlayerStateFrame_Clear(&state->saved);
}

BOOL WiFiPlazaPlayerState_IsSavedFree(WiFiPlazaPlayerState *state) {
    return state->saved.handlers == NULL;
}

BOOL WiFiPlazaPlayerState_HasHandler(WiFiPlazaPlayerState *state) {
    return state->current.handlers != NULL;
}

void WiFiPlazaPlayerStateFrame_Init(WiFiPlazaPlayerStateFrame *frame, const WiFiPlazaPlayerStateHandlers *handlers, void *arg, u32 step, void *work) {
    frame->handlers = handlers;
    frame->step = step;
    frame->work = work;
    frame->arg = arg;
}

void WiFiPlazaPlayerStateFrame_Clear(WiFiPlazaPlayerStateFrame *frame) {
    frame->handlers = NULL;
    frame->step = 0;
    frame->work = NULL;
    frame->arg = NULL;
}

void WiFiPlazaPlayerState_InitOwn(WiFiPlazaRoomWork *room) {
    WiFiPlazaRoomEntry *entry = ov49_02259FEC(room);
    WiFiPlazaPlayerStateManager *manager = ov49_0225A010(room);
    int index = ov49_0225A02C(room);
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    ov45_0222A5E8(lobby, 1);
    GF_ASSERT(entry->entryMode < 4);
    WiFiPlazaPlayerState_Set(manager, index, &sWiFiPlazaPlayerStateOwnEntries[entry->entryMode], NULL);
}

void WiFiPlazaPlayerState_InitOthers(WiFiPlazaRoomWork *room) {
    int i;
    int ownIndex = ov49_0225A02C(room);
    WiFiPlazaPlayerStateManager *manager = ov49_0225A010(room);
    for (i = 0; i < WIFI_PLAZA_PLAYER_STATE_COUNT; i++) {
        if (ownIndex != i) {
            WiFiPlazaPlayerState_Set(manager, i, &sWiFiPlazaPlayerStateOtherWait, NULL);
        }
    }
}

// direction: 0 up, 1 down, 2 left, 3 right
BOOL WiFiPlazaPlayerState_IsDirectionHeld(int direction) {
    int mask;
    switch (direction) {
    case 0:
        mask = PAD_KEY_UP;
        break;
    case 1:
        mask = PAD_KEY_DOWN;
        break;
    case 2:
        mask = PAD_KEY_LEFT;
        break;
    case 3:
        mask = PAD_KEY_RIGHT;
        break;
    }
    return (gSystem.heldKeys & mask) ? TRUE : FALSE;
}

// Starts the action of the first table entry listing `tile`
void WiFiPlazaPlayerState_RunTileAction(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index, const WiFiPlazaTileAction *actions, u32 tile) {
    void *work = WiFiPlazaPlayerState_GetWork(state);
    WiFiPlazaPlayerStateManager *manager = ov49_0225A010(room);
    int i;
    int j;

    for (i = 0; actions[i].action != WIFI_PLAZA_TILE_ACTION_END; i++) {
        for (j = 0; j < 8; j++) {
            if (actions[i].tiles[j] == 0xFF) {
                break;
            }
            if (actions[i].tiles[j] == tile) {
                switch (actions[i].action) {
                case WIFI_PLAZA_TILE_ACTION_SET:
                    WiFiPlazaPlayerState_FreeWork(state);
                    WiFiPlazaPlayerState_Set(manager, index, &actions[i].handlers, NULL);
                    return;
                case WIFI_PLAZA_TILE_ACTION_PUSH:
                    if (actions[i].callback != NULL) {
                        actions[i].callback(work, room, index, actions[i].tiles[j]);
                    }
                    WiFiPlazaPlayerState_Push(manager, index, &actions[i].handlers, NULL);
                    return;
                default:
                    return;
                }
            }
        }
    }
    GF_AssertFail();
}

WiFiPlazaObj *WiFiPlazaPlayerState_CreateOwnPlayer(WiFiPlazaRoomWork *room, int id) {
    VecFx32 pos;
    WiFiPlazaObj *obj = WiFiPlazaObj_CreateOwnPlayer(WiFiPlazaRoom_GetObjManager(room), id);
    WiFiPlazaCamera *camera = ov49_0225A008(room);
    WiFiPlazaObj_GetPosition(obj, &pos);
    WiFiPlazaCamera_SetTargetWithOffset(camera, pos.x, pos.y, pos.z);
    return obj;
}

void WiFiPlazaPlayerState_CreateOwnPlayerAt(WiFiPlazaRoomWork *room, int id, int x, int y, s32 direction) {
    WiFiPlazaObjManager *objManager = WiFiPlazaRoom_GetObjManager(room);
    WiFiPlazaCamera *camera = ov49_0225A008(room);
    WiFiPlazaObj *obj = WiFiPlazaObj_CreateOwnPlayerAt(objManager, id, x, y);
    WiFiPlazaObj_QueueCommand(objManager, obj, 0, direction);
    WiFiPlazaCamera_SetFollowObject(camera, obj);
}

void WiFiPlazaPlayerBounce_Start(WiFiPlazaPlayerBounce *bounce) {
    bounce->offset = 0;
    bounce->frame = 0;
    bounce->state = 1;
    bounce->timer = MTRandom() & 3;
    bounce->repeat = TRUE;
}

#pragma force_active on

fx32 WiFiPlazaPlayerBounce_Update(WiFiPlazaPlayerBounce *bounce) {
    switch (bounce->state) {
    case 0:
        bounce->offset = 0;
        break;
    case 1: {
        int timer = bounce->timer - 1;
        if (timer > 0) {
            bounce->timer = timer;
        } else {
            bounce->state = 2;
        }
        break;
    }
    case 2: {
        int angle;
        bounce->frame++;
        angle = bounce->frame * 0x7FFF / 4;
        bounce->offset = (fx32)(((s64)FX_SinCosTable_[((u16)angle >> 4) * 2] * FX32_CONST(6) + 0x800) >> 12);
        if (bounce->frame >= 4) {
            if (bounce->repeat == TRUE) {
                bounce->frame = 0;
                bounce->state = 1;
                bounce->timer = MTRandom() & 3;
            } else {
                bounce->state = 0;
            }
        }
        break;
    }
    }
    return bounce->offset;
}

#pragma force_active reset

void WiFiPlazaPlayerBounce_Stop(WiFiPlazaPlayerBounce *bounce) {
    bounce->repeat = FALSE;
}

BOOL WiFiPlazaPlayerBounce_IsActive(WiFiPlazaPlayerBounce *bounce) {
    return bounce->state != 0;
}

BOOL ov49_0225F448(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    struct {
        WiFiPlazaObj *obj;
        WiFiPlazaWork *lobby;
    } volatile temp;

    WiFiPlazaObjManager *objManager;

    objManager = WiFiPlazaRoom_GetObjManager(room);
    temp.obj = WiFiPlazaObj_FindById(objManager, index);
    temp.lobby = WiFiPlazaRoom_GetLobby(room);

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        ov49_0225A56C(room, index);
        WiFiPlazaPlayerState_SetStep(state, 1);
        if (index == ov45_0222A53C(temp.lobby)) {
            ov49_0225A53C(room, 0);
        }
        break;
    case 1:
        if (ov49_0225A5AC(room, index) == TRUE) {
            WiFiPlazaPlayerState_SetStep(state, 2);
        }
        break;
    case 2:
        if (index == ov45_0222A53C(temp.lobby)) {
            WiFiPlazaObj_SetHandler(objManager, temp.obj, 1);
        } else {
            if (ov49_0225A4F0(room) == TRUE) {
                if (index != ov49_0225A4E0(room)) {
                    WiFiPlazaObj_SetHandler(objManager, temp.obj, 2);
                }
            } else {
                WiFiPlazaObj_SetHandler(objManager, temp.obj, 2);
            }
            ov49_0225A04C(room, (u8)index, 0);
        }
        ov49_0225A54C(room);
        return TRUE;
    default:
        break;
    }

    return FALSE;
}

// First entry of the own player: it walks in, then two list menus pick two values for the lobby
BOOL WiFiPlazaPlayerState_OwnArrival(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaOwnArrivalWork *work;
    WiFiPlazaWork *lobby;
    WiFiPlazaObjManager *objManager;
    WiFiPlazaObj *ownPlayer;
    u32 choice;
    String *message;
    BOOL done;

    lobby = WiFiPlazaRoom_GetLobby(room);
    objManager = WiFiPlazaRoom_GetObjManager(room);
    ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
    work = WiFiPlazaPlayerState_GetWork(state);

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        work = WiFiPlazaPlayerState_AllocWork(state, sizeof(WiFiPlazaOwnArrivalWork));
        WiFiPlazaPlayerState_InitArrivalMenu(work, room);
        ov45_0222A4D0(lobby);
        work->listValue = ov45_0222B1B4(lobby);
        ov45_0222A72C(lobby, work->listValue);
        WiFiPlazaObj_SetHandler(objManager, WiFiPlazaPlayerState_CreateOwnPlayer(room, index), 4);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case 1:
        if (ov49_0225A030(room) == 0 && WiFiPlazaObj_IsFinished(ownPlayer) == TRUE) {
            WiFiPlazaObj_SetHandler(objManager, ownPlayer, 0);
            WiFiPlazaCamera_SetFollowObject(ov49_0225A008(room), ownPlayer);
            work->timer = 16;
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case 2:
        if (--work->timer == 0) {
            ov49_0225A018(room, 1);
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case 3:
        WiFiPlazaObj_QueueCommand(objManager, ownPlayer, 2, 0);
        work->nextStep = 4;
        WiFiPlazaPlayerState_SetStep(state, 7);
        break;
    case 4:
        WiFiPlazaObj_QueueCommand(objManager, ownPlayer, 2, 0);
        work->nextStep = 5;
        WiFiPlazaPlayerState_SetStep(state, 7);
        break;
    case 5:
        WiFiPlazaObj_QueueCommand(objManager, ownPlayer, 1, 3);
        work->nextStep = 6;
        WiFiPlazaPlayerState_SetStep(state, 7);
        break;
    case 6:
        WiFiPlazaObj_QueueCommand(objManager, ownPlayer, 2, 3);
        work->nextStep = 8;
        WiFiPlazaPlayerState_SetStep(state, 7);
        break;
    case 7:
        if (WiFiPlazaObj_GetParam(ownPlayer, 5) == 0) {
            WiFiPlazaPlayerState_SetStep(state, work->nextStep);
        }
        break;
    case 8:
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 1, 0));
        work->nextStep = 16;
        WiFiPlazaPlayerState_SetStep(state, 21);
        break;
    case 9:
        ov49_0225A09C(room, WiFiPlazaRoom_GetMsgString(room, 1, 5));
        ov49_0225A174(room, &work->menuTemplate, 0, 0);
        ov49_0225A1F4(room, 1);
        WiFiPlazaPlayerState_SetStep(state, 10);
        break;
    case 10:
        done = FALSE;
        choice = ov49_0225A1D4(room);
        switch (choice) {
        case LIST_CANCEL:
            PlaySE(SEQ_SE_DP_SELECT);
        case 0:
            work->firstChoice = 0;
            done = TRUE;
            break;
        case LIST_NOTHING_CHOSEN:
            break;
        default:
            work->firstChoice = choice;
            done = TRUE;
            ov49_0225A40C(room, 0, choice);
            break;
        }
        if (done == TRUE) {
            ov49_0225A1E4(room, 0, 0);
            if (work->firstChoice != 0) {
                ov49_0225A09C(room, WiFiPlazaRoom_GetMsgString(room, 1, 8));
                WiFiPlazaRoom_OpenYesNoMenu(room);
                WiFiPlazaPlayerState_SetStep(state, 11);
            } else {
                WiFiPlazaPlayerState_SetStep(state, 17);
            }
        }
        break;
    case 11:
        switch (WiFiPlazaRoom_GetYesNoResult(room)) {
        case 0:
            WiFiPlazaRoom_CloseYesNoMenu(room);
            WiFiPlazaPlayerState_SetStep(state, 12);
            ov45_0222A770(lobby, work->firstChoice, work->secondChoice);
            ov49_0225A490(room);
            PlaySE(SEQ_SE_DP_BUTTON3);
            break;
        case 1:
            WiFiPlazaRoom_CloseYesNoMenu(room);
            WiFiPlazaPlayerState_SetStep(state, 9);
            break;
        }
        break;
    case 12:
        ov49_0225A09C(room, WiFiPlazaRoom_GetMsgString(room, 1, 10));
        ov49_0225A174(room, &work->menuTemplate, 0, 0);
        ov49_0225A1F4(room, 1);
        WiFiPlazaPlayerState_SetStep(state, 13);
        break;
    case 13:
        done = FALSE;
        choice = ov49_0225A1D4(room);
        switch (choice) {
        case LIST_CANCEL:
            PlaySE(SEQ_SE_DP_SELECT);
        case 0:
            work->secondChoice = 0;
            done = TRUE;
            break;
        case LIST_NOTHING_CHOSEN:
            break;
        default:
            work->secondChoice = choice;
            done = TRUE;
            ov49_0225A40C(room, 0, choice);
            break;
        }
        if (done == TRUE) {
            ov49_0225A1E4(room, 0, 0);
            if (work->secondChoice != 0) {
                ov49_0225A09C(room, WiFiPlazaRoom_GetMsgString(room, 1, 8));
                WiFiPlazaRoom_OpenYesNoMenu(room);
                WiFiPlazaPlayerState_SetStep(state, 15);
            } else {
                WiFiPlazaPlayerState_SetStep(state, 19);
            }
        }
        break;
    case 15:
        switch (WiFiPlazaRoom_GetYesNoResult(room)) {
        case 0:
            WiFiPlazaRoom_CloseYesNoMenu(room);
            WiFiPlazaPlayerState_SetStep(state, 20);
            ov45_0222A770(lobby, work->firstChoice, work->secondChoice);
            ov49_0225A490(room);
            PlaySE(SEQ_SE_DP_BUTTON3);
            break;
        case 1:
            WiFiPlazaRoom_CloseYesNoMenu(room);
            WiFiPlazaPlayerState_SetStep(state, 12);
            break;
        }
        break;
    case 16:
        PlaySE(SEQ_SE_PL_EFF03_4);
        ov49_0225A530(room);
        ov49_0225A334(room, index, 0);
        ov49_0225A39C(room, work->listValue, 1);
        if (ov45_0222AB28(lobby, index) == 0) {
            message = WiFiPlazaRoom_GetMsgString(room, 1, 1);
        } else {
            message = WiFiPlazaRoom_GetMsgString(room, 1, 124);
        }
        WiFiPlazaRoom_PrintMessage(room, message);
        work->nextStep = 18;
        WiFiPlazaPlayerState_SetStep(state, 21);
        break;
    case 17:
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 1, 2));
        work->nextStep = 22;
        WiFiPlazaPlayerState_SetStep(state, 21);
        break;
    case 18:
        if (!IsSEPlaying(SEQ_SE_PL_EFF03_4) && (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) != 0) {
            PlaySE(SEQ_SE_DP_SELECT);
            WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 1, 4));
            work->nextStep = 9;
            WiFiPlazaPlayerState_SetStep(state, 21);
            ov45_0222A770(lobby, work->firstChoice, work->secondChoice);
            ov49_0225A428(room, index, 0);
        }
        break;
    case 19:
        ov49_0225A40C(room, 0, work->firstChoice);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 1, 12));
        work->nextStep = 17;
        WiFiPlazaPlayerState_SetStep(state, 21);
        break;
    case 20:
        ov49_0225A40C(room, 0, work->firstChoice);
        ov49_0225A40C(room, 1, work->secondChoice);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 1, 11));
        work->nextStep = 17;
        WiFiPlazaPlayerState_SetStep(state, 21);
        break;
    case 21:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaPlayerState_SetStep(state, work->nextStep);
        }
        break;
    case 22:
        WiFiPlazaObj_SetHandler(objManager, ownPlayer, 1);
        WiFiPlazaRoom_CloseMessage(room);
        WiFiPlazaPlayerState_FreeArrivalMenu(work, room);
        WiFiPlazaPlayerState_FreeWork(state);
        WiFiPlazaPlayerState_Set(ov49_0225A010(room), ov49_0225A02C(room), &sWiFiPlazaPlayerStateOwnWalk, NULL);
        break;
    }
    return FALSE;
}

BOOL ov49_0225FA14(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaObjManager *objManager;
    WiFiPlazaWork *lobby;
    WiFiPlazaRoomEntry *entry;
    WiFiPlazaObj *ownPlayer;

    lobby = WiFiPlazaRoom_GetLobby(room);
    entry = ov49_02259FEC(room);
    objManager = WiFiPlazaRoom_GetObjManager(room);
    ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        WiFiPlazaPlayerState_CreateOwnPlayerAt(room, index, entry->x, entry->y, 1);
        if (ov45_0222D844() == FALSE) {
            ov45_0222EB94();
        }
        ov45_0222A520(lobby, 0);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case 1:
        if (ov45_0222D844() != FALSE || ov45_0222EBC4() != FALSE) {
            entry = ov49_02259FEC(room);
            if (ov49_0225A030(room) == 0) {
                WiFiPlazaPlayerState_NextStep(state);
                ((WiFiPlazaObjMoveFunc)ov49_02258EAC)(objManager, ownPlayer, 2, entry->direction);
            }
        }
        break;
    case 2:
        if (WiFiPlazaObj_GetParam(ownPlayer, 5) == 0) {
            ov49_0225A018(room, 1);
            if (ov45_0222A4B8(lobby) == 1) {
                WiFiPlazaPlayerState_NextStep(state);
            } else {
                WiFiPlazaPlayerState_SetStep(state, 5);
            }
        }
        break;
    case 3:
        ov49_0225A37C(room, entry->param, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 0, 21));
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case 4:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaRoom_CloseMessage(room);
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case 5:
        WiFiPlazaObj_SetHandler(objManager, ownPlayer, 1);
        state = ov49_0225A010(room);
        WiFiPlazaPlayerState_Set((WiFiPlazaPlayerStateManager *)state, ov49_0225A02C(room), &sWiFiPlazaPlayerStateOwnWalk, NULL);
        break;
    }

    return FALSE;
}

BOOL ov49_0225FB5C(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaWork *lobby;
    WiFiPlazaObjManager *objManager;
    WiFiPlazaPlayerStateManager *manager;
    WiFiPlazaRoomEntry *entry;
    WiFiPlazaObj *ownPlayer;

    manager = ov49_0225A010(room);
    objManager = WiFiPlazaRoom_GetObjManager(room);
    ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
    lobby = WiFiPlazaRoom_GetLobby(room);
    entry = ov49_02259FEC(room);

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        WiFiPlazaPlayerState_CreateOwnPlayerAt(room, index, entry->x, entry->y, 1);
        WiFiPlazaPlayerState_NextStep(state);
        WiFiPlazaPlayerState_Push(manager, index, &ov49_02269B68, NULL);
        ov45_0222A4C8(lobby, 0);
        ov45_0222A4D0(lobby);
        break;
    case 1:
        entry = ov49_02259FEC(room);
        if (ov49_0225A030(room) == 0) {
            WiFiPlazaPlayerState_NextStep(state);
            ((WiFiPlazaObjMoveFunc)ov49_02258EAC)(objManager, ownPlayer, 2, entry->direction);
        }
        break;
    case 2:
        if (WiFiPlazaObj_GetParam(ownPlayer, 5) == 0) {
            ov49_0225A018(room, 1);
            if (ov45_0222A424(lobby) == TRUE) {
                WiFiPlazaPlayerState_SetStep(state, 3);
            } else {
                WiFiPlazaPlayerState_SetStep(state, 5);
            }
        }
        break;
    case 3:
        ov49_0225A37C(room, entry->param, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 0, 8));
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case 4:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaRoom_CloseMessage(room);
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case 5:
        WiFiPlazaObj_SetHandler(objManager, ownPlayer, 1);
        state = ov49_0225A010(room);
        WiFiPlazaPlayerState_Set((WiFiPlazaPlayerStateManager *)state, ov49_0225A02C(room), &sWiFiPlazaPlayerStateOwnWalk, NULL);
        break;
    }

    return FALSE;
}

BOOL ov49_0225FCA8(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    WiFiPlazaRoomEntry *entry = ov49_02259FEC(room);
    WiFiPlazaObjManager *objManager = WiFiPlazaRoom_GetObjManager(room);
    WiFiPlazaObj *ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        WiFiPlazaPlayerState_CreateOwnPlayerAt(room, index, entry->x, entry->y, 1);
        ov45_0222A520(lobby, 0);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case 1:
        entry = ov49_02259FEC(room);
        if (ov49_0225A030(room) == 0) {
            WiFiPlazaPlayerState_NextStep(state);
            ((WiFiPlazaObjMoveFunc)ov49_02258EAC)(objManager, ownPlayer, 2, entry->direction);
        }
        break;
    case 2:
        if (WiFiPlazaObj_GetParam(ownPlayer, 5) == 0) {
            ov49_0225A018(room, 1);
            if (ov45_0222A4B8(lobby) == 1) {
                WiFiPlazaPlayerState_NextStep(state);
            } else {
                WiFiPlazaPlayerState_SetStep(state, 5);
            }
        }
        break;
    case 3:
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 1, 91));
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case 4:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaRoom_CloseMessage(room);
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case 5:
        WiFiPlazaObj_SetHandler(objManager, ownPlayer, 1);
        state = ov49_0225A010(room);
        WiFiPlazaPlayerState_Set((WiFiPlazaPlayerStateManager *)state, ov49_0225A02C(room), &sWiFiPlazaPlayerStateOwnWalk, NULL);
        break;
    }

    return FALSE;
}

#pragma force_active on

// The own player walks around: map tile actions, talking to other players, the profile panel (X)
BOOL WiFiPlazaPlayerState_OwnWalk(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    int *savedHandler = WiFiPlazaPlayerState_GetWork(state);
    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        WiFiPlazaPlayerState_AllocWork(state, sizeof(int));
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case 1: {
        int moveState;
        WiFiPlazaObj *ownPlayer;
        int tile;
        void *models;
        WiFiPlazaMap *map;
        WiFiPlazaWork *lobby;
        WiFiPlazaPlayerStateManager *manager;
        int frontY, frontX, direction, frontTile, selected;
        WiFiPlazaObjManager *objManager;
        WiFiPlazaObj *other;
        UnkStruct_ov44_02232914 pos, frontPos;
        ov49_0225A53C(room, 0);
        manager = ov49_0225A010(room);
        lobby = WiFiPlazaRoom_GetLobby(room);
        objManager = WiFiPlazaRoom_GetObjManager(room);
        map = WiFiPlazaRoom_GetMap(room);
        models = WiFiPlazaRoom_GetModels(room);
        other = WiFiPlazaObj_GetSpecial(objManager);
        if (other) {
            if (!ov45_0222A330(lobby) && ov45_0222A3A0(lobby) == TRUE && !ov45_0222A2F8(lobby)) {
                if (WiFiPlazaObj_GetHandler(other) != 9) {
                    WiFiPlazaObj_SetHandler(objManager, other, 9);
                }
            } else if (WiFiPlazaObj_GetHandler(other) != 0) {
                WiFiPlazaObj_SetHandler(objManager, other, 0);
            }
        }
        ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
        moveState = WiFiPlazaObj_GetParam(ownPlayer, 5);
        direction = WiFiPlazaObj_GetParam(ownPlayer, 6);
        pos = ov49_02258E34(ownPlayer);
        frontPos = ov42_02228270(pos, direction);
        tile = WiFiPlazaMap_GetTileAttr(map, (u16)(pos.unk0 / 16), (u16)(pos.unk2 / 16));
        frontY = frontPos.unk2 / 16;
        frontX = frontPos.unk0 / 16;
        frontTile = WiFiPlazaMap_GetTileAttr(map, (u16)frontX, (u16)frontY);
        selected = ov49_0225A4E0(room);
        if (selected != index && ov49_0225A084(room, (u8)selected)) {
            ov49_0225A4D0(room);
        }
        if (moveState) {
            if (ov49_0225A500(room) == TRUE && (u32)(moveState - 1) <= 2) {
                selected = ov49_0225A4E0(room);
                if (selected != index) {
                    other = WiFiPlazaObj_FindById(objManager, selected);
                    if (other && !WiFiPlazaObj_GetHandler(other)) {
                        WiFiPlazaObj_SetHandler(objManager, other, *savedHandler);
                    }
                }
                ov49_0225A4D0(room);
            }
            break;
        }
        ov49_0225A53C(room, 0);
        if (((WiFiPlazaTileTest)ov49_02258A30)(tile) == TRUE) {
            WiFiPlazaPlayerState_RunTileAction(state, room, index, sWiFiPlazaStandTileActions, tile);
            WiFiPlazaObj_SetHandler(objManager, ownPlayer, 0);
            ov49_0225A03C(room, (u8)tile);
            ov49_0225A53C(room, 1);
            return FALSE;
        }
        if (ov49_0225A55C(room) == TRUE) {
            WiFiPlazaObj_SetHandler(objManager, ownPlayer, 0);
            WiFiPlazaPlayerState_Push(manager, index, &ov49_02269B80, NULL);
            ov49_0225A53C(room, 1);
            return FALSE;
        }
        if (ov45_0222B00C(lobby)) {
            WiFiPlazaPlayerState_Push(manager, index, &ov49_02269B70, NULL);
            WiFiPlazaObj_SetHandler(objManager, ownPlayer, 0);
            ov49_0225A53C(room, 1);
            return FALSE;
        }
        if (gSystem.newKeys & PAD_BUTTON_A) {
            WiFiPlazaObj *facing = WiFiPlazaObj_FindFacing(objManager, ownPlayer);
            if (facing) {
                int facingMoveState = WiFiPlazaObj_GetParam(facing, 5);
                int id = WiFiPlazaObj_GetParam(facing, 4);
                if (id != 254) {
                    int busy = ov49_0225A064(room, (u8)id);
                    if (!facingMoveState && !busy) {
                        WiFiPlazaPlayerState_Push(manager, index, &ov49_02269B60, NULL);
                        WiFiPlazaObj_SetHandler(objManager, ownPlayer, 0);
                        WiFiPlazaObj_SetHandler(objManager, facing, 0);
                        ov49_0225A53C(room, 1);
                        return FALSE;
                    }
                }
            }
            if (((WiFiPlazaTileTest)ov49_02258A90)(frontTile) == TRUE && !WiFiPlazaObj_GetParam(ownPlayer, 6)) {
                WiFiPlazaModels_TouchPropsAt(models, (u8)frontX, (u8)frontY);
                ov49_0225A03C(room, (u8)frontTile);
                ov49_0225A53C(room, 1);
                return FALSE;
            }
            if (((WiFiPlazaTileTest)ov49_02258A70)(frontTile) == TRUE) {
                WiFiPlazaPlayerState_RunTileAction(state, room, index, sWiFiPlazaCheckTileActions, frontTile);
                WiFiPlazaObj_SetHandler(objManager, ownPlayer, 0);
                ov49_0225A03C(room, (u8)frontTile);
                ov49_0225A53C(room, 1);
                return FALSE;
            }
        }
        if (WiFiPlazaPlayerState_IsDirectionHeld(direction) == TRUE) {
            WiFiPlazaObj *facing;
            if (!direction && ((WiFiPlazaTileTest)ov49_02258A50)(frontTile) == TRUE) {
                WiFiPlazaPlayerState_RunTileAction(state, room, index, sWiFiPlazaWalkUpTileActions, frontTile);
                WiFiPlazaObj_SetHandler(objManager, ownPlayer, 0);
                ov49_0225A03C(room, (u8)frontTile);
                ov49_0225A53C(room, 1);
                return FALSE;
            }
            facing = WiFiPlazaObj_FindFacing(objManager, ownPlayer);
            if (facing) {
                int id = WiFiPlazaObj_GetParam(facing, 4);
                if (id != 254 && !ov49_0225A4F0(room) && !ov49_0225A084(room, (u8)id)) {
                    ov49_0225A428(room, id, 1);
                    *savedHandler = WiFiPlazaObj_GetHandler(facing);
                    WiFiPlazaObj_SetHandler(objManager, facing, 0);
                    return FALSE;
                }
            }
        }
        if (gSystem.newKeys & PAD_BUTTON_X) {
            if (!ov49_0225A4F0(room)) {
                ov49_0225A428(room, ov45_0222A53C(lobby), 0);
                PlaySE(SEQ_SE_DP_SELECT);
            } else {
                int current = ov49_0225A4E0(room);
                if (current == index) {
                    ov49_0225A4D0(room);
                    PlaySE(SEQ_SE_DP_SELECT);
                }
            }
            return FALSE;
        }
        break;
    }
    }
    return FALSE;
}

#pragma force_active reset

BOOL WiFiPlazaPlayerState_OwnWalkLate(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    if (WiFiPlazaObj_GetParam(WiFiPlazaObj_GetOwnPlayer(WiFiPlazaRoom_GetObjManager(room)), 5)) {
        ov49_0225A53C(room, 1);
    }
    return FALSE;
}

// Exit tile: asks whether to leave the plaza
BOOL WiFiPlazaPlayerState_OwnExit(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaObjManager *objManager;
    WiFiPlazaObj *ownPlayer;
    int closed;

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        objManager = WiFiPlazaRoom_GetObjManager(room);
        ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
        WiFiPlazaObj_SetHandler(objManager, ownPlayer, 0);
        ov45_0222A5E8(WiFiPlazaRoom_GetLobby(room), 11);
        PlaySE(SEQ_SE_DP_DECIDE);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 1, 65));
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case 1:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case 2:
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 2, 25));
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case 3:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case 4:
        ov49_0225A294(room);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case 5:
        closed = FALSE;
        switch (WiFiPlazaRoom_GetYesNoResult(room)) {
        case 0:
            WiFiPlazaPlayerState_SetStep(state, 6);
            closed = TRUE;
            break;
        case 1:
            WiFiPlazaPlayerState_SetStep(state, 8);
            closed = TRUE;
            break;
        case 2:
            break;
        }
        if (closed == TRUE) {
            WiFiPlazaRoom_CloseYesNoMenu(room);
            WiFiPlazaRoom_CloseMessage(room);
        }
        break;
    case 6:
        ov49_0225A018(room, 0);
        objManager = WiFiPlazaRoom_GetObjManager(room);
        ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
        WiFiPlazaCamera_ClearFollowObject(ov49_0225A008(room));
        WiFiPlazaObj_SetHandler(objManager, ownPlayer, 3);
        WiFiPlazaPlayerState_SetStep(state, 7);
        break;
    case 7:
        if (WiFiPlazaObj_IsFinished(WiFiPlazaObj_GetOwnPlayer(WiFiPlazaRoom_GetObjManager(room))) == TRUE) {
            ov49_0225A034(room, 1);
            ov49_0225A038(room, 0);
            ov45_0222A5E8(WiFiPlazaRoom_GetLobby(room), 11);
            return TRUE;
        }
        break;
    case 8:
        objManager = WiFiPlazaRoom_GetObjManager(room);
        ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
        ov49_02258EAC(objManager, ownPlayer, 2, 0);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case 9:
        ov45_0222A5E8(WiFiPlazaRoom_GetLobby(room), 1);
        objManager = WiFiPlazaRoom_GetObjManager(room);
        state = ov49_0225A010(room);
        ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
        if (WiFiPlazaObj_GetParam(ownPlayer, 5) == 0) {
            WiFiPlazaObj_SetHandler(objManager, ownPlayer, 1);
            WiFiPlazaPlayerState_Set((WiFiPlazaPlayerStateManager *)state, index, &sWiFiPlazaPlayerStateOwnWalk, NULL);
        }
        break;
    }
    return FALSE;
}

#pragma force_active on

BOOL ov49_02260428(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaObjManager *objManager = WiFiPlazaRoom_GetObjManager(room);
    WiFiPlazaObj *obj = WiFiPlazaObj_FindById(objManager, index);
    WiFiPlazaWork *lobby;
    int tile = ov49_0225A040(room);
    WiFiPlazaAttractionWork *work = WiFiPlazaPlayerState_GetWork(state);
    lobby = WiFiPlazaRoom_GetLobby(room);
    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0: {
        u8 kind;
        work = WiFiPlazaPlayerState_AllocWork(state, sizeof(WiFiPlazaAttractionWork));
        switch ((u8)ov49_02260C58(tile)) {
        case 0:
            work->mode = 3;
            break;
        case 1:
            work->mode = 4;
            break;
        case 2:
            work->mode = 5;
            break;
        case 3:
            work->mode = 6;
            break;
        default:
            GF_AssertFail();
            work->mode = 5;
            break;
        }
        if (ov45_0222A330(lobby) == 1) {
            work->msgId = 22;
            work->msgBank = 0;
            WiFiPlazaPlayerState_SetStep(state, 5);
        } else if (ov45_0222A208(lobby) == 1) {
            work->msgId = 72;
            work->msgBank = 1;
            WiFiPlazaPlayerState_SetStep(state, 5);
        } else {
            kind = ov49_02260C58(tile);
            if (!ov45_0222EBF0(kind)) {
                work->msgId = 20;
                work->msgBank = 0;
                WiFiPlazaPlayerState_SetStep(state, 3);
            } else {
                if (!ov45_0222EB38(kind)) {
                    GF_AssertFail();
                }
                WiFiPlazaPlayerState_NextStep(state);
            }
        }
        break;
    }
    case 1: {
        int result = ov45_0222EB74();
        if (result == 1) {
            WiFiPlazaWork *lobby2;
            u8 value;
            WiFiPlazaPlayerState_SetStep(state, 2);
            {
                u8 players[4] = { 0, 0, 0, 0 };
                lobby2 = WiFiPlazaRoom_GetLobby(room);
                value = ov45_0222AB68(lobby2);
                if ((u32)(work->mode - 5) <= 1) {
                    int i;
                    u8 count = 0;
                    for (i = 0; i < 4; i++) {
                        int player = ov45_0222AB78(lobby2, i);
                        if (player != -1) {
                            if (count < 4) {
                                players[count] = player;
                            }
                            count++;
                        }
                    }
                } else {
                    players[0] = index;
                }
                ov45_0222AC14(lobby2, work->mode, value, players[0], players[1], players[2], players[3], 0);
            }
        } else if (result == 2) {
            work->msgId = 20;
            work->msgBank = 0;
            WiFiPlazaPlayerState_SetStep(state, 3);
        }
        break;
    }
    case 2: {
        int result = ov49_02260CC0(tile), lobbyState = ov49_02260D28(tile);
        int count, direction;
        WiFiPlazaRoomEntry *entry;
        UnkStruct_ov44_02232914 pos;
        ov49_0225A034(room, 1);
        ov49_0225A038(room, (u8)result);
        ov45_0222A5E8(WiFiPlazaRoom_GetLobby(room), lobbyState);
        switch (work->mode) {
        case 3:
        case 4:
            count = 5;
            break;
        case 5:
            count = 3;
            break;
        case 6:
            count = 4;
            break;
        default:
            GF_AssertFail();
            break;
        }
        ov45_0222B118(lobby, count);
        direction = ov42_022282A4(WiFiPlazaObj_GetParam(obj, 6));
        entry = ov49_02259FEC(room);
        pos = ov49_02258E34(obj);
        entry->entryMode = 1;
        entry->x = pos.unk0 / 16;
        entry->y = pos.unk2 / 16;
        entry->direction = direction;
        entry->param = work->mode;
        WiFiPlazaPlayerState_FreeWork(state);
        return TRUE;
    }
    case 3: {
        int handler;
        switch ((u8)WiFiPlazaObj_GetParam(obj, 6)) {
        case 0:
            handler = 6;
            break;
        case 1:
            handler = 5;
            break;
        case 2:
            handler = 8;
            break;
        case 3:
            handler = 7;
            break;
        default:
            GF_AssertFail();
            break;
        }
        WiFiPlazaObj_SetHandler(objManager, obj, handler);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    }
    case 4:
        if (WiFiPlazaObj_IsFinished(obj) == TRUE) {
            work->timer = 8;
            WiFiPlazaPlayerState_SetStep(state, 7);
        }
        break;
    case 5: {
        u8 direction = ((int (*)(s32))ov42_022282A4)((u8)WiFiPlazaObj_GetParam(obj, 6));
        ov49_02258EAC(objManager, obj, 2, direction);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    }
    case 6:
        if (!WiFiPlazaObj_GetParam(obj, 5)) {
            work->timer = 8;
            WiFiPlazaPlayerState_SetStep(state, 7);
        }
        break;
    case 7:
        if (--work->timer <= 0) {
            ov49_0225A37C(room, work->mode, 0);
            WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, work->msgBank, work->msgId));
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case 8:
        if (WiFiPlazaRoom_IsMessageFinished(room)) {
            WiFiPlazaPlayerState_FreeWork(state);
            WiFiPlazaObj_SetHandler(objManager, obj, 1);
            WiFiPlazaRoom_CloseMessage(room);
            WiFiPlazaPlayerState_Set(ov49_0225A010(room), index, &sWiFiPlazaPlayerStateOwnWalk, NULL);
        }
        break;
    }
    return FALSE;
}

#pragma force_active reset

BOOL ov49_022607C4(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaPlayerStateResult *result = WiFiPlazaPlayerState_GetWork(state);
    WiFiPlazaPlayerStateManager *manager = ov49_0225A010(room);
    WiFiPlazaObjManager *objManager = WiFiPlazaRoom_GetObjManager(room);
    WiFiPlazaObj *ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    int value;
    int choice;

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0: {
        u8 *selection = WiFiPlazaPlayerState_AllocWork(state, 4);
        WiFiPlazaObj_SetHandler(objManager, ownPlayer, 0);
        WiFiPlazaPlayerState_NextStep(state);
        selection[0] = 0;
        switch (ov49_0225A040(room)) {
        case 30:
            selection[1] = 0;
            selection[2] = 0;
            selection[3] = 2;
            break;
        case 31:
            selection[1] = 1;
            selection[2] = 1;
            selection[3] = 3;
            break;
        case 32:
            selection[1] = 2;
            selection[2] = 2;
            selection[3] = 4;
            break;
        default:
            GF_AssertFail();
            break;
        }
        ov45_0222A5E8(WiFiPlazaRoom_GetLobby(room), 12);
        WiFiPlazaPlayerState_Push(manager, index, &ov49_02269B88, selection);
        break;
    }
    case 1:
        if (result->result == 8) {
            WiFiPlazaPlayerState_SetStep(state, 2);
        } else {
            WiFiPlazaPlayerState_SetStep(state, 3);
        }
        break;
    case 2: {
        WiFiPlazaRoomEntry *entry;
        UnkStruct_ov44_02232914 position;
        UnkStruct_ov44_02232914 current;
        u32 packed;
        u32 direction;
        switch (ov49_0225A040(room)) {
        case 30:
            value = 3;
            choice = 0;
            break;
        case 31:
            value = 4;
            choice = 1;
            break;
        case 32:
            value = 5;
            choice = 2;
            break;
        }
        ov49_0225A034(room, 1);
        ov49_0225A038(room, (u8)value);
        IncrementGameStat119(lobby);
        ov45_0222B118(lobby, choice);
        direction = ov42_022282A4(WiFiPlazaObj_GetParam(ownPlayer, 6));
        entry = ov49_02259FEC(room);
        packed = ((u32(*)(WiFiPlazaObj *))ov49_02258E34)(ownPlayer);
        current.unk0 = packed;
        current.unk2 = packed >> 16;
        position = current;
        entry->entryMode = 2;
        entry->x = position.unk0 / 16;
        entry->y = position.unk2 / 16;
        entry->direction = direction;
        entry->param = result->param;
        ov45_0222A4C8(WiFiPlazaRoom_GetLobby(room), 1);
        WiFiPlazaPlayerState_FreeWork(state);
        return TRUE;
    }
    case 3:
        ov49_02258EAC(objManager, ownPlayer, 2, ov42_022282A4(WiFiPlazaObj_GetParam(ownPlayer, 6)));
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case 4:
        if (WiFiPlazaObj_GetParam(ownPlayer, 5) == 0) {
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case 5: {
        BOOL valid = TRUE;
        switch (result->result) {
        case 0:
            ov49_0225A37C(room, result->param, 0);
            value = 4;
            break;
        case 1:
            value = 5;
            break;
        case 2:
            ov49_0225A37C(room, result->param, 0);
            value = 6;
            break;
        case 3:
            value = 19;
            break;
        case 4:
            value = 17;
            break;
        case 6:
            value = 29;
            break;
        case 7:
            value = 7;
            break;
        default:
            valid = FALSE;
            break;
        }
        if (valid) {
            WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 0, value));
            WiFiPlazaPlayerState_NextStep(state);
        } else {
            WiFiPlazaPlayerState_SetStep(state, 7);
        }
        break;
    }
    case 6:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaRoom_CloseMessage(room);
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case 7:
        WiFiPlazaPlayerState_FreeWork(state);
        ov45_0222A5E8(WiFiPlazaRoom_GetLobby(room), 1);
        WiFiPlazaObj_SetHandler(objManager, ownPlayer, 1);
        WiFiPlazaPlayerState_Set(ov49_0225A010(room), index, &sWiFiPlazaPlayerStateOwnWalk, NULL);
        break;
    }
    return FALSE;
}

BOOL ov49_02260A68(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    u16 *work = WiFiPlazaPlayerState_GetWork(state);
    WiFiPlazaObjManager *objManager;
    WiFiPlazaObj *ownPlayer;
    WiFiPlazaWork *lobby;
    WiFiPlazaRoomEntry *entry;
    int value;
    u32 packed;
    UnkStruct_ov44_02232914 position;
    UnkStruct_ov44_02232914 current;

    ov49_0225A010(room);
    objManager = WiFiPlazaRoom_GetObjManager(room);
    ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
    lobby = WiFiPlazaRoom_GetLobby(room);

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0: {
        u16 *newWork = WiFiPlazaPlayerState_AllocWork(state, 12);
        ov45_0222A5E8(WiFiPlazaRoom_GetLobby(room), 13);
        if (ov45_0222A330(lobby) == 1) {
            newWork[0] = 122;
            WiFiPlazaPlayerState_SetStep(state, 2);
        } else if (ov45_0222A3A0(lobby) == TRUE) {
            newWork[1] = 10;
            goto enter;
        } else if (ov45_0222A2E0(lobby) == 1) {
            newWork[0] = 98;
            WiFiPlazaPlayerState_SetStep(state, 2);
        } else {
            newWork[1] = 9;
enter:
            WiFiPlazaPlayerState_SetStep(state, 1);
        }
        break;
    }
    case 1:
        if (work[1] == 9) {
            value = 9;
        } else {
            value = 10;
            ov45_0222A310(lobby);
        }
        ov49_0225A034(room, 1);
        ov49_0225A038(room, (u8)value);
        ov45_0222A704(lobby, 23, -1);
        value = ov42_022282A4(WiFiPlazaObj_GetParam(ownPlayer, 6));
        entry = ov49_02259FEC(room);
        packed = ((u32(*)(WiFiPlazaObj *))ov49_02258E34)(ownPlayer);
        current.unk0 = packed;
        current.unk2 = packed >> 16;
        position = current;
        entry->entryMode = 3;
        entry->x = position.unk0 / 16;
        entry->y = position.unk2 / 16;
        entry->direction = value;
        entry->param = 0;
        WiFiPlazaPlayerState_FreeWork(state);
        return TRUE;
    case 2:
        ov49_02258EAC(objManager, ownPlayer, 2, ov42_022282A4(WiFiPlazaObj_GetParam(ownPlayer, 6)));
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case 3:
        if (WiFiPlazaObj_GetParam(ownPlayer, 5) == 0) {
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case 4:
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 1, work[0]));
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case 5:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaRoom_CloseMessage(room);
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case 6:
        WiFiPlazaPlayerState_FreeWork(state);
        ov45_0222A5E8(WiFiPlazaRoom_GetLobby(room), 1);
        WiFiPlazaObj_SetHandler(objManager, ownPlayer, 1);
        WiFiPlazaPlayerState_Set(ov49_0225A010(room), index, &sWiFiPlazaPlayerStateOwnWalk, NULL);
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

int ov49_02260C58(int tile) {
    switch (tile) {
    case 22:
    case 23:
    case 24:
    case 25:
        return 2;
    case 26:
    case 27:
    case 28:
    case 29:
        return 3;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        return 0;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
        return 1;
    }
    GF_AssertFail();
    return 0;
}

int ov49_02260CC0(int tile) {
    switch (tile) {
    case 22:
    case 23:
    case 24:
    case 25:
        return 1;
    case 26:
    case 27:
    case 28:
    case 29:
        return 2;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        return 6;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
        return 7;
    }
    GF_AssertFail();
    return 0;
}

int ov49_02260D28(int tile) {
    switch (tile) {
    case 22:
    case 23:
    case 24:
    case 25:
        return 7;
    case 26:
    case 27:
    case 28:
    case 29:
        return 8;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        return 5;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
        return 6;
    }
    GF_AssertFail();
    return 0;
}

BOOL ov49_02260D90(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    String *message;
    void *object;
    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        object = WiFiPlazaRoom_GetLobby(room);
        if (ov45_0222A330(object) == 1) {
            message = WiFiPlazaRoom_GetMsgString(room, 1, 3);
        } else if (ov45_0222A374(object) == TRUE) {
            message = WiFiPlazaRoom_GetMsgString(room, 1, 78);
        } else {
            message = WiFiPlazaRoom_GetMsgString(room, 1, 2);
        }
        WiFiPlazaRoom_PrintMessage(room, message);
        PlaySE(SEQ_SE_DP_SELECT);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case 1:
        if (WiFiPlazaRoom_IsMessageFinished(room)) {
            WiFiPlazaRoom_CloseMessage(room);
            object = WiFiPlazaRoom_GetObjManager(room);
            WiFiPlazaObj_SetHandler(object, WiFiPlazaObj_GetOwnPlayer(object), 1);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

#pragma force_active on

// The own player jumps onto a mover and rides it until the room closes
BOOL WiFiPlazaPlayerState_OwnRideMover(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaObjManager *objManager = WiFiPlazaRoom_GetObjManager(room);
    void *models = WiFiPlazaRoom_GetModels(room);
    WiFiPlazaCamera *camera = ov49_0225A008(room);
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    WiFiPlazaPlayerRide *ride = WiFiPlazaPlayerState_GetWork(state);
    VecFx32 pos;
    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        ride = WiFiPlazaPlayerState_AllocWork(state, sizeof(WiFiPlazaPlayerRide));
        ride->obj = WiFiPlazaObj_FindById(objManager, index);
        switch (ov49_0225A040(room)) {
        case 33:
            ride->choice = 2;
            break;
        case 34:
            ride->choice = 1;
            break;
        case 35:
            ride->choice = 0;
            break;
        default:
            GF_AssertFail();
            break;
        }
        WiFiPlazaPlayerState_SetStep(state, 1);
        break;
    case 1:
        if (!ov45_0222A374(lobby)) {
            ride->nextStep = 8;
            WiFiPlazaPlayerState_SetStep(state, 6);
        } else if (ov45_0222A330(lobby) == 1) {
            ride->nextStep = 10;
            WiFiPlazaPlayerState_SetStep(state, 6);
        } else {
            ride->mover = ov45_0222ADB8(lobby, index, ride->choice);
            if (ride->mover != -1) {
                WiFiPlazaPlayerState_SetStep(state, 2);
                ov49_0225A09C(room, WiFiPlazaRoom_GetMsgString(room, 0, 31));
                ov49_0225A0BC(room);
            } else {
                ride->nextStep = 9;
                WiFiPlazaPlayerState_SetStep(state, 6);
            }
        }
        break;
    case 2:
        if ((gSystem.newKeys & PAD_BUTTON_B) || (gSystem.newKeys & PAD_KEY_DOWN)) {
            ov45_0222ADC8(lobby, index);
            ov49_0225A0CC(room);
            ride->resumeStep = 6;
            ride->nextStep = 15;
            WiFiPlazaPlayerState_SetStep(state, 12);
        } else if (WiFiPlazaPlayerRide_IsMoverHere(ride, models) == TRUE) {
            ov49_0225A0CC(room);
            ov45_0222AE44(lobby);
            ride->timer = 0;
            ov45_0222A5E8(WiFiPlazaRoom_GetLobby(room), 10);
            ov49_0225A044(room);
            WiFiPlazaCamera_ClearFollowObject(camera);
            ov49_02259130(ride->obj, 0);
            ov49_0225916C(ride->obj, 1);
            PlaySE(SEQ_SE_PL_DANSA5);
            ov49_0225A510(room);
            ride->resumeStep = 3;
            WiFiPlazaPlayerState_SetStep(state, 12);
        }
        break;
    case 3:
        if (WiFiPlazaPlayerRide_JumpOn(ride, models, camera) == TRUE) {
            ov49_0225916C(ride->obj, 0);
            WiFiPlazaObj_SetDirection(ride->obj, 2);
            ov49_022591B4(ride->obj, 8);
            WiFiPlazaPlayerState_SetStep(state, 4);
            ride->timer = 0;
        }
        break;
    case 4:
        WiFiPlazaPlayerRide_FollowMover(ride, models, room);
        WiFiPlazaPlayerRide_FollowCamera(ride, camera);
        if (WiFiPlazaPlayerRide_Land(ride, models) == TRUE) {
            WiFiPlazaPlayerState_SetStep(state, 5);
            ov49_0225A53C(room, 0);
        }
        break;
    case 5:
        WiFiPlazaPlayerRide_FollowMover(ride, models, room);
        WiFiPlazaPlayerRide_FollowCamera(ride, camera);
        WiFiPlazaObj_GetPosition(ride->obj, &pos);
        WiFiPlazaPlayerRide_FollowMoverBounce(ride, models, room);
        if (pos.x < FX32_CONST(96)) {
            ride->ticks = 0;
            WiFiPlazaPlayerState_SetStep(state, 13);
        }
        break;
    case 6:
        ov49_02258EAC(objManager, ride->obj, 2, 1);
        WiFiPlazaPlayerState_SetStep(state, 7);
        break;
    case 7:
        if (!WiFiPlazaObj_GetParam(ride->obj, 5)) {
            WiFiPlazaPlayerState_SetStep(state, ride->nextStep);
        }
        break;
    case 8:
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 0, 30));
        ride->nextStep = 15;
        WiFiPlazaPlayerState_SetStep(state, 11);
        break;
    case 9:
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 0, 32));
        ride->nextStep = 15;
        WiFiPlazaPlayerState_SetStep(state, 11);
        break;
    case 10:
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 0, 33));
        ride->nextStep = 15;
        WiFiPlazaPlayerState_SetStep(state, 11);
        break;
    case 11:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaPlayerState_SetStep(state, ride->nextStep);
        }
        break;
    case 12:
        WiFiPlazaRoom_CloseMessage(room);
        WiFiPlazaPlayerState_SetStep(state, ride->resumeStep);
        break;
    case 13:
        WiFiPlazaPlayerRide_FollowMover(ride, models, room);
        WiFiPlazaPlayerRide_FollowMoverBounce(ride, models, room);
        ride->ticks++;
        if (ride->ticks > 120) {
            ov49_0225A034(room, 1);
            ov49_0225A038(room, 0);
            WiFiPlazaRoom_CloseMessage(room);
            ov45_0222A5E8(WiFiPlazaRoom_GetLobby(room), 11);
            WiFiPlazaPlayerState_SetStep(state, 14);
        }
        break;
    case 14:
        WiFiPlazaPlayerRide_FollowMover(ride, models, room);
        WiFiPlazaPlayerRide_FollowMoverBounce(ride, models, room);
        break;
    case 15: {
        WiFiPlazaPlayerStateManager *manager = ov49_0225A010(room);
        WiFiPlazaRoom_CloseMessage(room);
        WiFiPlazaObj_SetHandler(objManager, ride->obj, 1);
        WiFiPlazaPlayerState_FreeWork(state);
        WiFiPlazaPlayerState_Set(manager, index, &sWiFiPlazaPlayerStateOwnWalk, NULL);
        break;
    }
    }
    return FALSE;
}

#pragma force_active reset

void WiFiPlazaPlayerRide_FollowCamera(WiFiPlazaPlayerRide *ride, WiFiPlazaCamera *camera) {
    VecFx32 pos;
    WiFiPlazaObj_GetPosition(ride->obj, &pos);
    WiFiPlazaCamera_SetTarget(camera, pos.x, pos.y, pos.z);
}

BOOL WiFiPlazaPlayerRide_IsMoverHere(WiFiPlazaPlayerRide *ride, void *models) {
    VecFx32 pos;
    VecFx32 moverPos;
    u32 mover;
    u32 side;
    ov45_0222AE08(ride->mover, &mover, &side);
    WiFiPlazaModels_GetMoverFrontPos(models, mover, side, &moverPos);
    WiFiPlazaObj_GetPosition(ride->obj, &pos);
    if (pos.x + FX32_ONE * 16 < moverPos.x) {
        return FALSE;
    }
    return TRUE;
}

#pragma force_active on

// Jump arc from the player's cell onto the mover (24 frames); the camera follows
BOOL WiFiPlazaPlayerRide_JumpOn(WiFiPlazaPlayerRide *ride, void *models, WiFiPlazaCamera *camera) {
    VecFx32 start, end, pos;
    u32 mover, side;
    UnkStruct_ov44_02232914 cell, packedCell;
    int done, difference, frame;
    u32 packed;
    ride->timer++;
    if (ride->timer < 24) {
        done = FALSE;
    } else {
        ride->timer = 24;
        done = TRUE;
    }
    ov45_0222AE08(ride->mover, &mover, &side);
    WiFiPlazaModels_GetMoverFrontPos(models, mover, side, &end);
    packed = ((u32(*)(WiFiPlazaObj *))ov49_02258E34)(ride->obj);
    packedCell.unk0 = packed;
    packedCell.unk2 = packed >> 16;
    cell = packedCell;
    WiFiPlazaMap_PosToVecFx32(&cell.unk0, &start.x);
    pos.x = 0;
    difference = end.y - start.y;
    frame = (int)(ride->timer > 0 ? (float)(ride->timer << 12) + 0.5f : (float)(ride->timer << 12) - 0.5f);
    pos.y = FX_Div((fx32)(((s64)frame * difference + 2048) >> 12), 24 << 12);
    if (ride->timer >= 4) {
        difference = end.z - start.z;
        frame = (int)(ride->timer - 4 > 0 ? (float)((ride->timer - 4) << 12) + 0.5f : (float)((ride->timer - 4) << 12) - 0.5f);
        pos.z = FX_Div((fx32)(((s64)frame * difference + 2048) >> 12), 20 << 12);
    } else {
        pos.z = 0;
    }
    VEC_Add(&pos, &start, &pos);
    WiFiPlazaCamera_SetTarget(camera, pos.x, pos.y, pos.z);
    pos.y += (fx32)(((s64)FX_SinCosTable_[((u16)(0x7FFF * ride->timer / 24) >> 4) * 2] * FX32_CONST(16) + 2048) >> 12);
    WiFiPlazaObj_SetPosition(ride->obj, &pos);
    return done;
}

#pragma force_active reset

// Landing hop on the mover (16 frames)
BOOL WiFiPlazaPlayerRide_Land(WiFiPlazaPlayerRide *ride, void *models) {
    VecFx32 pos;
    BOOL finished;
    fx32 offset;

    ride->timer++;
    if (ride->timer < 16) {
        finished = FALSE;
    } else {
        ride->timer = 16;
        finished = TRUE;
    }

    offset = FX_Mul(FX_SinIdx((u16)(((ride->timer % 8) * 0x7FFF) / 8)), FX32_CONST(3));
    WiFiPlazaObj_GetPosition(ride->obj, &pos);
    pos.y += offset;
    WiFiPlazaObj_SetPosition(ride->obj, &pos);
    return finished;
}

void WiFiPlazaPlayerRide_FollowMover(WiFiPlazaPlayerRide *ride, void *models, WiFiPlazaRoomWork *room) {
    VecFx32 pos;
    u32 side;
    u32 mover;
    ov45_0222AE08(ride->mover, &mover, &side);
    WiFiPlazaModels_GetMoverFrontPos(models, mover, side, &pos);
    WiFiPlazaObj_SetPosition(ride->obj, &pos);
}

void WiFiPlazaPlayerRide_FollowMoverBounce(WiFiPlazaPlayerRide *ride, void *models, WiFiPlazaRoomWork *room) {
    VecFx32 pos;
    u32 mover;
    u32 side;
    fx32 offset;
    ov45_0222AE08(ride->mover, &mover, &side);
    if (ov49_0225A520(room, mover) == TRUE) {
        if (!WiFiPlazaPlayerBounce_IsActive(&ride->bounce)) {
            WiFiPlazaPlayerBounce_Start(&ride->bounce);
        }
    } else {
        if (WiFiPlazaPlayerBounce_IsActive(&ride->bounce) == TRUE) {
            WiFiPlazaPlayerBounce_Stop(&ride->bounce);
        }
    }
    offset = WiFiPlazaPlayerBounce_Update(&ride->bounce);
    WiFiPlazaObj_GetPosition(ride->obj, &pos);
    pos.y += offset;
    WiFiPlazaObj_SetPosition(ride->obj, &pos);
}

void WiFiPlazaPlayerState_InitArrivalMenu(WiFiPlazaOwnArrivalWork *work, WiFiPlazaRoomWork *room) {
    int i;
    ov49_0225A10C(room, 18);
    for (i = 1; i < 18; i++) {
        ov49_0225A40C(room, 0, i);
        ov49_0225A144(room, WiFiPlazaRoom_GetMsgString(room, 1, 6), i);
    }
    ov49_0225A144(room, WiFiPlazaRoom_GetMsgString(room, 1, 7), 0);
    work->menuTemplate = sWiFiPlazaArrivalMenuTemplate;
    work->menuTemplate.items = ov49_0225A154(room);
}

void WiFiPlazaPlayerState_FreeArrivalMenu(WiFiPlazaOwnArrivalWork *work, WiFiPlazaRoomWork *room) {
    ov49_0225A134(room);
}

#pragma force_active on

// Another player's slot while that player is not in the room: creates its object once the lobby reports it
BOOL WiFiPlazaPlayerState_OtherWaitForPlayer(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    WiFiPlazaObjManager *objManager = WiFiPlazaRoom_GetObjManager(room);
    WiFiPlazaObj *obj = WiFiPlazaObj_FindById(objManager, index);
    WiFiPlazaPlacement *placement = WiFiPlazaPlayerState_GetWork(state);
    WiFiPlazaPlayerStateManager *manager = ov49_0225A010(room);
    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        WiFiPlazaPlayerState_AllocWork(state, 8);
        WiFiPlazaPlayerState_SetStep(state, 1);
    case 1: {
        int joined = ov45_0222A230(lobby, index);
        int present = ov45_0222A550(lobby, index);
        if (joined == TRUE || present == TRUE) {
            WiFiPlazaPlayerState_SetStep(state, 2);
        }
        break;
    }
    case 2: {
        WiFiPlazaPlayer *player = WiFiPlazaWork_GetPlayer(lobby, index);
        if (!player) {
            WiFiPlazaPlayerState_SetStep(state, 1);
            break;
        }
        if (ov45_0222AADC() == TRUE) {
            WiFiPlazaObj *newObj = WiFiPlazaObj_CreatePlayer(objManager, index, WiFiPlazaPlayer_GetAvatar(player));
            if (newObj) {
                ov49_022591C0(newObj, 0);
                WiFiPlazaPlayerState_SetStep(state, 3);
            }
        }
        break;
    }
    case 3: {
        WiFiPlazaPlayer *player = WiFiPlazaWork_GetPlayer(lobby, index);
        if (!player) {
            if (obj) {
                WiFiPlazaObj_Delete(obj);
            }
            WiFiPlazaPlayerState_SetStep(state, 1);
            break;
        }
        if (ov49_02261DBC(placement, lobby, room, obj, (u8)ov45_0222A920(player), 1)) {
            WiFiPlazaPlayerState_SetStep(state, 5);
            WiFiPlazaObj_SetHandler(objManager, obj, 0);
            ov49_0225A06C(room, (u8)index, 1);
            ov49_0225A04C(room, (u8)index, 1);
            ov49_02259130(obj, 0);
            WiFiPlazaPlayerState_Push(manager, index, &ov49_02269B58, placement);
        } else {
            WiFiPlazaObj_SetHandler(objManager, obj, 4);
            WiFiPlazaPlayerState_SetStep(state, 4);
            ov49_0225A04C(room, (u8)index, 1);
            ov49_0225A06C(room, (u8)index, 1);
            ov49_022591C0(obj, 1);
        }
        break;
    }
    case 4:
        if (WiFiPlazaObj_IsFinished(obj) == TRUE) {
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case 5:
        WiFiPlazaPlayerState_FreeWork(state);
        WiFiPlazaObj_SetHandler(objManager, obj, 2);
        WiFiPlazaPlayerState_Set(manager, index, &sWiFiPlazaPlayerStateOtherPresent, NULL);
        ov49_0225A04C(room, (u8)index, 0);
        ov49_0225A06C(room, (u8)index, 0);
        break;
    }
    return FALSE;
}

// Another player's slot while that player is in the room: leaving, riding a mover, moving to an attraction
BOOL WiFiPlazaPlayerState_OtherPresent(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaObj *obj;
    WiFiPlazaPlacement *placement;
    WiFiPlazaObjManager *objManager;
    WiFiPlazaPlayerStateManager *manager;
    WiFiPlazaWork *lobby;
    lobby = WiFiPlazaRoom_GetLobby(room);
    objManager = WiFiPlazaRoom_GetObjManager(room);
    manager = ov49_0225A010(room);
    obj = WiFiPlazaObj_FindById(objManager, index);
    WiFiPlazaObj_GetOwnPlayer(objManager);
    placement = WiFiPlazaPlayerState_GetWork(state);
    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        placement = WiFiPlazaPlayerState_AllocWork(state, 8);
        WiFiPlazaPlayerState_SetStep(state, 1);
    case 1:
        if (ov45_0222A25C(lobby, index)) {
            WiFiPlazaPlayerState_FreeWork(state);
            WiFiPlazaObj_SetHandler(objManager, obj, 0);
            ov49_0225A06C(room, (u8)index, 1);
            ov49_0225A04C(room, (u8)index, 1);
            WiFiPlazaPlayerState_Set(manager, index, &sWiFiPlazaPlayerStateOtherLeave, NULL);
            break;
        }
        ov45_0222A2A0(lobby, index);
        if (ov45_0222A374(lobby) == TRUE && ov45_0222ADA8(lobby, index) != -1) {
            WiFiPlazaObj_SetHandler(objManager, obj, 0);
            ov49_0225A06C(room, (u8)index, 1);
            ov49_0225A04C(room, (u8)index, 1);
            ov49_02259130(obj, 0);
            WiFiPlazaPlayerState_FreeWork(state);
            WiFiPlazaPlayerState_Set(manager, index, &ov49_02269B50, placement);
            break;
        }
        if (ov49_02261DBC(placement, lobby, room, obj, (u8)ov45_0222A920(WiFiPlazaWork_GetPlayer(lobby, index)), 0)) {
            WiFiPlazaObj_SetHandler(objManager, obj, 0);
            ov49_0225A06C(room, (u8)index, 1);
            ov49_0225A04C(room, (u8)index, 1);
            ov49_02259130(obj, 0);
            WiFiPlazaPlayerState_Push(manager, index, &ov49_02269B58, placement);
            break;
        }
        if (ov45_0222AD58(lobby, index) == TRUE) {
            WiFiPlazaObj_SetHandler(objManager, obj, 0);
            ov49_0225A04C(room, (u8)index, 1);
            WiFiPlazaPlayerState_Push(manager, index, &ov49_02269B80, NULL);
        }
        break;
    }
    return FALSE;
}

#pragma force_active reset

// Another player left the room: its object rises out and is deleted
BOOL WiFiPlazaPlayerState_OtherLeave(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaObjManager *objManager = WiFiPlazaRoom_GetObjManager(room);
    WiFiPlazaObj *obj = WiFiPlazaObj_FindById(objManager, index);

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        WiFiPlazaObj_SetHandler(objManager, obj, 3);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case 1:
        if (WiFiPlazaObj_IsFinished(obj) != FALSE) {
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case 2:
        WiFiPlazaObj_Delete(obj);
        WiFiPlazaPlayerState_Set(ov49_0225A010(room), index, &sWiFiPlazaPlayerStateOtherWait, NULL);
        break;
    }
    return FALSE;
}

// Another player rides a mover
BOOL ov49_02261930(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaObjManager *objManager;
    void *models;
    WiFiPlazaWork *lobby;
    VecFx32 vecD;
    VecFx32 vecC;
    VecFx32 vecB;
    VecFx32 vecA;
    WiFiPlazaOtherRideWork *work;
    BOOL done;
    fx32 delta;
    fx32 offset;

    lobby = WiFiPlazaRoom_GetLobby(room);
    objManager = WiFiPlazaRoom_GetObjManager(room);
    models = WiFiPlazaRoom_GetModels(room);
    work = WiFiPlazaPlayerState_GetWork(state);

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        work = WiFiPlazaPlayerState_AllocWork(state, sizeof(WiFiPlazaOtherRideWork));
        work->obj = WiFiPlazaObj_FindById(objManager, index);
        work->mover = ov45_0222ADA8(lobby, index);
        GF_ASSERT(work->mover != -1);
        ov45_0222AE08(work->mover, (u32 *)&work->mover, (u32 *)&work->side);
        WiFiPlazaObj_SetHandler(objManager, work->obj, 3);
        WiFiPlazaPlayerState_SetStep(state, 1);
        break;
    case 1:
        if (WiFiPlazaObj_IsFinished(work->obj) != FALSE) {
            WiFiPlazaModels_GetMoverFrontPos(models, work->mover, work->side, &vecD);
            work->x = vecD.x - FX32_ONE * 16;
            WiFiPlazaObj_GetPosition(work->obj, &vecD);
            work->startY = vecD.y;
            work->frame = 0;
            {
                UnkStruct_ov44_02232914 cell = { 0 };
                WiFiPlazaObj_SetGridPos(work->obj, cell);
            }
            PlaySE(SEQ_SE_DP_TELE2);
            ov49_02259184(work->obj, 1);
            WiFiPlazaPlayerState_SetStep(state, 2);
        }
        break;
    case 2:
        done = FALSE;
        work->frame++;
        if (work->frame >= 24) {
            work->frame = 24;
            done = TRUE;
        }
        WiFiPlazaModels_GetMoverFrontPos(models, work->mover, work->side, &vecC);
        vecB.z = vecC.z;
        vecB.x = work->x;
        delta = vecC.y - work->startY;
        vecB.y = FX_Div(FX_Mul(FX_F32_TO_FX32(work->frame), delta), FX32_CONST(24));
        vecB.y += work->startY;
        WiFiPlazaObj_SetPosition(work->obj, &vecB);
        if (done == TRUE) {
            ov49_02259184(work->obj, 0);
            ov49_022591B4(work->obj, 8);
            WiFiPlazaObj_SetDirection(work->obj, 2);
            WiFiPlazaPlayerState_SetStep(state, 3);
        }
        break;
    case 3:
        if (ov49_0225A520(room, work->mover) == TRUE) {
            if (WiFiPlazaPlayerBounce_IsActive(&work->bounce) == FALSE) {
                WiFiPlazaPlayerBounce_Start(&work->bounce);
            }
        } else if (WiFiPlazaPlayerBounce_IsActive(&work->bounce) == TRUE) {
            WiFiPlazaPlayerBounce_Stop(&work->bounce);
        }
        offset = WiFiPlazaPlayerBounce_Update(&work->bounce);
        WiFiPlazaModels_GetMoverFrontPos(models, work->mover, work->side, &vecA);
        vecA.y += offset;
        WiFiPlazaObj_SetPosition(work->obj, &vecA);
        if (ov45_0222AD80(lobby, work->mover) == 2) {
            WiFiPlazaPlayerState_SetStep(state, 4);
        }
        break;
    case 4:
        WiFiPlazaObj_Delete(work->obj);
        WiFiPlazaPlayerState_FreeWork(state);
        WiFiPlazaPlayerState_Set(ov49_0225A010(room), index, &sWiFiPlazaPlayerStateOtherWait, NULL);
        break;
    }
    return FALSE;
}

// Another player walks to the cell chosen by ov49_02261DBC (arg) and stays there while the lobby reports it
BOOL ov49_02261B74(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaObjManager *objManager;
    WiFiPlazaWork *lobby;
    BOOL leave;
    UnkStruct_ov44_02232914 current;
    UnkStruct_ov44_02232914 cell;
    UnkStruct_ov44_02232914 packedCell;
    WiFiPlazaPlacement *placement;
    void *obj;
    WiFiPlazaObj *ownPlayer;

    placement = WiFiPlazaPlayerState_GetArg(state);
    lobby = WiFiPlazaRoom_GetLobby(room);
    objManager = WiFiPlazaRoom_GetObjManager(room);
    obj = WiFiPlazaObj_FindById(objManager, index);
    ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        if (placement->justAppeared == FALSE) {
            WiFiPlazaPlayerState_NextStep(state);
        } else {
            WiFiPlazaPlayerState_SetStep(state, 3);
        }
        break;
    case 1:
        WiFiPlazaObj_SetHandler(objManager, obj, 3);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case 2:
    case 7:
    case 9:
        if (WiFiPlazaObj_IsFinished(obj) == TRUE) {
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case 3: {
        u32 packed = ((u32(*)(WiFiPlazaObj *))ov49_02258E34)(obj);
        packedCell.unk0 = packed;
        packedCell.unk2 = packed >> 16;
        current = packedCell;
        ov49_0225913C(obj, &current);
        if (ov49_022591CC(obj) == 0) {
            ov49_022591C0(obj, 1);
        }
        WiFiPlazaObj_SetHandler(objManager, obj, 4);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    }
    case 4:
        if (WiFiPlazaObj_IsFinished(obj) == TRUE) {
            WiFiPlazaPlayerState_NextStep(state);
            WiFiPlazaObj_SetHandler(objManager, obj, 0);
            ov49_0225A04C(room, index, 0);
            ov49_0225A06C(room, index, 0);
        }
        break;
    case 5:
        leave = FALSE;
        if (placement->besideOwnPlayer == TRUE && ov45_0222B0B0(lobby) == 0 && ov45_0222A920(WiFiPlazaWork_GetMyPlayer(lobby)) == 9) {
            ov45_0222B0A4(lobby);
            ov49_02258EAC(objManager, ownPlayer, 0, ov42_022282A4(WiFiPlazaObj_GetParam(obj, 6)));
        }
        obj = WiFiPlazaWork_GetPlayer(lobby, index);
        if (obj == NULL) {
            leave = TRUE;
        } else if (placement->besideOwnPlayer == TRUE && ov45_0222AFF8(lobby) == FALSE) {
            leave = TRUE;
        } else if (ov45_0222A2A0(lobby, index) != FALSE) {
            int kind = ov45_0222A920(obj);
            if (kind != placement->kind) {
                leave = TRUE;
            }
        }
        if (leave == TRUE) {
            WiFiPlazaPlayerState_NextStep(state);
            ov49_0225A04C(room, index, 1);
            ov49_0225A06C(room, index, 1);
        }
        break;
    case 6:
        WiFiPlazaObj_SetHandler(objManager, obj, 3);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case 8:
        if (WiFiPlazaObj_IsOnCell(WiFiPlazaObj_GetOwnPlayer(objManager), placement->x, placement->y) == FALSE) {
            cell.unk0 = placement->x << 4;
            cell.unk2 = placement->y << 4;
            WiFiPlazaObj_SetGridPos(obj, cell);
            ov49_0225913C(obj, &cell);
            WiFiPlazaObj_SetHandler(objManager, obj, 4);
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case 10:
        WiFiPlazaObj_SetHandler(objManager, obj, 2);
        ov49_0225A04C(room, index, 0);
        ov49_0225A06C(room, index, 0);
        return TRUE;
    }
    return FALSE;
}

// Places another player for lobby activity `kind` on a free cell of the matching map tags
BOOL ov49_02261DBC(WiFiPlazaPlacement *placement, WiFiPlazaWork *lobby, WiFiPlazaRoomWork *room, WiFiPlazaObj *obj, u8 kind, u32 justAppeared) {
    s32 direction;
    u16 cell[2];
    UnkStruct_ov44_02232914 position;
    UnkStruct_ov44_02232914 current;
    WiFiPlazaObj *ownPlayer;
    WiFiPlazaObjManager *objManager;
    WiFiPlazaMap *map;
    s32 id;
    u32 packed;
    s32 besideOwnPlayer = FALSE;

    objManager = WiFiPlazaRoom_GetObjManager(room);
    ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
    map = WiFiPlazaRoom_GetMap(room);
    id = WiFiPlazaObj_GetParam(obj, 4);
    direction = 1;

    switch (kind) {
    case 2:
        if (!WiFiPlazaPlayerState_FindFreeTaggedCell(map, objManager, ov49_02269B90, 4, cell)) {
            return besideOwnPlayer;
        }
        goto found;
    case 3:
        if (!WiFiPlazaPlayerState_FindFreeTaggedCell(map, objManager, ov49_02269BC0, 4, cell)) {
            return besideOwnPlayer;
        }
        goto found;
    case 4:
        if (!WiFiPlazaPlayerState_FindFreeTaggedCell(map, objManager, ov49_02269BA0, 4, cell)) {
            return besideOwnPlayer;
        }
        goto found;
    case 5:
        direction = 0;
        if (!WiFiPlazaPlayerState_FindFreeTaggedCell(map, objManager, ov49_02269C20, 8, cell)) {
            return besideOwnPlayer;
        }
        goto found;
    case 6:
        direction = 0;
        if (!WiFiPlazaPlayerState_FindFreeTaggedCell(map, objManager, ov49_02269C40, 8, cell)) {
            return besideOwnPlayer;
        }
        goto found;
    case 7:
        direction = 3;
        if (!WiFiPlazaPlayerState_FindFreeTaggedCell(map, objManager, ov49_02269BB0, 4, cell)) {
            return besideOwnPlayer;
        }
        goto found;
    case 8:
        direction = 0;
        if (!WiFiPlazaPlayerState_FindFreeTaggedCell(map, objManager, ov49_02269BD0, 4, cell)) {
            return besideOwnPlayer;
        }
        goto found;
    case 10:
        return besideOwnPlayer;
    case 9:
        if (!ov45_0222AFF8(lobby)) {
            return besideOwnPlayer;
        }
        if (!ov45_0222B00C(lobby)) {
            return besideOwnPlayer;
        }
        if (id != ov45_0222B020(lobby)) {
            return besideOwnPlayer;
        }
        if (WiFiPlazaObj_FindFreeNeighbor(objManager, ownPlayer, &direction, (UnkStruct_ov44_02232914 *)cell) != TRUE) {
            GF_AssertFail();
        }
        direction = ov42_022282A4(direction);
        besideOwnPlayer = TRUE;
        goto found;
    case 0:
    case 1:
    case 11:
    case 12:
    case 13:
    case 14:
        break;
    }

    return FALSE;

found:
    packed = ((u32(*)(WiFiPlazaObj *))ov49_02258E34)(obj);
    current.unk0 = packed;
    current.unk2 = packed >> 16;
    position = current;
    WiFiPlazaObj_SetGridPosAndDir(obj, *(UnkStruct_ov44_02232914 *)cell, direction);
    placement->x = position.unk0 / 16;
    placement->y = position.unk2 / 16;
    placement->kind = kind;
    placement->besideOwnPlayer = besideOwnPlayer;
    placement->justAppeared = justAppeared;
    return TRUE;
}

BOOL WiFiPlazaPlayerState_FindFreeTaggedCell(WiFiPlazaMap *map, WiFiPlazaObjManager *objManager, const u32 *tags, u32 count, u16 *outPos) {
    u32 i;
    u16 x;
    u16 y;

    for (i = 0; i < count; i++) {
        if (WiFiPlazaMap_FindTileTag(map, tags[i], &x, &y, 0) == TRUE && WiFiPlazaObj_FindAt(objManager, x, y) == FALSE) {
            outPos[0] = x << 4;
            outPos[1] = y << 4;
            return TRUE;
        }
    }
    return FALSE;
}
