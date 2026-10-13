#ifndef POKEHEARTGOLD_WIFI_PLAZA_INTERNAL_H
#define POKEHEARTGOLD_WIFI_PLAZA_INTERNAL_H

// Wi-Fi Plaza (overlay 45): the overlay's work data and the functions shared between its source files.

#include "global.h"

#include "billboard_lists.h"
#include "easy_chat.h"
#include "filesystem.h"
#include "friend_roster.h"
#include "gf_3d_loader.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "message_format.h"
#include "model_animation.h"
#include "msgdata.h"
#include "overlay_42.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pm_string.h"
#include "save.h"
#include "save_wifi_history.h"
#include "sys_task_api.h"

// Arguments of the Wi-Fi Plaza overlay (field_transition_tasks_2.c).
typedef struct WiFiPlazaArgs {
    void *loginTime;
    SaveData *save;
    u32 skipPrompt;
} WiFiPlazaArgs;

// Second part of a plaza player record (copied to the save data when the plaza is left, WiFiPlazaWork_Delete).
typedef struct WiFiPlazaPlayerSurvey {
    u32 surveyId;
    u32 answer; // < 3
} WiFiPlazaPlayerSurvey;

// A player in the plaza lobby: the own one is WiFiPlazaWork.profile.player, the others come from the lobby
// library (WiFiPlazaLobbyClient_GetPlayer).
typedef struct WiFiPlazaPlayer {
    u32 lobbyId;
    u32 trainerId;
    u16 name[8];
    u32 entryTime;   // WiFiPlaza_SecondsToTime of the entry time (WiFiPlaza_OnPlayerEnter)
    void *localTime; // WiFiPlazaTime: local time of the location (WiFiPlaza_GetLocalTime)
    u16 species[6];
    u8 forms[6];
    u8 isEgg[6];
    u8 gender; // >= 2: taken from the avatar table
    u8 language;
    u16 avatarIndex; // mapped to the avatar by WiFiPlaza_IndexToAvatar
    u16 country;
    u8 region;
    u8 hasNationalDex;
    u8 gameCleared;
    u8 unk_41; // < 27
    u8 version;
    u8 unk_43;    // < 15
    s64 unk_44;   // SysInfo_RTC.seconds_since_nitro_epoch
    u8 slots[12]; // < 24: used
    void *slotData[12];
    u16 unk_88[2]; // < 18
    WiFiPlazaPlayerSurvey survey;
} WiFiPlazaPlayer;

// The own plaza profile (WiFiPlazaProfile_InitFromSave); WiFiPlazaProfile_UpdateChecksum and WiFiPlazaProfile_IsChecksumValid keep its CRC.
typedef struct WiFiPlazaProfile {
    u16 name[8];
    u16 lobbyName[8]; // the name received from the lobby (WiFiPlazaProfile_SetFromLobbyRecord)
    WiFiPlazaPlayer player;
    u32 crc; // of player
} WiFiPlazaProfile;

// The ids of the players in the lobby (WiFiPlazaLobbyClient_GetPlayerList): -1 = free.
typedef struct WiFiPlazaPlayerList {
    s32 count;
    s32 *ids;
} WiFiPlazaPlayerList;

// Entry of the avatar table ov45_02254A84.
typedef struct WiFiPlazaAvatarEntry {
    u16 avatar;
    u16 unk_02;
} WiFiPlazaAvatarEntry;

// Part of WiFiPlazaWork at 0x1C0: an exchange with one other player of the lobby (WiFiPlazaWork_StartTalk starts it
// for a lobby slot, WiFiPlazaWork_AdvanceTalk / WiFiPlazaWork_CancelTalkRequest / WiFiPlazaWork_EndTalk answer or end it).
typedef struct WiFiPlazaTalkSession {
    u32 talkedWith;  // bit per lobby slot (WiFiPlazaTalkSession_HasTalkedWith / WiFiPlazaTalkSession_SetTalkedWith / WiFiPlazaTalkSession_ClearTalkedWith)
    u16 partnerSlot; // lobby slot of the other player
    u8 state;
    u8 unk_07;
    u16 partnerMessage;
    u8 role; // 0 = none, 1 = asked the other player, 2 = asked by it (WiFiPlazaWork_IsTalking / WiFiPlazaWork_WasAskedToTalk)
    u8 isFirstTalk;
    u8 talkCounts[20]; // per lobby slot, counts up to 6 (WiFiPlazaTalkSession_IncTalkCount; WiFiPlazaWork_CanTalkWith)
    u16 sendMessage;   // sent with WiFiPlazaLobbyClient_SendToPlayer (4 bytes from here)
    u16 sendState;
    u16 timeoutActive;  // WiFiPlazaTalkSession_StartTimeout sets it, WiFiPlazaTalkSession_StopTimeout clears it
    s16 timeoutCounter; // counts up to 900 while timeoutActive is set (WiFiPlazaTalkSession_UpdateTimeout)
} WiFiPlazaTalkSession;

// A message to the lobby (WiFiPlazaLobbyClient_SendToLobby type 4).
typedef struct WiFiPlazaLobbyMessage {
    u32 ids[4]; // WiFiPlazaLobbyClient_SlotToId of lobby slots
    u8 count;   // of ids
    u8 type;
    u8 unk_12;
    u8 unk_13_0 : 7;
    u8 unk_13_7 : 1;
} WiFiPlazaLobbyMessage;

// Part of WiFiPlazaWork at 0x1FC (WiFiPlazaEventSchedule_Update / WiFiPlazaEventSchedule_Init / WiFiPlazaEventSchedule_ApplyVolume work on it).
typedef struct WiFiPlazaEventSchedule {
    u8 flag0 : 1;
    u8 flag1 : 1;
    u8 mode2 : 2;
    u8 flag4 : 1;
    u8 mode5 : 2;
    u8 flag7 : 1;
    u8 colorEvent;
    u8 light1Event;
    u8 thirdEvent;
    s16 closeTimer;
    s16 lightingDelay;
    s16 fireworksDelay;
    s16 paradeDelay;
    u8 unk_0c;
    u8 lowVolume;
    u8 bgmFadedOut;
    u8 low : 4;
    u8 high : 4;
} WiFiPlazaEventSchedule;

// Part of WiFiPlazaWork at 0x3A0.
typedef struct WiFiPlazaUnk3A0 {
    s16 timer; // ov45_0222BCA0 starts it, ov45_0222BCB8 counts it down
    u8 unk_02; // ov45_0222BD24 / ov45_0222BD2C
    u8 unk_03;
    u8 flags[8]; // ov45_0222B118 sets one, ov45_0222B134 finds a clear one
} WiFiPlazaUnk3A0;

// Part of WiFiPlazaWork at 0x49C: a timer in 1/30 s of the lobby time (WiFiPlazaTimer_Init sets it up, WiFiPlazaTimer_Start
// starts it, WiFiPlazaTimer_Update runs it).
typedef struct WiFiPlazaTimer {
    u32 active;
    s64 start; // WiFiPlazaLobbyClient_GetTime
    s32 elapsed;
    s32 duration;
    u32 phase;        // _02254A28[].phase, then 5 at the end
    u32 counter;      // updates in the current phase
    void *playerInfo; // WiFiPlazaPlayerInfo_New (ov45_0222B244 / ov45_0222B270)
} WiFiPlazaTimer;

// A phase of WiFiPlazaTimer and its share of the duration (in 1/256).
typedef struct WiFiPlazaTimerPhase {
    u16 phase;
    u16 length;
} WiFiPlazaTimerPhase;

// Part of WiFiPlazaWork at 0x3AC: the chance of each group of WiFiPlazaWork.groups (WiFiPlazaGroupWeights_PickGroup).
typedef struct WiFiPlazaGroupWeights {
    u8 weights[9];
    u8 unk_09[3];
    u8 seen[20]; // per lobby slot (WiFiPlazaGroupWeights_ReportPlace)
} WiFiPlazaGroupWeights;

// Four words per lobby slot, made from a code (WiFiPlazaWordSet_Set).
typedef struct WiFiPlazaWordSet {
    u8 flags[20];
    u16 words[20][4];
    WallpaperPasswordBank *bank;
} WiFiPlazaWordSet;

// The survey tally of this plaza, cached by the survey results screen (WiFiPlazaSurveyTally, 0x14 bytes).
typedef struct WiFiPlazaTallyCache {
    u32 words[5];
} WiFiPlazaTallyCache;

// Part of WiFiPlazaWork at 0x508 (WiFiPlaza_NewPlayerProfile / WiFiPlaza_FreePlayerProfile set it up and free it).
typedef struct WiFiPlazaUnk508 {
    PlayerProfile *profile; // the own profile as the lobby sees it (WiFiPlazaProfile_CopyToPlayerProfile)
    void *unk_04;
    WiFiPlazaTallyCache tally; // WiFiPlazaWork_SetSurveyTally / WiFiPlazaWork_GetSurveyTally
    BOOL hasTally;
} WiFiPlazaUnk508;

// Plaza work data shared by the sub-applications (WiFiPlazaWork_New).
// Part of WiFiPlazaWork at 0x20C: nine groups of three lobby slots (ov45_0222C388 clears it).
typedef struct WiFiPlazaGroups {
    u32 active; // ov45_0222C3A8 sets it
    s32 time;
    s32 counters[9];
    u32 states[9];
    u32 ids[9][3]; // -1 = free (ov45_0222C388)
    u32 unk_bc[9][3];
    u32 unk_128[9][3];
} WiFiPlazaGroups;

// A time of day (WiFiPlaza_SecondsToTime / WiFiPlazaTime_Add / WiFiPlazaTime_Subtract).
typedef union WiFiPlazaTime {
    u32 word;

    struct {
        u8 hour;
        u8 minute;
        u8 second;
        u8 unk_03;
    } field;

    u8 value[4];
    s8 signedValue[4];
} WiFiPlazaTime;

// A country and region (WiFiPlazaPlaces_Add adds them to a list of 50).
typedef struct WiFiPlazaPlace {
    u16 country;
    u8 region;
    u8 value : 4;
    u8 occupied : 4;
} WiFiPlazaPlace;

