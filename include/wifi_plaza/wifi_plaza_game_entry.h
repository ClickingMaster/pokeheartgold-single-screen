#ifndef POKEHEARTGOLD_WIFI_PLAZA_GAME_ENTRY_H
#define POKEHEARTGOLD_WIFI_PLAZA_GAME_ENTRY_H

#include "global.h"

#include "wifi_plaza/wifi_plaza_room.h"

#include "list_menu.h"

// Registration for the Plaza Games at their booths. Up to four players gather in a lobby game session; when it
// fills up or the time runs out, the members exchange their lobby ids over the link and the game starts.

// Outcome of a registration, left in WiFiPlazaGameEntryParam.result for the booth handler that started it
enum WiFiPlazaGameEntryResult {
    WIFI_PLAZA_GAME_ENTRY_IN_PLAY,       // the game started without us (msg 4)
    WIFI_PLAZA_GAME_ENTRY_SESSION_FULL,  // a game is in session (msg 5)
    WIFI_PLAZA_GAME_ENTRY_PLAZA_CLOSING, // registration is closed (msg 6)
    WIFI_PLAZA_GAME_ENTRY_NO_MEMBERS,    // nobody joined in time (msg 0x13)
    WIFI_PLAZA_GAME_ENTRY_LINK_ERROR,    // a member dropped out (msg 0x11)
    WIFI_PLAZA_GAME_ENTRY_CANCELLED,     // CANCEL in the join menu (no message)
    WIFI_PLAZA_GAME_ENTRY_DROPPED_OUT,   // B button while waiting (msg 0x1D)
    WIFI_PLAZA_GAME_ENTRY_NOT_READY,     // the registration system is not ready (msg 7)
    WIFI_PLAZA_GAME_ENTRY_STARTED,
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
    u8 sessionActive;
    u8 space;     // free places (4 while no session is active)
    u8 accepting; // 0 while the game is played
    u8 game;
    s16 seconds;
    u8 showSeconds;
    u8 redraw; // WIFI_PLAZA_GAME_STATUS_* bits
} WiFiPlazaGameStatus;

#define WIFI_PLAZA_GAME_STATUS_TITLE   (1 << 0)
#define WIFI_PLAZA_GAME_STATUS_COUNTS  (1 << 1)
#define WIFI_PLAZA_GAME_STATUS_STATE   (1 << 2)
#define WIFI_PLAZA_GAME_STATUS_SECONDS (1 << 3)

typedef struct WiFiPlazaGameEntry {
    s16 timeLeft;      // frames until the registration closes
    u16 entriesClosed; // host closed the entries
    u8 nextStep;       // after the message / the link sync
    s8 syncDelay;
    u8 syncId;
    u8 reportedMembers; // member count last reported to the lobby
    u16 members;
    u16 syncTimer;
    s32 lobbyId; // sent to the other members
    s32 timeout;
    BOOL startingSoon;
    ListMenuTemplate menuTemplate;
    u32 menuCount;
    WiFiPlazaGameStatus status;
} WiFiPlazaGameEntry;

BOOL WiFiPlazaGameEntry_Register(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 playerIndex);
BOOL WiFiPlazaGameEntry_EndLink(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
BOOL WiFiPlazaGameEntry_ReadSign(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);

#endif // POKEHEARTGOLD_WIFI_PLAZA_GAME_ENTRY_H
