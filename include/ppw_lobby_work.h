#ifndef POKEHEARTGOLD_PPW_LOBBY_WORK_H
#define POKEHEARTGOLD_PPW_LOBBY_WORK_H

#include "overlay45_format_buffer_private.h"

// The callbacks the game passes to PPW_LobbyInitializeAsync, in table order.
typedef struct PPWLobbyCallbacks {
    void (*onEnterChannel)(void);
    void (*onPlayerEnter)(void);
    void (*onPlayerLeave)(void);
    void (*onEvent3)(void);
    void (*onMessage)(void);
    void (*onLocationsChanged)(void);
    void (*onPlayerRecordChanged)(void);
    void (*onPlayerUpdate)(void);
    void (*onMatchInfo)(int, void *);
    void (*onMatchClosed)(int, void *);
    void (*onLobbyEvent)(void);
    void (*onOwnRecord)(void);
    void (*onEvent12Result)(void);
    void (*onSpecialVisitorsReceived)(void);
    void (*onSurveyReceived)(void);
    void (*onSurveySubmitted)(s32);
    void (*onExcessFlood)(int);
} PPWLobbyCallbacks;

typedef struct PPWLobbyChannel {
    int kind;
    Str name;
    u32 unk_10;
    u32 unk_14;
} PPWLobbyChannel;

// Uploaded as the "b_lib_u_system" channel key.
typedef struct PPWLobbySystemProfile {
    int channel;
    s64 time;
    u32 unk_0C;
} PPWLobbySystemProfile;

typedef struct PPWLobbyPlayer {
    const void *vtable;
    PPWLobbySystemProfile systemProfile;
    int systemProfileChanged;
    Vec userProfile;
    int userProfileChanged;
    int state;
    int id;
    int systemProfileReceived;
    int userProfileReceived;
    int entered;
} PPWLobbyPlayer;

typedef struct PPWLobbyPlayers {
    const void *vtable;
    u32 playerSet[3];
    PPWLobbyPlayer self;
} PPWLobbyPlayers;

typedef struct PPWLobbySchedule {
    int state;
    int unk_04;
    u8 unk_08[0x24];
} PPWLobbySchedule;

typedef struct PPWLobbyWork {
    int state;
    int error;
    PPWLobbyCallbacks callbacks;
    u32 userId;
    u8 unk_50[0x18];
    int recruiting;
    int submittingQuestionnaire;
    u32 unk_70;
    u8 recruitInfo[0x44];
    PPWLobbyChannel mainChannel;
    PPWLobbyChannel subChannel;
    PPWLobbyPlayers players;
    u32 timers[4];
    PPWLobbySchedule schedule;
    u8 unk_170[0x30C];
} PPWLobbyWork;

// The start of the player record the game uploads as its user profile.
typedef struct PPWLobbyUserProfile {
    u32 lobbyId;
    u32 trainerId;
    u8 unk_08[0x31];
    u8 language;
    u8 unk_3A[8];
    u8 version;
} PPWLobbyUserProfile;

#endif // POKEHEARTGOLD_PPW_LOBBY_WORK_H