// Part of WiFiPlazaWork at 0x4BC: the last activities of the own player (id < 24, 24 = none; value -1 = none), a ring
// of 13 entries; crc covers everything before save (WiFiPlazaHistory_UpdateChecksum / WiFiPlazaHistory_CheckChecksum).
typedef struct WiFiPlazaHistory {
    s32 values[13];
    u8 ids[13];
    u8 oldest;
    u8 next;
    SaveData *save;
    u16 crc;
    u16 corrupt;
} WiFiPlazaHistory;

// The applications of the plaza (wifi_plaza_apps.c), one overlay at a time.
typedef struct WiFiPlazaApps {
    OverlayManager *manager;
    void *args[13]; // per app
    u8 current;     // app
    u8 skipPrompt;
    u16 heapID;
    struct WiFiPlazaWork *work;
    SaveData *save;
    void *loginTime;
} WiFiPlazaApps;

// What the lobby request handlers (wifi_plaza_requests.c) pass to the plaza news (with the player profiles in
// WiFiPlazaWork.profiles).
typedef struct WiFiPlazaPairRequest {
    PlayerProfile *profiles[2];
    u16 slots[2];
} WiFiPlazaPairRequest;

typedef struct WiFiPlazaPairOptionRequest {
    PlayerProfile *profiles[2];
    u16 slots[2];
    u32 option;
} WiFiPlazaPairOptionRequest;

typedef struct WiFiPlazaGroupRequest {
    u32 count;
    PlayerProfile *profiles[4];
    u16 slots[4];
} WiFiPlazaGroupRequest;

typedef struct WiFiPlazaGroupFlagRequest {
    u32 option;
    u32 count;
    PlayerProfile *profiles[4];
    u16 slots[4];
    u32 flag;
} WiFiPlazaGroupFlagRequest;

typedef struct WiFiPlazaSingleRequest {
    u32 option;
    u32 count;
    PlayerProfile *profile;
    u16 slot;
} WiFiPlazaSingleRequest;

typedef struct WiFiPlazaGroupOptionRequest {
    u32 option;
    u32 count;
    PlayerProfile *profiles[4];
    u16 slots[4];
} WiFiPlazaGroupOptionRequest;

// What WiFiPlazaWork_AddLobbyEventNotice passes to WiFiPlazaNews_AddLobbyEventNotice.
typedef struct WiFiPlazaLobbyNotice {
    u32 *time;
    u32 which;
} WiFiPlazaLobbyNotice;

// A lobby slot in the plaza news.
typedef struct WiFiPlazaNewsSlot {
    u16 state; // 2: empty
    u16 flags; // bit 1: the slot of WiFiPlazaNews_SetOldestPlayer
} WiFiPlazaNewsSlot;

typedef struct WiFiPlazaNewsSlots {
    WiFiPlazaNewsSlot slots[20];
    u16 change;   // 1: a player came, 2: a player left (WiFiPlazaNewsSlots_SetChange)
    u16 lastSlot; // WiFiPlazaNewsSlots_SetLastJoined
} WiFiPlazaNewsSlots;

// An event (topic) of the plaza news: type 0-8 (message ov45_02254C0C[type]), up to four player names and slots.
typedef struct WiFiPlazaNewsEvent {
    u8 type;
    u32 a;
    u32 b;
    u32 c;
    String *names[4];
    u16 slots[4];
    u16 rank;
    s16 timer;
    struct WiFiPlazaNewsEvent *next;
    struct WiFiPlazaNewsEvent *prev;
} WiFiPlazaNewsEvent;

typedef struct WiFiPlazaNewsEventList {
    WiFiPlazaNewsEvent events[8];
    WiFiPlazaNewsEvent head; // of the list ordered by rank
} WiFiPlazaNewsEventList;

// The plaza news (WiFiPlazaWork.news, WiFiPlazaNews_New), handed to the plaza news app.
typedef struct WiFiPlazaNews {
    u32 openTime; // WiFiPlazaTime of the lobby opening (WiFiPlazaNews_SetOpenTime)
    u32 isOpen;
    WiFiPlazaNewsSlots slots;
    WiFiPlazaNewsEventList events;
} WiFiPlazaNews;

typedef struct WiFiPlazaWork {
    SaveData *save;
    WiFiPlazaNews *news;
    u32 lightingStarted;
    WiFiPlazaPlace places[50]; // filled by WiFiPlaza_FillPlaces from the profile
    u32 currentTime;
    s64 lobbyTime;     // WiFiPlazaLobbyClient_GetTime, refreshed every frame
    s64 lobbyOpenTime; // ov45_022320C4 when the lobby opened (WiFiPlaza_OnLobbyEvent 0)
    PlayerProfile *profiles[4];
    u32 changeFlags[4]; // per lobby slot: entered, left, events, updated (WiFiPlazaWork_TakePlayerEntered / A25C / A288 / A2A0)
    WiFiPlazaProfile profile;
    WiFiPlazaTalkSession talk;
    u8 unk_1e8[0x14];
    WiFiPlazaEventSchedule eventSchedule;
    WiFiPlazaGroups groups;
    WiFiPlazaUnk3A0 unk_3a0;
    WiFiPlazaGroupWeights groupWeights;
    u8 specialVisitors[0x14]; // per lobby slot (WiFiPlaza_SetSlotFlag); copied out by WiFiPlazaWork_GetSpecialVisitors
    u8 gameMembers[4];
    WiFiPlazaWordSet wordSet;
    WiFiPlazaTimer timer;
    WiFiPlazaHistory history;
    WiFiPlazaUnk508 unk_508;
    enum HeapID heapID;
    u32 hasError;
} WiFiPlazaWork;

// Location tables of the earth place NARC (as in geonet_globe_app.c); y is the longitude angle.
typedef struct WiFiPlazaEarthCountry {
    u16 kind; // 2: the country has regions
    s16 x;
    s16 y;
} WiFiPlazaEarthCountry;

typedef struct WiFiPlazaEarthRegion {
    s16 x;
    s16 y;
} WiFiPlazaEarthRegion;

// Same layout as the box test bounds of model_box_test.c.
typedef struct WiFiPlazaBoxTestBounds {
    s16 width;
    s16 height;
    s16 depth;
    s16 pad06;
    fx32 scaleX;
    fx32 scaleY;
    fx32 scaleZ;
} WiFiPlazaBoxTestBounds;

// Lobby callbacks given to WiFiPlazaLobbyClient_Init (with the work as the last argument).
typedef struct WiFiPlazaCallbacks {
    void (*onPlayerEnter)(int id, WiFiPlazaPlayer *player, WiFiPlazaWork *work, BOOL isOwn);
    void (*onPlayerLeave)(int id, WiFiPlazaWork *work);
    void (*onPlayerUpdate)(int id, WiFiPlazaPlayer *player, WiFiPlazaWork *work);
    void (*onLobbyEvent)(u32 event, WiFiPlazaWork *work);
    void (*onOwnRecord)(const WiFiPlazaPlayer *data, u32 size, WiFiPlazaWork *work);
} WiFiPlazaCallbacks;

// The time a player entered the lobby (an s64 kept as two words, WiFiPlazaLobbyClient_GetPlayerRecord).
typedef struct WiFiPlazaLobbyEntryTime {
    u32 lo;
    u32 hi;
} WiFiPlazaLobbyEntryTime;

// What the lobby keeps per player (WiFiPlazaLobbyClient_SetPlayerRecord / WiFiPlazaLobbyClient_GetPlayerRecord).
typedef struct WiFiPlazaLobbyPlayerRecord {
    WiFiPlazaLobbyEntryTime entryTime;
    u32 channel; // lobby library channel (WiFiPlazaLobbyClient_ChannelToLib), 5 = none
} WiFiPlazaLobbyPlayerRecord;

// The ids of the players in the lobby (list 0) and in the own channel (list 1), -1 = free.
typedef struct WiFiPlazaLobbyIdList {
    s16 count;
    u16 capacity;
    s32 *ids;
} WiFiPlazaLobbyIdList;

// A message type of the lobby client (WiFiPlazaLobbyClient_SetLobbyCommands / _SetChannelCommands): its receive
// callback and the size of its data.
typedef struct WiFiPlazaLobbyCommand {
    void (*callback)(int id, const void *data, u32 size, void *arg);
    u32 size;
} WiFiPlazaLobbyCommand;

typedef struct WiFiPlazaLobbyCommandTable {
    const WiFiPlazaLobbyCommand *commands;
    u32 count;
    void *arg;
} WiFiPlazaLobbyCommandTable;

// Country and region of a lobby slot (WiFiPlazaLobbyClient_GetLocationCountry / WiFiPlazaLobbyClient_GetLocationRegion).
typedef struct WiFiPlazaLobbyLocation {
    u16 country;
    u8 region;
    u8 active;
} WiFiPlazaLobbyLocation;

// What the host of a matchmaking session sends about it (WiFiPlazaLobbyClient_OnMatchInfo).
typedef struct WiFiPlazaLobbyMatchInfo {
    int index;
    int maxPlayers;
    int playerCount; // ov00_021E7144 for the own session
    int state;       // 1 once the host closed it (WiFiPlazaLobbyClient_CloseMatch)
    WiFiPlazaLobbyEntryTime startTime;
    u8 unk_18[0x18];
    char name[0x14];
} WiFiPlazaLobbyMatchInfo;

// A matchmaking session of the lobby client: WiFiPlazaLobbyClient_HostMatch hosts it, WiFiPlazaLobbyClient_JoinMatch joins it (ov00_021E5CEC with its
// name, its player count and timer / 30 seconds).
typedef struct WiFiPlazaLobbyMatch {
    WiFiPlazaLobbyMatchInfo info;
    u16 active;
    u16 timer;  // frames left, 1800 at the start
    int hostId; // own id when hosting (WiFiPlazaLobbyClient_HostMatch)
} WiFiPlazaLobbyMatch;

typedef struct WiFiPlazaLobbyKeyValue {
    u32 value;
    u32 key;
} WiFiPlazaLobbyKeyValue;

