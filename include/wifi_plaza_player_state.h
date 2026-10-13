#ifndef POKEHEARTGOLD_WIFI_PLAZA_PLAYER_STATE_H
#define POKEHEARTGOLD_WIFI_PLAZA_PLAYER_STATE_H

#include "global.h"

#include "wifi_plaza_obj.h"
#include "wifi_plaza_room.h"

// Per-player state machines of the Wi-Fi Plaza room (room work +0x3F0). Every lobby player index (0-19) has a
// slot running a handler pair: `update` before the room's objects move, `lateUpdate` after them. A handler
// keeps its progress in a step counter and an optional heap work buffer. Push suspends the current handler
// and runs another one; when that one returns TRUE the suspended handler resumes.

#define WIFI_PLAZA_PLAYER_STATE_COUNT 20

typedef struct WiFiPlazaPlayerState WiFiPlazaPlayerState;

typedef BOOL (*WiFiPlazaPlayerStateFunc)(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);

typedef struct WiFiPlazaPlayerStateHandlers {
    WiFiPlazaPlayerStateFunc update;
    WiFiPlazaPlayerStateFunc lateUpdate;
} WiFiPlazaPlayerStateHandlers;

typedef struct WiFiPlazaPlayerStateFrame {
    const WiFiPlazaPlayerStateHandlers *handlers; // NULL: no handler
    void *work;                                   // WiFiPlazaPlayerState_AllocWork
    void *arg;                                    // given to Set / Push
    u32 step;
} WiFiPlazaPlayerStateFrame;

struct WiFiPlazaPlayerState {
    u16 heapID;
    u8 active;
    u8 index;
    WiFiPlazaPlayerStateFrame current; // 0x04
    WiFiPlazaPlayerStateFrame saved;   // 0x14 suspended by Push
};

typedef struct WiFiPlazaPlayerStateManager {
    enum HeapID heapID;
    WiFiPlazaRoomWork *room;
    WiFiPlazaPlayerState states[WIFI_PLAZA_PLAYER_STATE_COUNT];
    WiFiPlazaPlayerState exclusive; // 0x2D8 while it has a handler, the only slot that runs
} WiFiPlazaPlayerStateManager;

// Map tile actions (tables of WiFiPlazaPlayerState_RunTileAction), terminated by action
// WIFI_PLAZA_TILE_ACTION_END
enum {
    WIFI_PLAZA_TILE_ACTION_SET,  // free the caller's work and replace its handler
    WIFI_PLAZA_TILE_ACTION_PUSH, // call `callback`, then push the handler
    WIFI_PLAZA_TILE_ACTION_NONE,
    WIFI_PLAZA_TILE_ACTION_END,
};

typedef void (*WiFiPlazaTileActionCallback)(void *work, WiFiPlazaRoomWork *room, u32 index, u8 tile);

typedef struct WiFiPlazaTileAction {
    u8 tiles[8]; // tile attributes, 0xFF-terminated when shorter than 8
    u32 action;
    WiFiPlazaPlayerStateHandlers handlers;
    WiFiPlazaTileActionCallback callback;
} WiFiPlazaTileAction;

// Small random up-and-down hop (6 units over 4 frames, then a pause of 0-3 frames) of a player standing
// on an active mover
typedef struct WiFiPlazaPlayerBounce {
    fx32 offset;
    volatile s8 frame;
    u8 state; // 0 idle, 1 pausing, 2 hopping
    u8 timer;
    u8 repeat;
} WiFiPlazaPlayerBounce;

