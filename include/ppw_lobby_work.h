#ifndef POKEHEARTGOLD_PPW_LOBBY_WORK_H
#define POKEHEARTGOLD_PPW_LOBBY_WORK_H

#include "lobby_event_deque.h"
#include "overlay45_format_buffer_private.h"

// The callbacks the game passes to PPW_LobbyInitializeAsync, in table order.
typedef struct PPWLobbyCallbacks {
    void (*onEnterChannel)(int, int);
    void (*onPlayerEnter)(u32, void *, void *, u32);
    void (*onPlayerLeave)(int);
    void (*onEvent3)(int, int, int, void *);
    void (*onMessage)(int, int, int, void *, u32);
    void (*onLocationsChanged)(int, int, int, int, const char *, const void *, u32, u32);
    void (*onPlayerRecordChanged)(int, const void *);
    void (*onPlayerUpdate)(int, const u8 *, u32);
    void (*onMatchInfo)(int, void *);
    void (*onMatchClosed)(int, void *);
    void (*onLobbyEvent)(u32);
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
    LobbyCharVector userProfile;
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

// A special visitor of the lobby: player id and visitor value, as the game receives them.
typedef struct PPWLobbyVipRecord {
    u32 id;
    u32 value;
} PPWLobbyVipRecord;

typedef struct PPWLobbyVipVector {
    PPWLobbyVipRecord *data;
    u32 size;
    u32 capacity;
} PPWLobbyVipVector;

// The schedule record as the server sends it and PPWLobbySchedule_Copy writes it: a header and the timed events.
typedef struct PPWLobbyScheduleRecord {
    u32 lockTime;
    u32 unk_04;
    u32 roomFlag;
    u8 roomType;
    u8 season;
    u16 eventCount;
    LobbyEvent events[1];
} PPWLobbyScheduleRecord;

// The event schedule the server sends for the lobby, and the questionnaire.
typedef struct PPWLobbySchedule {
    int state;
    int hasTime;
    PPWLobbyTime time;
    s64 startTime;
    BOOL startRequested;
    u32 lockTime;
    u32 unk_20;
    u32 roomFlag;
    u8 roomType;
    u8 season;
    LobbyEventVector events;
    LobbyEventDeque pendingEvents;
    PPWLobbyVipVector vips;
    u8 questionnaire[0x2D8];
    BOOL hasQuestionnaire;
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
} PPWLobbyWork;

#ifdef __cplusplus
extern "C" {
#endif

extern PPWLobbyWork *gPPWLobbyWork;

PPWLobbyWork *PPWLobby_GetWork(void);
BOOL PPWLobbySchedule_GetQuestionnaire(PPWLobbySchedule *, void *);
BOOL PPWLobbySchedule_SetQuestionnaire(PPWLobbySchedule *, const void *);
void PPWLobbySchedule_Process(PPWLobbySchedule *);
BOOL PPWLobbySchedule_CanStart(PPWLobbySchedule *);
BOOL PPWLobbySchedule_Start(PPWLobbySchedule *);
PPWLobbySchedule *PPWLobbySchedule_Init(PPWLobbySchedule *);
BOOL PPWLobbySchedule_Copy(PPWLobbySchedule *, PPWLobbyScheduleRecord *, u32 *);
void PPWLobbySchedule_CopyToVector(LobbyCharVector *, PPWLobbySchedule *);
LobbyEvent *LobbyEventVector_Data(LobbyEventVector *);

#ifdef __cplusplus
}
#endif

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