typedef struct WiFiPlazaLobbyIdValue {
    u32 id;
    u32 value;
} WiFiPlazaLobbyIdValue;

// Overlays the first 8 bytes of WiFiPlazaLobbyClient.unk_6a0 (WiFiPlazaLobbyIdValueList_Init).
typedef struct WiFiPlazaLobbyIdValueList {
    WiFiPlazaLobbyIdValue *items;
    u16 capacity;
    volatile u16 count;
} WiFiPlazaLobbyIdValueList;

typedef struct WiFiPlazaLobbyClientUnk1B4 {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08; // bit 0: ov45_0222ECDC field 4
    u8 unk_0c;
    u8 unk_0d;
    volatile u16 count;
    WiFiPlazaLobbyKeyValue values[126]; // ov45_022303BC
} WiFiPlazaLobbyClientUnk1B4;

// ov45_0222F544 / ov45_0222F628 / ov45_0222F6B0 read the two halves.
typedef struct WiFiPlazaLobbyClientUnk6A4 {
    u32 unk_00;
    u32 unk_04;
    s32 unk_08;
    u8 unk_0c[0xDC];
    u8 unk_e8[3][0x24];
    u8 unk_154[12];
} WiFiPlazaLobbyClientUnk6A4;

typedef struct WiFiPlazaLobbyClientUnk6A0 {
    u32 unk_00;
    WiFiPlazaLobbyClientUnk6A4 unk_04[2];
    u32 unk_2c4;
    u32 unk_2c8[3];
    u32 unk_2d4[3];
} WiFiPlazaLobbyClientUnk6A0;

// A player on the plaza map (overlay 49): a billboard sprite of the field character models with a model under it.
typedef struct WiFiPlazaPlayerObject {
    u8 mode : 4;         // 0: plays `effect` (WiFiPlazaPlayerObject_UpdateEffect), else follows the player's action (WiFiPlazaPlayerObject_FollowPlayer)
    u8 culled : 2;       // WiFiPlazaPlayerObject_UpdateCulling
    u8 visible : 2;      // WiFiPlazaPlayerObject_SetVisible
    u8 effectActive : 1; // WiFiPlazaPlayerObject_StartEffect
    u8 effect : 7;       // index into ov45_02254F1C
    u8 timer;
    u8 step;
    Field2DObject *player; // NULL = free slot
    void *sprite;                    // billboard object
    ModelInstance model;
    u8 action;        // Field2DObject_GetParam field 5
    u8 savedSequence; // sequence and frame when an action of ov45_02230DC4 ended
    u16 actionFrame;  // Field2DObject_GetParam field 8
    u32 savedFrame;
    u32 lightFlags;
} WiFiPlazaPlayerObject;

typedef struct WiFiPlazaObjectModel {
    ModelResource resource;
    int alpha; // 0 = the models are not drawn (WiFiPlazaPlayerObjects_Draw)
} WiFiPlazaObjectModel;

// The player objects (WiFiPlazaPlayerObjects_New).
typedef struct WiFiPlazaPlayerObjects {
    GF_2DGfxRawResMan *models;    // the billboard model
    GF_2DGfxRawResMan *sequences; // 2 sprite sequences (ov45_02254C38)
    GF_3DGfxRawResMan *textures;  // 20 sprites (ov45_02254C48)
    void *billboards;
    WiFiPlazaPlayerObject *objects;
    u32 count;
    WiFiPlazaObjectModel model;
    u32 lightFlags; // of the billboard model
} WiFiPlazaPlayerObjects;

typedef struct WiFiPlazaSpriteConfig {
    u16 spriteId; // Field2DObject_GetParam field 7
    u16 fileId : 15;
    u16 ownTexture : 1; // texture loaded per object instead of once into VRAM
} WiFiPlazaSpriteConfig;

typedef struct WiFiPlazaBillboardParams {
    u32 count;
    u32 heapID;
} WiFiPlazaBillboardParams;

// Same layout as the billboard code's gfx sequence (billboard_gfx_sequence.c).
typedef struct WiFiPlazaBillboardSequence {
    const u16 *startFrame;
    const u8 *textureIdx;
    const u8 *plttIdx;
    u32 seqCount;
} WiFiPlazaBillboardSequence;

typedef struct WiFiPlazaBillboardResource {
    void *modelFile;
    NNSG3dResTex *texture;
    const void *ranges;
    WiFiPlazaBillboardSequence sequence;
    NNSG3dTexKey texKey;
    NNSG3dTexKey tex4x4Key;
    NNSG3dPlttKey plttKey;
} WiFiPlazaBillboardResource;

typedef struct WiFiPlazaBillboardCreateParams {
    void *billboards;
    WiFiPlazaBillboardResource *resource;
    VecFx32 position;
    VecFx32 scale;
} WiFiPlazaBillboardCreateParams;

// The lobby client (_022577C0, wifi_plaza_lobby_client.c).
typedef struct WiFiPlazaLobbyClient {
    SaveData *save;
    FriendRoster *friendRoster;
    SaveWiFiHistory *wifiHistory;
    WiFiPlazaCallbacks callbacks;
    void *callbackArg;  // last argument of the callbacks
    u8 *playerData;     // playerDataSize bytes per lobby slot
    u32 playerDataSize; // WiFiPlazaLobbyClient_AllocPlayerData
    WiFiPlazaLobbyPlayerRecord records[20];
    WiFiPlazaLobbyIdList idLists[2];
    u8 state;        // 0 = none, 1 = connecting, 2 = in the lobby, 3, 4 = error (WiFiPlazaLobbyClient_Update)
    u8 channel;      // 4 = none (WiFiPlazaLobbyClient_JoinChannel)
    u8 channelState; // 4 = error
    u8 unk_12f;
    u8 unk_130;
    u8 unk_131;
    u8 unk_132; // error
    u8 unk_133; // error
    WiFiPlazaLobbyLocation locations[20];
    WiFiPlazaLobbyCommandTable commandTables[2]; // lobby, channel
    s64 time;                                    // lobby time, refreshed every frame
    u8 unk_1a4[0x10];
    WiFiPlazaLobbyClientUnk1B4 unk_1b4; // received with ov45_02232170 (WiFiPlazaLobbyClient_OnEnterChannel)
    u32 unk_5b4;                        // its size
    WiFiPlazaLobbyMatch matches[3];
    u16 currentMatch; // 3 = none
    u8 unk_69e;
    u8 unk_69f;
    WiFiPlazaLobbyClientUnk6A0 unk_6a0;
    u32 unk_980;
    int initResult; // ov45_022310C0
} WiFiPlazaLobbyClient;

// wifi_plaza_work.c
void ov45_0222A1F8(WiFiPlazaWork *work);
u32 WiFiPlazaWork_HasError(WiFiPlazaWork *work);
void ov45_0222A208(void *a0, void *a1, void *a2);
void *WiFiPlazaWork_GetNews(WiFiPlazaWork *work);
void *WiFiPlazaWork_GetPlaces(WiFiPlazaWork *work);
u32 *ov45_0222A22C(WiFiPlazaWork *work);
BOOL WiFiPlazaWork_TakePlayerEntered(WiFiPlazaWork *work, int bit);
BOOL WiFiPlazaWork_TakePlayerLeft(WiFiPlazaWork *work, int bit);
BOOL WiFiPlazaWork_IsEventFlagSet(WiFiPlazaWork *work, int bit);
BOOL WiFiPlazaWork_TakePlayerUpdated(WiFiPlazaWork *work, int bit);
void *ov45_0222A2C8(void **ptr);
int ov45_0222A2CC(WiFiPlazaWork *work);
u32 ov45_0222A2E0(WiFiPlazaWork *work);
u32 ov45_0222A2F8(WiFiPlazaWork *work);
void ov45_0222A310(WiFiPlazaWork *work);
u32 WiFiPlazaWork_IsLobbyOpen(WiFiPlazaWork *work);
u32 WiFiPlazaWork_IsClosing(WiFiPlazaWork *work);
BOOL WiFiPlazaWork_IsClosed(WiFiPlazaWork *work);
u32 WiFiPlazaWork_GetFireworksState(WiFiPlazaWork *work);
BOOL WiFiPlazaWork_IsParadeStarted(WiFiPlazaWork *work);
u32 ov45_0222A394(WiFiPlazaWork *work);
BOOL WiFiPlazaWork_IsLightingStarted(WiFiPlazaWork *work);
u32 WiFiPlazaWork_GetColorEvent(WiFiPlazaWork *work);
u32 WiFiPlazaWork_GetLight1Event(WiFiPlazaWork *work);
u32 ov45_0222A3EC(WiFiPlazaWork *work);
void ov45_0222A404(WiFiPlazaWork *work);
int ov45_0222A414(WiFiPlazaWork *work);
void *ov45_0222A424(WiFiPlazaWork *work);
void ov45_0222A430(WiFiPlazaWork *work, void *value);
void WiFiPlazaWork_ClearGameMembers(WiFiPlazaWork *work);
void WiFiPlazaWork_SetGameMember(WiFiPlazaWork *work, int id, u32 index);
void WiFiPlazaWork_SetOwnGameMember(WiFiPlazaWork *work, u32 index);
void WiFiPlazaWork_GetGameMembers(WiFiPlazaWork *work, void *dest);
void ov45_0222A4A8(WiFiPlazaWork *work);
int ov45_0222A4B8(WiFiPlazaWork *work);
void ov45_0222A4C8(WiFiPlazaWork *work, u8 value);

