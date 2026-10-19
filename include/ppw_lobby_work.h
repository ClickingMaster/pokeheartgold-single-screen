#ifndef POKEHEARTGOLD_PPW_LOBBY_WORK_H
#define POKEHEARTGOLD_PPW_LOBBY_WORK_H

#include "overlay45_format_buffer_private.h"

// The callbacks the game passes to PPW_LobbyInitializeAsync, in table order.
typedef struct PPWLobbyCallbacks {
    void (*onEnterChannel)(int, int);
    void (*onPlayerEnter)(u32, void *, void *, u32);
    void (*onPlayerLeave)(int);
    void (*onEvent3)(int, int, int, void *);
    void (*onMessage)(int, int, int, void *, u32);
    void (*onLocationsChanged)(int, int, int, int, int, char *, u32, int);
    void (*onPlayerRecordChanged)(int, const void *);
    void (*onPlayerUpdate)(int, const u8 *, u32);
    void (*onMatchInfo)(int, void *);
    void (*onMatchClosed)(int, void *);
    void (*onLobbyEvent)(void);
    void (*onOwnRecord)(int, void *, u32);
    int (*onScheduleReceived)(int, void *);
    void (*onSpecialVisitorsReceived)(int, void *, u32);
    void (*onSurveyReceived)(int, void *);
    void (*onSurveySubmitted)(s32);
    void (*onExcessFlood)(int);
} PPWLobbyCallbacks;

typedef struct PPWLobbyChannel {
    int kind;
    Str name;
    int usersEnumerated;
    int modeReady;
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

// Channel time in seconds (RTC_ConvertDateTimeToSecond).
typedef struct PPWLobbyTime {
    s64 seconds;
} PPWLobbyTime;

typedef struct PPWLobbySchedule {
    int state;
    int hasTime;
    PPWLobbyTime time;
    u8 unk_10[0x1C];
} PPWLobbySchedule;

typedef struct PPWLobbyWork {
    int state;
    int error;
    PPWLobbyCallbacks callbacks;
    u32 userId;
    u32 prefix;
    u32 unk_54;
    Str channelBaseName;
    int isNewChannel;
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
