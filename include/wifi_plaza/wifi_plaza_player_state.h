#ifndef POKEHEARTGOLD_WIFI_PLAZA_PLAYER_STATE_H
#define POKEHEARTGOLD_WIFI_PLAZA_PLAYER_STATE_H

#include "global.h"

#include "wifi_plaza/wifi_plaza_obj.h"
#include "wifi_plaza/wifi_plaza_room.h"

// Per-player state machines of the Wi-Fi Plaza room. Every lobby player index (0-19) has a
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
    void *work;
    void *arg; // given to Set / Push
    u32 step;
} WiFiPlazaPlayerStateFrame;

struct WiFiPlazaPlayerState {
    u16 heapID;
    u8 active;
    u8 index;
    WiFiPlazaPlayerStateFrame current;
    WiFiPlazaPlayerStateFrame saved; // suspended by Push
};

typedef struct WiFiPlazaPlayerStateManager {
    enum HeapID heapID;
    WiFiPlazaRoomWork *room;
    WiFiPlazaPlayerState states[WIFI_PLAZA_PLAYER_STATE_COUNT];
    WiFiPlazaPlayerState exclusive; // while it has a handler, the only slot that runs
} WiFiPlazaPlayerStateManager;

// Map tile actions, terminated by action WIFI_PLAZA_TILE_ACTION_END
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
void WiFiPlazaPlayerState_SetActive(WiFiPlazaPlayerStateManager *manager, u32 index, u8 active);
void WiFiPlazaPlayerState_InitOwn(WiFiPlazaRoomWork *room);
void WiFiPlazaPlayerState_InitOthers(WiFiPlazaRoomWork *room);

#endif // POKEHEARTGOLD_WIFI_PLAZA_PLAYER_STATE_H