// wifi_plaza_player.c
void WiFiPlazaWork_PlayBGM(WiFiPlazaWork *work);
void WiFiPlazaWork_SetLowVolume(WiFiPlazaWork *work, int value);
int WiFiPlazaWork_GetOwnSlot(void *unused);
void WiFiPlazaWork_GetOwnId(void);
BOOL WiFiPlazaWork_IsSlotOccupied(void *unused, int index);
WiFiPlazaPlayer *WiFiPlazaWork_GetPlayer(WiFiPlazaWork *work, u32 index);
WiFiPlazaPlayer *WiFiPlazaWork_GetMyPlayer(WiFiPlazaWork *work);
void ov45_0222A5E8(WiFiPlazaWork *work, int value);
void WiFiPlazaWork_AddHistory(WiFiPlazaWork *work, int index, int value);
void ov45_0222A72C(WiFiPlazaWork *work, u32 value);
void ov45_0222A770(WiFiPlazaWork *work, int a, int b);
void ov45_0222A7DC(WiFiPlazaWork *work, u32 a, u32 b);
void WiFiPlazaPlayer_CopyToProfile(WiFiPlazaPlayer *player, PlayerProfile *profile, enum HeapID heapID);
int ov45_0222A920(WiFiPlazaPlayer *player);
u8 WiFiPlazaPlayer_GetHistoryId(WiFiPlazaPlayer *player, u32 index);
void *WiFiPlazaPlayer_GetHistoryValue(WiFiPlazaPlayer *player, u32 index);
u32 WiFiPlazaPlayer_GetLobbyId(WiFiPlazaPlayer *player);
u32 WiFiPlazaPlayer_GetTrainerID(WiFiPlazaPlayer *player);
int WiFiPlazaPlayer_GetAvatarIndex(WiFiPlazaPlayer *player);
void *WiFiPlazaPlayer_GetLocalTime(WiFiPlazaPlayer *player);
u32 WiFiPlazaPlayer_GetGender(WiFiPlazaPlayer *player);
u32 WiFiPlazaPlayer_GetLanguage(WiFiPlazaPlayer *player);
BOOL WiFiPlazaPlayer_IsLanguageValid(WiFiPlazaPlayer *player);
u8 WiFiPlazaPlayer_GetRawLanguage(WiFiPlazaPlayer *player);
u16 WiFiPlazaPlayer_GetAvatar(WiFiPlazaPlayer *player);
u16 WiFiPlazaPlayer_GetCountry(WiFiPlazaPlayer *player);
u8 WiFiPlazaPlayer_GetRegion(WiFiPlazaPlayer *player);
u8 ov45_0222AAC8(WiFiPlazaPlayer *player);
BOOL ov45_0222AADC(WiFiPlazaPlayer *player);
u16 ov45_0222AAEC(WiFiPlazaPlayer *player, u32 index);
void ov45_0222AB0C(WiFiPlazaPlayer *player, WiFiPlazaPlayerSurvey *out);

// wifi_plaza_lobby.c
void *WiFiPlazaWork_GetOwnPlayerProfile(WiFiPlazaWork *work);
int WiFiPlazaWork_IsSpecialVisitor(WiFiPlazaWork *work, int a1);
void WiFiPlazaWork_GetSpecialVisitors(WiFiPlazaWork *work, void *dest);
int WiFiPlazaWork_HasWordSet(WiFiPlazaWork *work, int a1);
u16 *WiFiPlazaWork_GetWordSet(WiFiPlazaWork *work, int a1);
u32 WiFiPlaza_GetChannelPlayerCount(void);
int WiFiPlaza_ChannelSlotToSlot(void *unused, int a1);
void WiFiPlaza_SendChatNews(void *unused, u32 slot0, u32 slot1);
void WiFiPlaza_SendTapToyTradeNews(void *unused, u32 slot0, u32 slot1, u8 value);
void WiFiPlaza_SendActivityNews(void *unused, int selector, int count, int slot0, int slot1, int slot2, int slot3, int flag);
void WiFiPlaza_SendMinigameResultNews(void *unused, int mode, u8 kind, u32 slot0, u32 slot1, u32 slot2, u32 slot3);
int ov45_0222AD2C(WiFiPlazaWork *work);
int ov45_0222AD3C(WiFiPlazaWork *work);
int WiFiPlazaWork_GetOpenDuration(WiFiPlazaWork *work);
u8 ov45_0222AD58(WiFiPlazaWork *work, u32 index);
void ov45_0222AD70(void *unused, u32 value);
int ov45_0222AD80(WiFiPlazaWork *work, int a1);
int ov45_0222AD90(WiFiPlazaWork *work, int a1);
int ov45_0222ADA0(void);
int ov45_0222ADA8(WiFiPlazaWork *work, int a1);
void ov45_0222ADB8(WiFiPlazaWork *work, int a1, int a2);
void ov45_0222ADC8(WiFiPlazaWork *work, int a1);
void ov45_0222ADD8(WiFiPlazaWork *work, int a1);
void ov45_0222ADE8(WiFiPlazaWork *work, int a1);
void ov45_0222ADF8(WiFiPlazaWork *work, int a1);
void ov45_0222AE08(u32 value, u32 *quotient, u32 *remainder);
void ov45_0222AE24(WiFiPlazaWork *work, int a1, int a2);
void ov45_0222AE34(WiFiPlazaWork *work, int a1);
void ov45_0222AE44(void);
void ov45_0222AE54(void);
void WiFiPlazaWork_ResetTalk(WiFiPlazaWork *work);
int WiFiPlazaWork_StartTalk(WiFiPlazaWork *work, int slot);
void WiFiPlazaWork_AdvanceTalk(WiFiPlazaWork *work, int a1);
void WiFiPlazaWork_CancelTalkRequest(WiFiPlazaWork *work);
void WiFiPlazaWork_EndTalk(WiFiPlazaWork *work);
BOOL WiFiPlazaWork_IsTalking(WiFiPlazaWork *work);
BOOL WiFiPlazaWork_WasAskedToTalk(WiFiPlazaWork *work);
u16 WiFiPlazaWork_GetTalkPartner(WiFiPlazaWork *work);
u8 WiFiPlazaWork_IsFirstTalk(WiFiPlazaWork *work);
u8 WiFiPlazaWork_GetTalkState(WiFiPlazaWork *work);
u16 ov45_0222B040(WiFiPlazaWork *work);
BOOL WiFiPlazaWork_CanTalkWith(WiFiPlazaWork *work, u32 index);
BOOL WiFiPlazaWork_IsTalkOver(WiFiPlazaWork *work);
void ov45_0222B094(WiFiPlazaWork *work);
void ov45_0222B0A4(WiFiPlazaWork *work);
u8 ov45_0222B0B0(WiFiPlazaWork *work);

// wifi_plaza_state.c
void WiFiPlazaWork_IncPartnerTalkCount(WiFiPlazaWork *work);
void WiFiPlazaWork_IncTalkCount(WiFiPlazaWork *work, int slot);
void WiFiPlazaWork_SetTalkedWith(WiFiPlazaWork *work, int slot);
void IncrementGameStat47(WiFiPlazaWork *work);
void IncrementGameStat119(WiFiPlazaWork *work);
void ov45_0222B118(WiFiPlazaWork *work, int slot);
int ov45_0222B134(WiFiPlazaWork *work);
int ov45_0222B1B4(WiFiPlazaWork *work);
u32 WiFiPlazaWork_GetFireworksPhase(WiFiPlazaWork *work);
u32 WiFiPlazaWork_GetFireworksCounter(WiFiPlazaWork *work);
void WiFiPlazaWork_SetSurveyTally(WiFiPlazaWork *work, const WiFiPlazaTallyCache *tally);
BOOL WiFiPlazaWork_GetSurveyTally(WiFiPlazaWork *work, WiFiPlazaTallyCache *tally);
void ov45_0222B244(WiFiPlazaWork *work);
void ov45_0222B270(WiFiPlazaWork *work);
BOOL WiFiPlaza_IsNameTerminated(const u16 *name, u32 count, enum HeapID heapID);
void WiFiPlaza_OnPlayerEnter(int id, WiFiPlazaPlayer *player, WiFiPlazaWork *work, BOOL isOwn);
void WiFiPlaza_OnPlayerLeave(int id, WiFiPlazaWork *work);
void WiFiPlaza_OnPlayerUpdate(int id, WiFiPlazaPlayer *player, WiFiPlazaWork *work);
void WiFiPlaza_OnLobbyEvent(u32 event, WiFiPlazaWork *work);
void WiFiPlaza_OnOwnRecord(const WiFiPlazaPlayer *data, u32 size, WiFiPlazaWork *work);
void WiFiPlaza_FillPlaces(WiFiPlazaProfile *profile, WiFiPlazaPlace *out);
void WiFiPlazaWork_CheckClosingTime(WiFiPlazaWork *work);
void WiFiPlazaProfile_InitFromSave(WiFiPlazaProfile *profile, SaveData *save, enum HeapID heapID);
void WiFiPlazaWork_SendOwnRecord(WiFiPlazaWork *work);
void WiFiPlazaProfile_UpdateChecksum(WiFiPlazaProfile *profile, SaveData *save);
BOOL WiFiPlazaProfile_IsChecksumValid(WiFiPlazaProfile *profile, SaveData *save);
void WiFiPlazaProfile_SetFromLobbyRecord(WiFiPlazaProfile *profile, const WiFiPlazaPlayer *src, u32 size);
void WiFiPlaza_ClearEventFlags(u32 *flags);
void WiFiPlazaEventSchedule_Update(WiFiPlazaEventSchedule *state, u32 *flags, WiFiPlazaGroups *a2, WiFiPlazaTimer *a3);
void WiFiPlazaEventSchedule_Init(WiFiPlazaEventSchedule *state);
void WiFiPlazaEventSchedule_ApplyVolume(WiFiPlazaEventSchedule *state);
void ov45_0222BCA0(WiFiPlazaUnk3A0 *a0);
int ov45_0222BCA8(WiFiPlazaUnk3A0 *a0);
void ov45_0222BCB8(WiFiPlazaUnk3A0 *a0);
void WiFiPlaza_NewPlayerProfile(PlayerProfile **profile, enum HeapID heapID);
void WiFiPlaza_FreePlayerProfile(PlayerProfile **profile);
void WiFiPlazaProfile_CopyToPlayerProfile(PlayerProfile **profile, WiFiPlazaProfile *src, enum HeapID heapID);
void ov45_0222BD24(WiFiPlazaUnk3A0 *a0);
u8 ov45_0222BD2C(WiFiPlazaUnk3A0 *a0);