WiFiPlazaPlayerStateManager *WiFiPlazaPlayerState_CreateManager(WiFiPlazaRoomWork *room, enum HeapID heapID);
void WiFiPlazaPlayerState_DeleteManager(WiFiPlazaPlayerStateManager *manager);
void WiFiPlazaPlayerState_Update(WiFiPlazaPlayerStateManager *manager);
void WiFiPlazaPlayerState_LateUpdate(WiFiPlazaPlayerStateManager *manager);
void *WiFiPlazaPlayerState_GetArg(WiFiPlazaPlayerState *state);
void *WiFiPlazaPlayerState_AllocWork(WiFiPlazaPlayerState *state, u32 size);
void WiFiPlazaPlayerState_FreeWork(WiFiPlazaPlayerState *state);
void *WiFiPlazaPlayerState_GetWork(WiFiPlazaPlayerState *state);
u32 WiFiPlazaPlayerState_GetStep(WiFiPlazaPlayerState *state);
void WiFiPlazaPlayerState_SetStep(WiFiPlazaPlayerState *state, u32 step);
void WiFiPlazaPlayerState_NextStep(WiFiPlazaPlayerState *state);
void WiFiPlazaPlayerState_Set(WiFiPlazaPlayerStateManager *manager, u32 index, const WiFiPlazaPlayerStateHandlers *handlers, void *arg);
void WiFiPlazaPlayerState_Push(WiFiPlazaPlayerStateManager *manager, u32 index, const WiFiPlazaPlayerStateHandlers *handlers, void *arg);
void WiFiPlazaPlayerState_SetActive(WiFiPlazaPlayerStateManager *manager, u32 index, u8 active);
void WiFiPlazaPlayerState_RunAll(WiFiPlazaPlayerStateManager *manager, u32 which);
void WiFiPlazaPlayerState_Init(WiFiPlazaPlayerState *state, u32 index, u32 heapID);
void WiFiPlazaPlayerState_Clear(WiFiPlazaPlayerState *state);
void WiFiPlazaPlayerState_Run(WiFiPlazaPlayerState *state, WiFiPlazaPlayerStateManager *manager, u32 which, u32 index);
void WiFiPlazaPlayerState_SetFrame(WiFiPlazaPlayerState *state, const WiFiPlazaPlayerStateHandlers *handlers, void *arg);
void WiFiPlazaPlayerState_SetActiveFlag(WiFiPlazaPlayerState *state, u8 active);
void WiFiPlazaPlayerState_PushFrame(WiFiPlazaPlayerState *state, const WiFiPlazaPlayerStateHandlers *handlers, void *arg);
void WiFiPlazaPlayerState_PopFrame(WiFiPlazaPlayerState *state);
BOOL WiFiPlazaPlayerState_IsSavedFree(WiFiPlazaPlayerState *state);
BOOL WiFiPlazaPlayerState_HasHandler(WiFiPlazaPlayerState *state);
void WiFiPlazaPlayerStateFrame_Init(WiFiPlazaPlayerStateFrame *frame, const WiFiPlazaPlayerStateHandlers *handlers, void *arg, u32 step, void *work);
void WiFiPlazaPlayerStateFrame_Clear(WiFiPlazaPlayerStateFrame *frame);
void WiFiPlazaPlayerState_InitOwn(WiFiPlazaRoomWork *room);
void WiFiPlazaPlayerState_InitOthers(WiFiPlazaRoomWork *room);
BOOL WiFiPlazaPlayerState_IsDirectionHeld(int direction);
void WiFiPlazaPlayerState_RunTileAction(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index, const WiFiPlazaTileAction *actions, u32 tile);
WiFiPlazaObj *WiFiPlazaPlayerState_CreateOwnPlayer(WiFiPlazaRoomWork *room, int id);
void WiFiPlazaPlayerState_CreateOwnPlayerAt(WiFiPlazaRoomWork *room, int id, int x, int y, s32 direction);
void WiFiPlazaPlayerBounce_Start(WiFiPlazaPlayerBounce *bounce);
fx32 WiFiPlazaPlayerBounce_Update(WiFiPlazaPlayerBounce *bounce);
void WiFiPlazaPlayerBounce_Stop(WiFiPlazaPlayerBounce *bounce);
BOOL WiFiPlazaPlayerBounce_IsActive(WiFiPlazaPlayerBounce *bounce);
BOOL WiFiPlazaPlayerState_PlayInstrument(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaPlayerState_OwnArrival(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaPlayerState_OwnReturnFromAttraction(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaPlayerState_OwnReturnFromMinigame(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaPlayerState_OwnReturnFromSurvey(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaPlayerState_OwnWalk(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaPlayerState_OwnWalkLate(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaPlayerState_OwnExit(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaPlayerState_OwnEnterAttraction(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaPlayerState_OwnEnterMinigame(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaPlayerState_OwnUseSurveyMonitor(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
int WiFiPlazaPlayerState_GetAttractionSlot(int tile);
int WiFiPlazaPlayerState_GetAttractionResult(int tile);
int WiFiPlazaPlayerState_GetAttractionLobbyState(int tile);
BOOL WiFiPlazaTalk_Greeter(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaPlayerState_OwnRideMover(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaPlayerState_OtherWaitForPlayer(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaPlayerState_OtherPresent(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaPlayerState_OtherLeave(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaPlayerState_OtherRideMover(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaPlayerState_OtherDoActivity(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaPlayerState_FindFreeTaggedCell(WiFiPlazaMap *map, WiFiPlazaObjManager *objManager, const u32 *tags, u32 count, u16 *outPos);

#endif // POKEHEARTGOLD_WIFI_PLAZA_PLAYER_STATE_H
