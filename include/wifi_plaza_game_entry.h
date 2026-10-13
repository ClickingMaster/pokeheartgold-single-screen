#ifndef POKEHEARTGOLD_WIFI_PLAZA_GAME_ENTRY_H
#define POKEHEARTGOLD_WIFI_PLAZA_GAME_ENTRY_H

#include "global.h"

#include "list_menu.h"
#include "wifi_plaza_room.h"

// Registration for the Plaza Games at their booths (message bank 0 of the room, msg 776), run as player state
// handlers. Up to four players gather in a lobby game session (ov45_0222F2xx / ov45_0222F3xx, indexed by game slot);
// when it fills up or the time runs out, the members exchange their lobby ids over the link and the game starts.

// Outcome of a registration, left in WiFiPlazaGameEntryParam.result for the booth handler that started it
enum WiFiPlazaGameEntryResult {
    WIFI_PLAZA_GAME_ENTRY_IN_PLAY,       // 0 the game started without us (msg 4)
    WIFI_PLAZA_GAME_ENTRY_SESSION_FULL,  // 1 a game is in session (msg 5)
    WIFI_PLAZA_GAME_ENTRY_PLAZA_CLOSING, // 2 registration is closed (msg 6)
    WIFI_PLAZA_GAME_ENTRY_NO_MEMBERS,    // 3 nobody joined in time (msg 0x13)
    WIFI_PLAZA_GAME_ENTRY_LINK_ERROR,    // 4 a member dropped out (msg 0x11)
    WIFI_PLAZA_GAME_ENTRY_CANCELLED,     // 5 CANCEL in the join menu (no message)
    WIFI_PLAZA_GAME_ENTRY_DROPPED_OUT,   // 6 B button while waiting (msg 0x1D)
    WIFI_PLAZA_GAME_ENTRY_NOT_READY,     // 7 the registration system is not ready (msg 7)
    WIFI_PLAZA_GAME_ENTRY_STARTED,       // 8
};

// Handler argument, filled in by the booth handler
typedef struct WiFiPlazaGameEntryParam {
    s8 result;       // enum WiFiPlazaGameEntryResult
    u8 slot;         // lobby game session
    u8 game;         // activity name / game id reported to the lobby
    u8 playerStatus; // WiFiPlazaWork_SetActivity value while registering
} WiFiPlazaGameEntryParam;

// The game's status panel: session state, free places and the seconds left
typedef struct WiFiPlazaGameStatus {
    u8 sessionActive; // 0x00 WiFiPlazaLobbyClient_IsMatchActive
    u8 space;         // 0x01 free places (4 while no session is active)
    u8 accepting;     // 0x02 WiFiPlazaLobbyClient_IsMatchOpen; 0 while the game is played
    u8 game;          // 0x03
    s16 seconds;      // 0x04
    u8 showSeconds;   // 0x06
    u8 redraw;        // 0x07 WIFI_PLAZA_GAME_STATUS_* bits
} WiFiPlazaGameStatus;

#define WIFI_PLAZA_GAME_STATUS_TITLE   (1 << 0)
#define WIFI_PLAZA_GAME_STATUS_COUNTS  (1 << 1)
#define WIFI_PLAZA_GAME_STATUS_STATE   (1 << 2)
#define WIFI_PLAZA_GAME_STATUS_SECONDS (1 << 3)

typedef struct WiFiPlazaGameEntry {
    s16 timeLeft;                  // 0x00 frames until the registration closes
    u16 entriesClosed;             // 0x02 host closed the entries (WiFiPlazaLobbyClient_CloseMatch)
    u8 nextStep;                   // 0x04 after the message / the link sync
    s8 syncDelay;                  // 0x05
    u8 syncId;                     // 0x06
    u8 reportedMembers;            // 0x07 member count last reported to the lobby
    u16 members;                   // 0x08
    u16 syncTimer;                 // 0x0A
    s32 lobbyId;                   // 0x0C sent to the other members
    s32 timeout;                   // 0x10
    BOOL startingSoon;             // 0x14
    ListMenuTemplate menuTemplate; // 0x18
    u32 menuCount;                 // 0x38
    WiFiPlazaGameStatus status;    // 0x3C
} WiFiPlazaGameEntry;

BOOL WiFiPlazaGameEntry_Register(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, int playerIndex);
BOOL WiFiPlazaGameEntry_EndLink(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room);
BOOL WiFiPlazaGameEntry_ReadSign(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room);
void WiFiPlazaGameEntry_PrintMessage(WiFiPlazaGameEntry *entry, WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 msgId, u32 nextStep);
void WiFiPlazaGameEntry_StartSync(WiFiPlazaGameEntry *entry, WiFiPlazaPlayerState *state, u8 nextStep, u8 syncId);
void WiFiPlazaGameEntry_InitMenu(WiFiPlazaGameEntry *entry, WiFiPlazaRoomWork *room, u32 count, u32 firstMsg);
void WiFiPlazaGameEntry_FreeMenu(WiFiPlazaGameEntry *entry, WiFiPlazaRoomWork *room);
BOOL WiFiPlazaGameEntry_UpdateWaiting(WiFiPlazaGameEntry *entry, WiFiPlazaRoomWork *room, WiFiPlazaGameEntryParam *param, WiFiPlazaPlayerState *state, int playerIndex);
void WiFiPlazaGameStatus_Open(WiFiPlazaGameStatus *status, WiFiPlazaRoomWork *room, u32 game, u32 showSeconds, s32 frames);
void WiFiPlazaGameStatus_Close(WiFiPlazaGameStatus *status, WiFiPlazaRoomWork *room);
void WiFiPlazaGameStatus_SetFrames(WiFiPlazaGameStatus *status, s32 frames);
void WiFiPlazaGameStatus_Update(WiFiPlazaGameStatus *status, WiFiPlazaRoomWork *room, u32 force);
void WiFiPlazaGameStatus_UpdateInPlay(WiFiPlazaGameStatus *status, WiFiPlazaRoomWork *room, u32 force);
void WiFiPlazaGameStatus_Draw(WiFiPlazaGameStatus *status, WiFiPlazaRoomWork *room, u32 force, u32 showInPlay);

#endif // POKEHEARTGOLD_WIFI_PLAZA_GAME_ENTRY_H