// wifi_plaza.c
BOOL WiFiPlaza_Init(OverlayManager *man, int *state);
BOOL WiFiPlaza_Main(OverlayManager *man, int *state);
BOOL WiFiPlaza_Exit(OverlayManager *man, int *state);
void WiFiPlaza_VBlankTask(SysTask *task, void *data);
WiFiPlazaWork *WiFiPlazaWork_New(SaveData *save, enum HeapID heapID);
void WiFiPlazaWork_Delete(WiFiPlazaWork *work);
void WiFiPlazaWork_Update(WiFiPlazaWork *work);

// wifi_plaza_requests.c
void ov45_0222BD30(volatile u8 *a0);
void WiFiPlazaTalkSession_Init(WiFiPlazaTalkSession *a0);
void WiFiPlazaTalkSession_Reset(WiFiPlazaTalkSession *a0);
void WiFiPlazaTalkSession_Abort(WiFiPlazaTalkSession *a0);
BOOL WiFiPlazaTalkSession_HasTalkedWith(WiFiPlazaTalkSession *a0, u32 slot);
void WiFiPlazaTalkSession_SetTalkedWith(WiFiPlazaTalkSession *a0, u32 slot);
void WiFiPlazaTalkSession_ClearTalkedWith(WiFiPlazaTalkSession *a0, u32 slot);
void WiFiPlazaTalkSession_IncTalkCount(WiFiPlazaTalkSession *a0, u32 slot);
void WiFiPlazaTalkSession_ResetTalkCount(WiFiPlazaTalkSession *a0, u32 slot);
void WiFiPlazaTalkSession_SetMessage(WiFiPlazaTalkSession *a0, int value);
void WiFiPlazaWork_SendTalkAnswer(WiFiPlazaWork *work, int id, u16 value);
void WiFiPlazaWork_SendTalkEnd(WiFiPlazaWork *work, int id);
void WiFiPlazaTalkSession_StartTimeout(WiFiPlazaTalkSession *a0);
void WiFiPlazaTalkSession_StopTimeout(WiFiPlazaTalkSession *a0);
void WiFiPlazaTalkSession_UpdateTimeout(WiFiPlazaTalkSession *a0);
BOOL WiFiPlazaTalkSession_IsWithinTimeout(WiFiPlazaTalkSession *a0);
s16 WiFiPlazaTalkSession_GetTimeoutCounter(WiFiPlazaTalkSession *a0);
void WiFiPlaza_HandleChatNews(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg);
void WiFiPlaza_HandleTapToyTradeNews(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg);
void WiFiPlaza_HandleMinigameEntryNews(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg);
void WiFiPlaza_HandleFootprintStampNews(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg);
void WiFiPlaza_HandleVisitorProfilesNews(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg);
void WiFiPlaza_HandlePlazaNewsNews(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg);
void WiFiPlaza_HandleMinigameResultNews(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg);
void ov45_0222C36C(void);
void WiFiPlazaWork_AddLobbyEventNotice(WiFiPlazaWork *work, int which);
void ov45_0222C388(WiFiPlazaGroups *a0);
void ov45_0222C3A8(WiFiPlazaGroups *a0);

// wifi_plaza_groups.c
void ov45_0222C3B0(WiFiPlazaGroups *groups);
int ov45_0222C408(WiFiPlazaGroups *groups, u32 id, u32 value);
void ov45_0222C480(WiFiPlazaGroups *groups, u32 id);
int ov45_0222C4B4(WiFiPlazaGroups *groups, u32 place);
int ov45_0222C4E4(WiFiPlazaGroups *groups, u32 group);
int ov45_0222C4FC(WiFiPlazaGroups *groups, u32 group);
void ov45_0222C514(WiFiPlazaGroups *groups, u32 id);
int ov45_0222C54C(WiFiPlazaGroups *groups, u32 place);
void ov45_0222C580(WiFiPlazaGroups *groups, u32 place);
int ov45_0222C5B4(WiFiPlazaGroups *groups, int id);
void ov45_0222C5E8(WiFiPlazaGroups *groups, u32 place, u32 value);
BOOL ov45_0222C61C(WiFiPlazaGroups *groups, u32 place, u32 value);
u32 ov45_0222C658(WiFiPlazaGroups *groups, u32 place);

// wifi_plaza_receive.c
void WiFiPlaza_OnTalkRequest(int id, u16 *data, u32 size, WiFiPlazaWork *work);
void WiFiPlaza_OnTalkAnswer(int id, u16 *data, u32 size, WiFiPlazaWork *work);
void WiFiPlaza_OnTalkState(int id, u16 *data, u32 size, WiFiPlazaWork *work);
void WiFiPlaza_OnTalkCancel(int id, u16 *data, u32 size, WiFiPlazaWork *work);
void WiFiPlaza_OnLobbyMessage(int id, WiFiPlazaLobbyMessage *msg, u32 size, WiFiPlazaWork *work);

// wifi_plaza_word_set.c
void ov45_0222C83C(int id, void *data, u32 size, WiFiPlazaWork *work);
void ov45_0222C858(int id, void *data, u32 size, WiFiPlazaWork *work);
void ov45_0222C88C(int id, void *data, u32 size, WiFiPlazaWork *work);
void WiFiPlazaGroupWeights_Init(WiFiPlazaGroupWeights *weights);
void WiFiPlazaGroupWeights_ReportPlace(WiFiPlazaGroupWeights *weights, u32 place, u32 slot);
u32 WiFiPlazaGroupWeights_PickGroup(WiFiPlazaGroupWeights *weights);
void WiFiPlaza_SetSlotFlag(u8 *flags, u32 slot, u8 value);
u32 WiFiPlaza_GetSlotFlag(u8 *flags, u32 slot);
void WiFiPlazaWordSet_Init(WiFiPlazaWordSet *wordSet, enum HeapID heapID);
void WiFiPlazaWordSet_Free(WiFiPlazaWordSet *wordSet);
void WiFiPlazaWordSet_Set(WiFiPlazaWordSet *wordSet, u32 slot, u8 flag, u32 code);
u32 WiFiPlazaWordSet_HasWords(WiFiPlazaWordSet *wordSet, u32 slot);
u16 *WiFiPlazaWordSet_GetWords(WiFiPlazaWordSet *wordSet, u32 slot);
void WiFiPlaza_MakeWordsFromCode(WallpaperPasswordBank *bank, u32 code, u16 *words);
void WiFiPlazaTimer_Init(WiFiPlazaTimer *timer, int start, int end);
void WiFiPlazaTimer_Start(WiFiPlazaTimer *timer);
void WiFiPlazaTimer_Update(WiFiPlazaTimer *timer);
u32 WiFiPlazaTimer_GetCounter(WiFiPlazaTimer *timer);
u32 WiFiPlazaTimer_GetPhase(WiFiPlazaTimer *timer);

// wifi_plaza_history.c
void WiFiPlazaHistory_Init(WiFiPlazaHistory *history, SaveData *save);
void WiFiPlazaHistory_Add(WiFiPlazaHistory *history, int id, s32 value);
BOOL WiFiPlazaHistory_ReplaceValue(WiFiPlazaHistory *history, s32 oldValue, s32 newValue);
u8 WiFiPlazaHistory_RemoveOldest(WiFiPlazaHistory *history);
u8 WiFiPlazaHistory_GetId(WiFiPlazaHistory *history, u32 index);
s32 WiFiPlazaHistory_GetValue(WiFiPlazaHistory *history, u32 index);
void WiFiPlazaHistory_UpdateChecksum(WiFiPlazaHistory *history);
void WiFiPlazaHistory_CheckChecksum(WiFiPlazaHistory *history);
u16 WiFiPlazaHistory_IsCorrupt(WiFiPlazaHistory *history);

// wifi_plaza_apps.c
int WiFiPlaza_AvatarToIndex(u16 avatar);
u16 WiFiPlaza_IndexToAvatar(u32 index);
WiFiPlazaApps *WiFiPlazaApps_New(u32 skipPrompt, SaveData *save, void *loginTime, WiFiPlazaWork *work, enum HeapID heapID);
void WiFiPlazaApps_Delete(WiFiPlazaApps *apps);
void WiFiPlazaApps_StartFirst(WiFiPlazaApps *apps);
BOOL WiFiPlazaApps_Run(WiFiPlazaApps *apps);
void WiFiPlazaApps_VBlank(WiFiPlazaApps *apps);
void WiFiPlazaApps_StartOverlay(WiFiPlazaApps *apps, void **args, const OverlayManagerTemplate *template);
void WiFiPlazaApps_InitArgs(WiFiPlazaApps *apps, u32 app, enum HeapID heapID);
void WiFiPlazaApps_FreeArgs(WiFiPlazaApps *apps, u32 app);
void WiFiPlazaApps_Start(WiFiPlazaApps *apps, u32 app);
BOOL WiFiPlazaApps_End(WiFiPlazaApps *apps, u32 app);
void WiFiPlazaApps_StartDisconnectOverlay(WiFiPlazaApps *apps, void **args, u32 mode);
void WiFiPlaza_SecondsToTime(u32 *out, s64 *seconds);

// wifi_plaza_util.c
void WiFiPlazaTime_Add(const WiFiPlazaTime *left, const WiFiPlazaTime *right, WiFiPlazaTime *out);
void WiFiPlazaTime_Subtract(const WiFiPlazaTime *first, const WiFiPlazaTime *second, WiFiPlazaTime *out);
void WiFiPlazaPlaces_Add(WiFiPlazaPlace *places, u32 country, u32 region, u32 value);
u16 WiFiPlazaPlaces_GetCountry(WiFiPlazaPlace *places, u32 index);
u8 WiFiPlazaPlaces_GetRegion(WiFiPlazaPlace *places, u32 index);
u8 WiFiPlazaPlaces_GetValue(WiFiPlazaPlace *places, u32 index);
u8 WiFiPlazaPlaces_IsOccupied(WiFiPlazaPlace *places, u32 index);
void WiFiPlaza_DisableModelLighting(void *file);
BOOL WiFiPlaza_IsRegionValid(u32 country, u32 region);
u8 ov45_0222D7C0(u32 index);
int WiFiPlaza_GetErrorMessageClass(u32 errorCode, u32 context);
BOOL WiFiPlaza_IsErrorRecoverable(u32 errorCode, u32 context);
BOOL WiFiPlaza_HasConnectionFailed(void);
WiFiPlazaNews *WiFiPlazaNews_New(enum HeapID heapID);
void WiFiPlazaNews_Delete(WiFiPlazaNews *news);
void WiFiPlazaNews_Update(WiFiPlazaNews *news);

// wifi_plaza_news.c
void WiFiPlazaNews_SetOpenTime(WiFiPlazaNews *news, u32 *openTime);
void WiFiPlazaNews_AddPlayer(WiFiPlazaNews *news, int slot, int state, int flags);
BOOL WiFiPlazaNews_RemovePlayer(WiFiPlazaNews *news, int slot);
void WiFiPlazaNews_SetOldestPlayer(WiFiPlazaNews *news, int slot);
void WiFiPlazaNews_AddChatEvent(WiFiPlazaNews *news, WiFiPlazaPairRequest *req);
void WiFiPlazaNews_AddTapToyTradeEvent(WiFiPlazaNews *news, WiFiPlazaPairOptionRequest *req);
void WiFiPlazaNews_AddMinigameEntryEvent(WiFiPlazaNews *news, WiFiPlazaGroupFlagRequest *req);
void WiFiPlazaNews_AddFootprintStampEvent(WiFiPlazaNews *news, WiFiPlazaSingleRequest *req);
void WiFiPlazaNews_AddVisitorProfilesEvent(WiFiPlazaNews *news, WiFiPlazaGroupRequest *req);
void WiFiPlazaNews_AddPlazaNewsEvent(WiFiPlazaNews *news, WiFiPlazaGroupRequest *req);
void WiFiPlazaNews_AddLobbyEventNotice(WiFiPlazaNews *news, WiFiPlazaLobbyNotice *notice);
void WiFiPlazaNews_AddSpecialVisitorEvent(WiFiPlazaNews *news, int *slot, const u8 *flags);
void WiFiPlazaNews_AddMinigameResultEvent(WiFiPlazaNews *news, WiFiPlazaGroupOptionRequest *req);
void WiFiPlazaNews_RemoveCurrentEvent(WiFiPlazaNews *news);
u32 WiFiPlazaNews_GetOpenTime(WiFiPlazaNews *news, u32 *openTime);
BOOL WiFiPlazaNews_IsSlotOccupied(WiFiPlazaNews *news, int slot);
int WiFiPlazaNews_GetSlotGender(WiFiPlazaNews *news, int slot);
BOOL WiFiPlazaNews_IsOwnSlot(WiFiPlazaNews *news, int slot);
BOOL WiFiPlazaNews_IsOldestSlot(WiFiPlazaNews *news, int slot);
BOOL WiFiPlazaNews_IsLastJoinedSlot(WiFiPlazaNews *news, int slot);
u16 WiFiPlazaNews_GetSlotChange(WiFiPlazaNews *news);
BOOL WiFiPlazaNews_HasEvent(WiFiPlazaNews *news);
u8 WiFiPlazaNews_GetEventType(WiFiPlazaNews *news);
BOOL WiFiPlazaNews_FormatEvent(WiFiPlazaNews *news, const u8 *flags, int unused, String *out, enum HeapID heapID);
int WiFiPlazaNews_GetEventSlot(WiFiPlazaNews *news, int unused, int index);
void WiFiPlazaNewsSlots_Init(WiFiPlazaNewsSlots *slots);
void WiFiPlazaNewsSlots_Set(WiFiPlazaNewsSlots *slots, int slot, int state, int flags);
void WiFiPlazaNewsSlots_AddPlayer(WiFiPlazaNewsSlots *slots, int slot, int state, int flags);
void WiFiPlazaNewsSlots_RemovePlayer(WiFiPlazaNewsSlots *slots, int slot);
void WiFiPlazaNewsSlots_SetFlags(WiFiPlazaNewsSlots *slots, int slot, int flags);
void WiFiPlazaNewsSlots_SetChange(WiFiPlazaNewsSlots *slots, int change);
void WiFiPlazaNewsSlots_SetLastJoined(WiFiPlazaNewsSlots *slots, int slot);
BOOL WiFiPlazaNewsSlots_IsOccupied(WiFiPlazaNewsSlots *slots, int slot);
int WiFiPlazaNewsSlots_GetGender(WiFiPlazaNewsSlots *slots, int slot);
BOOL WiFiPlazaNewsSlots_IsOwn(WiFiPlazaNewsSlots *slots, int slot);
BOOL WiFiPlazaNewsSlots_IsOldest(WiFiPlazaNewsSlots *slots, int slot);
int WiFiPlazaNewsSlots_GetFlags(WiFiPlazaNewsSlots *slots, int slot);
u16 WiFiPlazaNewsSlots_GetChange(WiFiPlazaNewsSlots *slots);

// wifi_plaza_news_events.c
BOOL WiFiPlazaNewsSlots_IsLastJoined(WiFiPlazaNewsSlots *slots, u32 slot);
void WiFiPlazaNewsEventList_Init(WiFiPlazaNewsEventList *list, enum HeapID heapID);
void WiFiPlazaNewsEventList_Free(WiFiPlazaNewsEventList *list);
void WiFiPlazaNewsEventList_Update(WiFiPlazaNewsEventList *list);
void WiFiPlazaNewsEventList_RemoveFirst(WiFiPlazaNewsEventList *list);
WiFiPlazaNewsEvent *WiFiPlazaNewsEventList_GetFreeEvent(WiFiPlazaNewsEventList *list, u16 rank);
void WiFiPlazaNewsEvent_InsertAfter(WiFiPlazaNewsEvent *at, WiFiPlazaNewsEvent *event);
void WiFiPlazaNewsEventList_Insert(WiFiPlazaNewsEventList *list, WiFiPlazaNewsEvent *event);
void WiFiPlazaNewsEventList_Remove(WiFiPlazaNewsEventList *list, WiFiPlazaNewsEvent *event);
void WiFiPlazaNewsEvent_Set(WiFiPlazaNewsEvent *event, u32 a, u32 b, u32 c, const PlayerProfile *player0, const PlayerProfile *player1, const PlayerProfile *player2, const PlayerProfile *player3, u16 slot0, u16 slot1, u16 slot2, u16 slot3, u32 timer, u32 rank, u32 type);
BOOL WiFiPlazaNewsEvent_FormatChat(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out);
BOOL WiFiPlazaNewsEvent_FormatTapToyTrade(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out);
BOOL WiFiPlazaNewsEvent_FormatMinigameEntry(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out);
BOOL WiFiPlazaNewsEvent_FormatFootprintStamp(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out);
BOOL WiFiPlazaNewsEvent_FormatChecking(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out);
BOOL WiFiPlazaNewsEvent_FormatLobbyEventNotice(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out);
BOOL WiFiPlazaNewsEvent_FormatSpecialVisitor(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out);
BOOL WiFiPlazaNewsEvent_FormatMinigameResult(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out);
u16 WiFiPlazaNewsEvent_GetChatSlot(WiFiPlazaNewsEvent *event, u32 index);
u16 WiFiPlazaNewsEvent_GetTapToyTradeSlot(WiFiPlazaNewsEvent *event, u32 index);
u16 WiFiPlazaNewsEvent_GetMinigameEntrySlot(WiFiPlazaNewsEvent *event, u32 index);
u16 WiFiPlazaNewsEvent_GetFootprintStampSlot(WiFiPlazaNewsEvent *event, u32 index);
u16 WiFiPlazaNewsEvent_GetCheckingSlot(WiFiPlazaNewsEvent *event, u32 index);
u16 WiFiPlazaNewsEvent_GetLobbyEventNoticeSlot(WiFiPlazaNewsEvent *event, u32 index);
u16 WiFiPlazaNewsEvent_GetSpecialVisitorSlot(WiFiPlazaNewsEvent *event, u32 index);

// wifi_plaza_lobby_client.c
extern WiFiPlazaLobbyClient *_022577C0;
u16 WiFiPlazaNewsEvent_GetMinigameResultSlot(WiFiPlazaNewsEvent *event, u32 index);
BOOL WiFiPlaza_IsMinigamePlayerCountValid(u32 activity, u32 count);
void WiFiPlazaLobbyClient_Init(enum HeapID heapID, SaveData *save, u32 count, WiFiPlazaCallbacks *callbacks, void *callbackArg);
void WiFiPlazaLobbyClient_Free(void);
int WiFiPlazaLobbyClient_Update(void);
void ov45_0222E7CC(void);
void ov45_0222E7FC(void);
void ov45_0222E804(int value);
void ov45_0222E810(int value, int mode);
BOOL WiFiPlazaLobbyClient_IsInLobby(void);
void ov45_0222E91C(void);
BOOL WiFiPlazaLobbyClient_IsDisconnected(void);
int WiFiPlazaLobbyClient_GetState(void);
u8 ov45_0222E9BC(void);
int WiFiPlazaLobbyClient_GetOwnId(void);
void WiFiPlazaLobbyClient_SetOwnPlayer(WiFiPlazaPlayer *player);
WiFiPlazaPlayer *WiFiPlazaLobbyClient_GetPlayer(int id);
void WiFiPlazaLobbyClient_GetEntryTime(int id, s64 *time);
int WiFiPlazaLobbyClient_GetOldestPlayer(void);
int WiFiPlazaLobbyClient_GetOldestOtherPlayer(int excluded);
BOOL WiFiPlazaLobbyClient_JoinChannel(int channel);
u8 ov45_0222EB74(void);
BOOL WiFiPlazaLobbyClient_LeaveChannel(void);
BOOL WiFiPlazaLobbyClient_IsNotInChannel(void);
BOOL ov45_0222EBF0(int channel);
void WiFiPlazaLobbyClient_GetPlayerList(WiFiPlazaPlayerList *list);
void WiFiPlazaLobbyClient_GetChannelPlayerList(u32 *out);
int WiFiPlazaLobbyClient_IdToSlot(int id);
int WiFiPlazaLobbyClient_IdToChannelSlot(int id);
int WiFiPlazaLobbyClient_SlotToId(int slot);
int WiFiPlazaLobbyClient_ChannelSlotToId(int slot);
void WiFiPlazaLobbyClient_GetTime(s64 *time);
u32 ov45_0222ECDC(int field);
int ov45_0222ED7C(void);
BOOL ov45_0222EDA8(void);
u32 WiFiPlazaLobbyClient_GetLocationCountry(u8 slot);
u8 WiFiPlazaLobbyClient_GetLocationRegion(u8 slot);
void WiFiPlazaLobbyClient_SetLobbyCommands(const void *commands, u32 count, void *arg);
void WiFiPlazaLobbyClient_SetChannelCommands(const void *commands, u32 count, void *arg);
void WiFiPlazaLobbyClient_ClearLobbyCommands(void);
void WiFiPlazaLobbyClient_ClearChannelCommands(void);
void WiFiPlazaLobbyClient_SendToLobby(int type, const void *data, int size);
void WiFiPlazaLobbyClient_SendToPlayer(int type, int id, const void *data, int size);
void WiFiPlazaLobbyClient_SendToChannel(int type, const void *data, int size);
BOOL WiFiPlazaLobbyClient_HostMatch(int index, int maxPlayers);
void ov45_0222F154(void);
void WiFiPlazaLobbyClient_CloseMatch(void);
BOOL WiFiPlazaLobbyClient_IsCurrentMatchClosed(void);
u8 ov45_0222F254(void);
BOOL WiFiPlazaLobbyClient_IsMatchActive(int index);
BOOL WiFiPlazaLobbyClient_IsMatchOpen(int index);
int WiFiPlazaLobbyClient_GetMatchFreePlaces(int index);
u32 WiFiPlazaLobbyClient_GetMatchPlayerCount(int index);
BOOL WiFiPlazaLobbyClient_JoinMatch(int index);
u32 WiFiPlazaLobbyClient_GetMatchTimer(int index);
int WiFiPlazaLobbyClient_GetMatchHostId(void);
u8 WiFiPlazaLobbyClient_IsMatchHost(void);
int WiFiPlazaLobbyClient_IsSpecialVisitor(int id);
int WiFiPlazaLobbyClient_GetSpecialVisitorCode(int id);
void ov45_0222F4D4(int index);
u32 ov45_0222F524(void);
u32 ov45_0222F544(int field);
void *ov45_0222F628(int field);
u8 ov45_0222F6B0(int which, int index);
void WiFiPlazaLobbyClient_EndMatch(void);
BOOL WiFiPlazaLobbyClient_CheckResult(int result);
void WiFiPlazaLobbyClient_ClearErrors(void);
int WiFiPlazaLobbyClient_ChannelFromLib(int libChannel);
int WiFiPlazaLobbyClient_ChannelToLib(int channel);

// wifi_plaza_lobby_players.c
BOOL WiFiPlazaLobbyClient_AddOwnLocation(WiFiPlazaLobbyClient *client);
void WiFiPlazaLobbyClient_AllocPlayerData(WiFiPlazaLobbyClient *client, u32 size, enum HeapID heapID);
void WiFiPlazaLobbyClient_FreePlayerData(WiFiPlazaLobbyClient *client);
void WiFiPlazaLobbyClient_SetPlayerData(WiFiPlazaLobbyClient *client, int id, const void *data);
void WiFiPlazaLobbyClient_CopyPlayerData(WiFiPlazaLobbyClient *client, int id, const void *data, u32 size);
WiFiPlazaPlayer *WiFiPlazaLobbyClient_GetPlayerData(WiFiPlazaLobbyClient *client, int id);
void WiFiPlazaLobbyClient_ClearPlayerData(WiFiPlazaLobbyClient *client, int id);
void WiFiPlazaLobbyClient_SetPlayerRecord(WiFiPlazaLobbyClient *client, int id, const WiFiPlazaLobbyPlayerRecord *record);
WiFiPlazaLobbyPlayerRecord *WiFiPlazaLobbyClient_GetPlayerRecord(WiFiPlazaLobbyClient *client, int id);
void WiFiPlazaLobbyClient_ClearPlayerRecord(WiFiPlazaLobbyClient *client, int id);
void WiFiPlazaLobbyClient_AllocIdLists(WiFiPlazaLobbyClient *client, u32 lobbyCapacity, u32 channelCapacity, enum HeapID heapID);
void WiFiPlazaLobbyClient_FreeIdLists(WiFiPlazaLobbyClient *client);
void WiFiPlazaLobbyClient_ClearIdList(WiFiPlazaLobbyClient *client, int which);
void WiFiPlazaLobbyClient_AddToIdList(WiFiPlazaLobbyClient *client, int id, int which);
void WiFiPlazaLobbyClient_RemoveFromIdList(WiFiPlazaLobbyClient *client, int id, int which);
int WiFiPlazaLobbyClient_FindInIdList(WiFiPlazaLobbyClient *client, int id, int which);
int WiFiPlazaLobbyClient_GetIdListEntry(WiFiPlazaLobbyClient *client, int slot, int which);
void WiFiPlazaLobbyClient_RebuildChannelList(WiFiPlazaLobbyClient *client, u32 channel);

// wifi_plaza_lobby_events.c
BOOL WiFiPlazaLobbyClient_HasChannelRoom(WiFiPlazaLobbyClient *client, int channel);
void WiFiPlazaLobbyClient_OnEnterChannel(int result, u32 libChannel);
void WiFiPlazaLobbyClient_OnPlayerEnter(int id, const WiFiPlazaLobbyPlayerRecord *record, const void *data, u32 size);
void WiFiPlazaLobbyClient_OnPlayerLeave(int id);
void ov45_0222FDD4(void);
void WiFiPlazaLobbyClient_OnMessage(int id, u32 libChannel, u32 type, const void *data, u32 size);
void WiFiPlazaLobbyClient_OnLocationsChanged(int a0, int a1, int a2, int a3, const char *key, const void *data);
void WiFiPlazaLobbyClient_OnPlayerRecordChanged(int id, const WiFiPlazaLobbyPlayerRecord *record);
void WiFiPlazaLobbyClient_OnPlayerUpdate(int id, const void *data);
void WiFiPlazaLobbyClient_OnMatchInfo(int id, WiFiPlazaLobbyMatchInfo *info);
void WiFiPlazaLobbyClient_OnMatchClosed(int id, const WiFiPlazaLobbyMatchInfo *info);
void WiFiPlazaLobbyClient_OnLobbyEvent(u32 event);
void WiFiPlazaLobbyClient_OnOwnRecord(int result, const WiFiPlazaPlayer *data, u32 size);
BOOL ov45_02230090(int result);
void ov45_022300B0(int result, const void *data, u32 size);
void ov45_022300DC(int result, const void *data);
void ov45_02230108(void);
void ov45_02230130(void);
void WiFiPlazaLobbyClient_InitMatches(WiFiPlazaLobbyClient *client);

// wifi_plaza_lobby_matches.c
void WiFiPlazaLobbyMatches_Set(WiFiPlazaLobbyClient *client, int index, int hostId, const WiFiPlazaLobbyMatchInfo *info);
BOOL WiFiPlazaLobbyMatches_Remove(WiFiPlazaLobbyClient *client, int index, int hostId);
BOOL WiFiPlazaLobbyMatches_IsActive(WiFiPlazaLobbyClient *client, int index);
u16 WiFiPlazaLobbyMatches_CalcTimer(WiFiPlazaLobbyClient *client, int index);
WiFiPlazaLobbyMatch *WiFiPlazaLobbyMatches_Get(WiFiPlazaLobbyClient *client, int index);
void WiFiPlazaLobbyMatches_Update(WiFiPlazaLobbyClient *client);
void WiFiPlazaLobbyMatchInfo_SetStartTime(WiFiPlazaLobbyMatchInfo *info, const s64 *time);
void WiFiPlazaLobbyMatchInfo_GetStartTime(const WiFiPlazaLobbyMatchInfo *info, s64 *time);
void ov45_02230390(int result, void *arg);

// wifi_plaza_player_objects.c
void ov45_02230394(int result, void *arg);
u32 ov45_022303BC(WiFiPlazaLobbyClientUnk1B4 *unk, int key);
void WiFiPlazaLobbyIdValueList_Init(WiFiPlazaLobbyClientUnk6A0 *unk, int count, enum HeapID heapID);
void WiFiPlazaLobbyIdValueList_Free(WiFiPlazaLobbyClientUnk6A0 *unk);
void WiFiPlazaLobbyIdValueList_Set(WiFiPlazaLobbyClientUnk6A0 *unk, const void *data, u32 count);
BOOL WiFiPlazaLobbyIdValueList_Contains(WiFiPlazaLobbyClientUnk6A0 *unk, int id);
u32 WiFiPlazaLobbyIdValueList_GetValue(WiFiPlazaLobbyClientUnk6A0 *unk, int id);
void ov45_0223048C(u32 *dest, const void *data);
WiFiPlazaPlayerObjects *WiFiPlazaPlayerObjects_New(u32 count, BOOL mode, enum HeapID heapID, enum HeapID loadHeapID);
void WiFiPlazaPlayerObjects_Delete(WiFiPlazaPlayerObjects *objects);
void WiFiPlazaPlayerObjects_Update(WiFiPlazaPlayerObjects *objects);
void WiFiPlazaPlayerObjects_Draw(WiFiPlazaPlayerObjects *objects);
void ov45_022306F4(WiFiPlazaPlayerObjects *objects);
void WiFiPlazaPlayerObjects_SetModelAlpha(WiFiPlazaPlayerObjects *objects, int alpha);
WiFiPlazaPlayerObject *WiFiPlazaPlayerObjects_Add(WiFiPlazaPlayerObjects *objects, Field2DObject *player);
void WiFiPlazaPlayerObject_Delete(WiFiPlazaPlayerObject *object);
void WiFiPlazaPlayerObject_SetMode(WiFiPlazaPlayerObject *object, u32 mode);
void WiFiPlazaPlayerObject_SetLightFlags(WiFiPlazaPlayerObject *object, u32 lightFlags);
u32 WiFiPlazaPlayerObject_IsCulled(WiFiPlazaPlayerObject *object);
void WiFiPlazaPlayerObject_SetGridPosition(WiFiPlazaPlayerObject *object, const Field2DPoint *gridPos);
void WiFiPlazaPlayerObject_SetPosition(WiFiPlazaPlayerObject *object, const VecFx32 *position);
void WiFiPlazaPlayerObject_GetPosition(WiFiPlazaPlayerObject *object, VecFx32 *position);
void WiFiPlazaPlayerObject_SetStandingSequence(WiFiPlazaPlayerObject *object, u32 sequence);
void WiFiPlazaPlayerObject_StartEffect(WiFiPlazaPlayerObject *object, u32 effect);
void WiFiPlazaPlayerObject_StopEffect(WiFiPlazaPlayerObject *object);
void WiFiPlazaPlayerObject_SetEffectStep(WiFiPlazaPlayerObject *object, u8 step);
void WiFiPlazaPlayerObject_SetVisible(WiFiPlazaPlayerObject *object, int visible);
u32 WiFiPlazaPlayerObject_IsVisible(WiFiPlazaPlayerObject *object);
const WiFiPlazaSpriteConfig *WiFiPlaza_GetSpriteConfig(int spriteId);
u32 WiFiPlaza_GetSpriteSequence(int standing, u32 sequence);
u32 WiFiPlaza_CalcSequenceFrame(u32 divisor, u16 value, u32 factor);
void WiFiPlazaObjectModel_Load(WiFiPlazaObjectModel *model, NARC *narc, int fileId, enum HeapID heapID);
void WiFiPlazaObjectModel_Free(WiFiPlazaObjectModel *model);
void WiFiPlazaObjectModel_SetAlpha(WiFiPlazaObjectModel *model, int alpha);
int WiFiPlazaObjectModel_GetAlpha(WiFiPlazaObjectModel *model);
void WiFiPlaza_InitModelInstance(ModelResource *resource, ModelInstance *instance);
WiFiPlazaPlayerObject *WiFiPlazaPlayerObjects_GetFreeObject(WiFiPlazaPlayerObjects *objects);
BOOL WiFiPlazaPlayerObject_IsUsed(WiFiPlazaPlayerObject *object);
void WiFiPlazaPlayerObject_Clear(WiFiPlazaPlayerObject *object);
void WiFiPlazaPlayerObject_DrawModel(WiFiPlazaPlayerObject *object);
void WiFiPlazaPlayerObject_FollowPlayer(WiFiPlazaPlayerObject *object);
void WiFiPlazaPlayerObject_ShowStandingSprite(WiFiPlazaPlayerObject *object);
void WiFiPlazaPlayerObject_PlayStandingSequence(WiFiPlazaPlayerObject *object);
void ov45_02230BFC(WiFiPlazaPlayerObject *object);
void WiFiPlazaPlayerObject_PlayWalkingSequence(WiFiPlazaPlayerObject *object);
void WiFiPlazaPlayerObject_UpdateEffect(WiFiPlazaPlayerObject *object);
void WiFiPlazaPlayerObject_EffectTurnAround(WiFiPlazaPlayerObject *object);
void ov45_02230D20(WiFiPlazaPlayerObject *object);
void ov45_02230D5C(WiFiPlazaPlayerObject *object);
BOOL ov45_02230DC4(u32 action);
void WiFiPlazaPlayerObject_UpdateCulling(WiFiPlazaPlayerObject *object);
void WiFiPlazaPlayerObject_ApplyVisibility(WiFiPlazaPlayerObject *object);
void WiFiPlazaPlayerObject_ApplyLightFlags(void *sprite, WiFiPlazaPlayerObject *object);

// wifi_plaza_sprite_culling.c
BOOL WiFiPlaza_IsSpriteInView(void *sprite);

// wifi_plaza_time_zone.c
WiFiPlazaTime WiFiPlaza_GetLocalTime(int country, u16 region, u32 time, enum HeapID heapID);
void WiFiPlaza_GetLongitude(NARC *narc, enum HeapID heapID, int country, u16 region, s32 *longitude);

// data (asm)
extern const WiFiPlazaTimerPhase _02254A28[5];
extern const u8 ov45_02254BB4[];
extern const u8 ov45_02254BC8[];  // news event rank per type of WiFiPlazaNews_AddMinigameResultEvent
extern const u16 ov45_02254BCA[]; // message per player count (WiFiPlazaNewsEvent_FormatMinigameResult)
extern const u16 ov45_02254BD2[];
extern const u32 ov45_02254BDC[]; // news event rank per type of WiFiPlazaNews_AddMinigameEntryEvent
// slot getter / message of a news event per type
extern u16 (*const ov45_02254BE8[9])(WiFiPlazaNewsEvent *event, u32 index);
extern BOOL (*const ov45_02254C0C[9])(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out, enum HeapID heapID);
// start time of each group
extern const s32 ov45_02254A3C[9];
// handlers per lobby message type
extern void (*const ov45_02254A60[9])(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg);

// defined in the other overlay 45 files
// lobby library
int ov45_02232170(WiFiPlazaLobbyClientUnk1B4 *data, u32 *size);
int ov45_022322A4(int a0, int a1, const char *key, const void *data, u32 size);
void ov45_02232304(int a0, int a1, const char *key, int a3);
int ov45_022310C0(const void *a0, const void *a1, int mode, void (**callbacks)(void), void *userData, int value, u32 playerDataSize);
int ov45_02231490(void);
int ov45_02231514(void);
int ov45_02231C8C(int channel);
int ov45_02231EC0(void);
int ov45_022321E0(void);
int ov45_0223247C(int channel, int type, const void *data, int size);
int ov45_02232500(int id, int type, const void *data, int size);
int ov45_02232580(void);
void ov45_022325B0(WiFiPlazaPlayer *player, u32 size, int a2);
int ov45_0223285C(WiFiPlazaLobbyMatch *match);
int ov45_02232BB0(WiFiPlazaLobbyMatch *match);
int ov45_02232CA4(void);
int ov45_02232E60(int index);
void ov45_022331E8(void);
void ov45_02233204(void);
void ov45_02233220(void);
extern const u8 ov45_02254C30[4]; // room per channel
extern const char *ov45_02254EF0; // lobby data key of the location list
extern const u8 ov45_02254F04[];
extern const u8 ov45_02254F14[];
void ov45_022320C4(void *a0);

extern const WiFiPlazaAvatarEntry ov45_02254A84[16];
extern u8 ov45_02254AC4[];

extern const u8 ov45_02254C34[4];   // sprite directions of effect WiFiPlazaPlayerObject_EffectTurnAround
extern const u16 ov45_02254C38[2];  // sprite sequence files
extern const VecFx32 ov45_02254C3C; // sprite scale
extern WiFiPlazaSpriteConfig ov45_02254C48[20];
extern const u8 ov45_02254C98[];
extern void (*ov45_02254F1C[3])(WiFiPlazaPlayerObject *object);      // effects
extern void (*const ov45_02254F28[])(WiFiPlazaPlayerObject *object); // per player action

// defined in other overlays / the ARM9 static part
void WiFiPlazaMap_PosToVecFx32(const s16 *gridPos, s32 *position);
void WiFiPlazaMap_LoadModelFile(NNSG3dResFileHeader **file, NARC *narc, int fileId, enum HeapID heapID);
// billboards
void *Billboard_Create(WiFiPlazaBillboardParams *params);
void Billboard_Delete(void *billboards);
void *BillboardObject_Create(WiFiPlazaBillboardCreateParams *params);
void BillboardObject_Delete(void *sprite);
void BillboardObject_SetCallback(void *sprite, void (*callback)(void *sprite, WiFiPlazaPlayerObject *object), WiFiPlazaPlayerObject *object);
void BillboardObject_SetPosition(void *sprite, const VecFx32 *position);
const VecFx32 *BillboardObject_GetPosition(void *sprite);
void BillboardObject_SetSequence(void *sprite, u32 sequence);
void BillboardObject_SetSequenceFrame(void *sprite, fx32 frame);
void BillboardObject_AdvanceFrame(void *sprite, fx32 frames);
u8 BillboardObject_GetSequence(void *sprite);
u32 BillboardObject_GetSequenceFrame(void *sprite);
void *BillboardObject_GetModelNoCast(void *sprite);
const VecFx32 *BillboardObject_GetScale(void *sprite);
BOOL GF3d_BoundsBoxTest(const VecFx32 *translation, const WiFiPlazaBoxTestBounds *bounds);
void BillboardGfxSequence_Init(const void *data, WiFiPlazaBillboardSequence *sequence);
void BillboardResource_Init(WiFiPlazaBillboardResource *resource, void *modelFile, NNSG3dResTex *texture, const void *ranges, WiFiPlazaBillboardSequence *sequence);
void BillboardResource_InitWithKeys(WiFiPlazaBillboardResource *resource, void *modelFile, NNSG3dResTex *texture, const void *ranges, WiFiPlazaBillboardSequence *sequence, NNSG3dTexKey texKey, NNSG3dTexKey tex4x4Key, NNSG3dPlttKey plttKey);
u32 NNS_G3dMdlGetMdlLightEnableFlag(const NNSG3dResMdl *pResMdl, u32 matID);
void NNS_G3dMdlSetMdlLightEnableFlagAll(NNSG3dResMdl *pResMdl, u32 light);
void NNS_G3dMdlSetMdlPolygonIDAll(NNSG3dResMdl *pResMdl, u32 polygonID);
void NNS_G3dMdlSetMdlAlphaAll(NNSG3dResMdl *pResMdl, u32 alpha);
void *Save_PlazaSurvey_Get(SaveData *save);
void PlazaSurvey_SetSurveyId(void *a0, u32 a1);
void PlazaSurvey_SetAnswer(void *a0, u32 a1);
void *WiFiPlazaPlayerInfo_New(PlayerProfile *profile, enum HeapID heapID);
void WiFiPlazaPlayerInfo_Free(void *a0);

#endif // POKEHEARTGOLD_WIFI_PLAZA_INTERNAL_H
