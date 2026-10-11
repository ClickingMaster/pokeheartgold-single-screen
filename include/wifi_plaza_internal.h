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
    void *unk_00;
    SaveData *save;
    u32 unk_08;
} WiFiPlazaArgs;

// Second part of a plaza player record (copied to the save data when the plaza is left, WiFiPlazaWork_Delete).
typedef struct WiFiPlazaPlayerUnk8C {
    u32 unk_00;
    u32 unk_04; // < 3
} WiFiPlazaPlayerUnk8C;

// A player in the plaza lobby: the own one is WiFiPlazaWork.profile.player, the others come from the lobby
// library (ov45_0222EA2C).
typedef struct WiFiPlazaPlayer {
    u32 unk_00;
    u32 trainerId;
    u16 name[8];
    u32 unk_18;   // ov45_0222D500 of the entry time (WiFiPlaza_OnPlayerEnter)
    void *unk_1c; // WiFiPlazaTime: local time of the location (ov45_02230F94)
    u16 species[6];
    u8 forms[6];
    u8 isEgg[6];
    u8 gender; // >= 2: taken from the avatar table
    u8 language;
    u16 unk_3a; // mapped to the avatar by ov45_0222CD04
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
    WiFiPlazaPlayerUnk8C unk_8c;
} WiFiPlazaPlayer;

// The own plaza profile (WiFiPlazaProfile_InitFromSave); WiFiPlazaProfile_UpdateChecksum and WiFiPlazaProfile_IsChecksumValid keep its CRC.
typedef struct WiFiPlazaProfile {
    u16 name[8];
    u16 unk_10[8]; // the name received from the lobby (ov45_0222BB00)
    WiFiPlazaPlayer player;
    u32 crc; // of player
} WiFiPlazaProfile;

// The ids of the players in the lobby (ov45_0222EC10): -1 = free.
typedef struct WiFiPlazaPlayerList {
    s32 unk_00;
    s32 *ids;
} WiFiPlazaPlayerList;

// Entry of the avatar table ov45_02254A84.
typedef struct WiFiPlazaAvatarEntry {
    u16 unk_00;
    u16 unk_02;
} WiFiPlazaAvatarEntry;

// Part of WiFiPlazaWork at 0x1C0: an exchange with one other player of the lobby (ov45_0222AE74 starts it
// for a lobby slot, ov45_0222AED8 / ov45_0222AF80 / ov45_0222AFC4 answer or end it).
typedef struct WiFiPlazaUnk1C0 {
    u32 unk_00; // bit per lobby slot (ov45_0222BD74 / ov45_0222BD94 / ov45_0222BDB0)
    u16 unk_04; // lobby slot of the other player
    u8 unk_06;  // state
    u8 unk_07;
    u16 unk_08;
    u8 unk_0a; // kind: 0 = none, 1 / 2 (ov45_0222AFF8 / ov45_0222B00C)
    u8 unk_0b;
    u8 unk_0c[20]; // per lobby slot, counts up to 6 (ov45_0222BDCC; ov45_0222B048)
    u16 unk_20;    // sent with WiFiPlazaLobbyClient_SendToPlayer (4 bytes from here)
    u16 unk_22;
    u16 unk_24; // ov45_0222BE48 sets it, ov45_0222BE54 clears it
    s16 unk_26; // counts up to 900 while unk_24 is set (ov45_0222BE5C)
} WiFiPlazaUnk1C0;

// A message to the lobby (WiFiPlazaLobbyClient_SendToLobby type 4).
typedef struct WiFiPlazaLobbyMessage {
    u32 ids[4]; // ov45_0222EC90 of lobby slots
    u8 count;   // of ids
    u8 type;
    u8 unk_12;
    u8 unk_13_0 : 7;
    u8 unk_13_7 : 1;
} WiFiPlazaLobbyMessage;

// Part of WiFiPlazaWork at 0x1FC (ov45_0222BB60 / ov45_0222BC3C / ov45_0222BC84 work on it).
typedef struct WiFiPlazaUnk1FC {
    u8 flag0 : 1;
    u8 flag1 : 1;
    u8 mode2 : 2;
    u8 flag4 : 1;
    u8 mode5 : 2;
    u8 flag7 : 1;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0a;
    u8 unk_0c;
    u8 unk_0d;
    u8 unk_0e;
    u8 low : 4;
    u8 high : 4;
} WiFiPlazaUnk1FC;

// Part of WiFiPlazaWork at 0x3A0.
typedef struct WiFiPlazaUnk3A0 {
    s16 timer; // ov45_0222BCA0 starts it, ov45_0222BCB8 counts it down
    u8 unk_02; // ov45_0222BD24 / ov45_0222BD2C
    u8 unk_03;
    u8 flags[8]; // ov45_0222B118 sets one, ov45_0222B134 finds a clear one
} WiFiPlazaUnk3A0;

// Part of WiFiPlazaWork at 0x49C: a timer in 1/30 s of the lobby time (ov45_0222CA7C sets it up, ov45_0222CA8C
// starts it, ov45_0222CAA0 runs it).
typedef struct WiFiPlazaTimer {
    u32 active;
    s64 start; // ov45_0222ECB8
    s32 elapsed;
    s32 duration;
    u32 phase;    // _02254A28[].phase, then 5 at the end
    u32 counter;  // updates in the current phase
    void *unk_1c; // WiFiPlazaPlayerInfo_New (ov45_0222B244 / ov45_0222B270)
} WiFiPlazaTimer;

// A phase of WiFiPlazaTimer and its share of the duration (in 1/256).
typedef struct WiFiPlazaTimerPhase {
    u16 phase;
    u16 length;
} WiFiPlazaTimerPhase;

// Part of WiFiPlazaWork at 0x3AC: the chance of each group of WiFiPlazaWork.unk_20c (ov45_0222C900).
typedef struct WiFiPlazaGroupWeights {
    u8 weights[9];
    u8 unk_09[3];
    u8 seen[20]; // per lobby slot (ov45_0222C8C8)
} WiFiPlazaGroupWeights;

// Four words per lobby slot, made from a code (ov45_0222C9A0).
typedef struct WiFiPlazaWordSet {
    u8 flags[20];
    u16 words[20][4];
    WallpaperPasswordBank *bank;
} WiFiPlazaWordSet;

// The survey tally of this plaza, cached by the survey results screen (WiFiPlazaSurveyTally, 0x14 bytes).
typedef struct WiFiPlazaTallyCache {
    u32 words[5];
} WiFiPlazaTallyCache;

// Part of WiFiPlazaWork at 0x508 (ov45_0222BCC8 / ov45_0222BCD8 set it up and free it).
typedef struct WiFiPlazaUnk508 {
    PlayerProfile *profile; // the own profile as the lobby sees it (ov45_0222BCE4)
    void *unk_04;
    WiFiPlazaTallyCache tally; // ov45_0222B1FC / ov45_0222B21C
    BOOL hasTally;
} WiFiPlazaUnk508;

// Plaza work data shared by the sub-applications (WiFiPlazaWork_New).
// Part of WiFiPlazaWork at 0x20C: nine groups of three lobby slots (ov45_0222C388 clears it).
typedef struct WiFiPlazaUnk20C {
    u32 active; // ov45_0222C3A8 sets it
    s32 time;
    s32 counters[9];
    u32 states[9];
    u32 ids[9][3]; // -1 = free (ov45_0222C388)
    u32 unk_bc[9][3];
    u32 unk_128[9][3];
} WiFiPlazaUnk20C;

// A time of day (ov45_0222D500 / ov45_0222D524 / ov45_0222D594).
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

// A country and region (ov45_0222D638 adds them to a list of 50).
typedef struct WiFiPlazaPlace {
    u16 country;
    u8 region;
    u8 value : 4;
    u8 occupied : 4;
} WiFiPlazaPlace;

// Part of WiFiPlazaWork at 0x4BC: the last activities of the own player (id < 24, 24 = none; value -1 = none), a ring
// of 13 entries; crc covers everything before save (ov45_0222CCA4 / ov45_0222CCB8).
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
    u8 unk_39;
    u16 heapID;
    struct WiFiPlazaWork *work;
    SaveData *save;
    void *unk_44;
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

// What ov45_0222C370 passes to ov45_0222DB98.
typedef struct WiFiPlazaLobbyNotice {
    u32 *time;
    u32 which;
} WiFiPlazaLobbyNotice;

// A lobby slot in the plaza news.
typedef struct WiFiPlazaNewsSlot {
    u16 state; // 2: empty
    u16 flags; // bit 1: the slot of ov45_0222D8F0
} WiFiPlazaNewsSlot;

typedef struct WiFiPlazaNewsSlots {
    WiFiPlazaNewsSlot slots[20];
    u16 change;   // 1: a player came, 2: a player left (ov45_0222DEA4)
    u16 lastSlot; // ov45_0222DEB8
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

// The plaza news (WiFiPlazaWork.news, ov45_0222D860), handed to the plaza news app.
typedef struct WiFiPlazaNews {
    u32 openTime; // WiFiPlazaTime of the lobby opening (ov45_0222D8BC)
    u32 isOpen;
    WiFiPlazaNewsSlots slots;
    WiFiPlazaNewsEventList events;
} WiFiPlazaNews;

typedef struct WiFiPlazaWork {
    SaveData *save;
    WiFiPlazaNews *news;
    u32 unk_08;
    WiFiPlazaPlace places[50]; // filled by ov45_0222B79C from the profile
    u32 unk_d4;
    s64 unk_d8; // ov45_0222ECB8, refreshed every frame
    s64 unk_e0; // ov45_022320C4 when the lobby opened (WiFiPlaza_OnLobbyEvent 0)
    PlayerProfile *profiles[4];
    u32 unk_f8[4]; // per lobby slot: entered, left, events, updated (ov45_0222A230 / A25C / A288 / A2A0)
    WiFiPlazaProfile profile;
    WiFiPlazaUnk1C0 unk_1c0;
    u8 unk_1e8[0x14];
    WiFiPlazaUnk1FC unk_1fc;
    WiFiPlazaUnk20C unk_20c;
    WiFiPlazaUnk3A0 unk_3a0;
    WiFiPlazaGroupWeights groupWeights;
    u8 unk_3cc[0x14]; // per lobby slot (ov45_0222C944); copied out by ov45_0222AB38
    u8 unk_3e0[4];
    WiFiPlazaWordSet wordSet;
    WiFiPlazaTimer timer;
    WiFiPlazaHistory history;
    WiFiPlazaUnk508 unk_508;
    enum HeapID heapID;
    u32 unk_52c; // error flag
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

// The time a player entered the lobby (an s64 kept as two words, ov45_0222F954).
typedef struct WiFiPlazaLobbyEntryTime {
    u32 lo;
    u32 hi;
} WiFiPlazaLobbyEntryTime;

// What the lobby keeps per player (ov45_0222F928 / ov45_0222F954).
typedef struct WiFiPlazaLobbyPlayerRecord {
    WiFiPlazaLobbyEntryTime entryTime;
    u32 channel; // lobby library channel (ov45_0222F7B0), 5 = none
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

// Country and region of a lobby slot (ov45_0222EDC4 / ov45_0222EDF0).
typedef struct WiFiPlazaLobbyLocation {
    u16 country;
    u8 region;
    u8 active;
} WiFiPlazaLobbyLocation;

// What the host of a matchmaking session sends about it (ov45_0222FF7C).
typedef struct WiFiPlazaLobbyMatchInfo {
    int index;
    int maxPlayers;
    int playerCount; // ov00_021E7144 for the own session
    int state;       // 1 once the host closed it (ov45_0222F1BC)
    WiFiPlazaLobbyEntryTime startTime;
    u8 unk_18[0x18];
    char name[0x14];
} WiFiPlazaLobbyMatchInfo;

// A matchmaking session of the lobby client: ov45_0222F028 hosts it, ov45_0222F350 joins it (ov00_021E5CEC with its
// name, its player count and timer / 30 seconds).
typedef struct WiFiPlazaLobbyMatch {
    WiFiPlazaLobbyMatchInfo info;
    u16 active;
    u16 timer;  // frames left, 1800 at the start
    int hostId; // own id when hosting (ov45_0222F028)
} WiFiPlazaLobbyMatch;

typedef struct WiFiPlazaLobbyKeyValue {
    u32 value;
    u32 key;
} WiFiPlazaLobbyKeyValue;

typedef struct WiFiPlazaLobbyIdValue {
    u32 id;
    u32 value;
} WiFiPlazaLobbyIdValue;

// Overlays the first 8 bytes of WiFiPlazaLobbyClient.unk_6a0 (ov45_022303E4).
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
    u8 mode : 4;         // 0: plays `effect` (ov45_02230CB0), else follows the player's action (ov45_02230ACC)
    u8 culled : 2;       // ov45_02230DF4
    u8 visible : 2;      // ov45_02230978
    u8 effectActive : 1; // ov45_0223093C
    u8 effect : 7;       // index into ov45_02254F1C
    u8 timer;
    u8 step;
    UnkStruct_ov42_02228110 *player; // NULL = free slot
    void *sprite;                    // billboard object
    ModelInstance model;
    u8 action;        // ov42_02228188 field 5
    u8 savedSequence; // sequence and frame when an action of ov45_02230DC4 ended
    u16 actionFrame;  // ov42_02228188 field 8
    u32 savedFrame;
    u32 lightFlags;
} WiFiPlazaPlayerObject;

typedef struct WiFiPlazaObjectModel {
    ModelResource resource;
    int alpha; // 0 = the models are not drawn (ov45_022306B4)
} WiFiPlazaObjectModel;

// The player objects (ov45_02230498).
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
    u16 spriteId; // ov42_02228188 field 7
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
    u32 playerDataSize; // ov45_0222F848
    WiFiPlazaLobbyPlayerRecord records[20];
    WiFiPlazaLobbyIdList idLists[2];
    u8 state;        // 0 = none, 1 = connecting, 2 = in the lobby, 3, 4 = error (WiFiPlazaLobbyClient_Update)
    u8 channel;      // 4 = none (ov45_0222EB38)
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
    WiFiPlazaLobbyClientUnk1B4 unk_1b4; // received with ov45_02232170 (ov45_0222FC44)
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
u32 ov45_0222A1FC(WiFiPlazaWork *work);
void ov45_0222A208(void *a0, void *a1, void *a2);
void *ov45_0222A210(WiFiPlazaWork *work);
void *ov45_0222A214(WiFiPlazaWork *work);
u32 *ov45_0222A22C(WiFiPlazaWork *work);
BOOL ov45_0222A230(WiFiPlazaWork *work, int bit);
BOOL ov45_0222A25C(WiFiPlazaWork *work, int bit);
BOOL ov45_0222A288(WiFiPlazaWork *work, int bit);
BOOL ov45_0222A2A0(WiFiPlazaWork *work, int bit);
void *ov45_0222A2C8(void **ptr);
int ov45_0222A2CC(WiFiPlazaWork *work);
u32 ov45_0222A2E0(WiFiPlazaWork *work);
u32 ov45_0222A2F8(WiFiPlazaWork *work);
void ov45_0222A310(WiFiPlazaWork *work);
u32 ov45_0222A324(WiFiPlazaWork *work);
u32 ov45_0222A330(WiFiPlazaWork *work);
BOOL ov45_0222A33C(WiFiPlazaWork *work);
u32 ov45_0222A35C(WiFiPlazaWork *work);
BOOL ov45_0222A374(WiFiPlazaWork *work);
u32 ov45_0222A394(WiFiPlazaWork *work);
BOOL ov45_0222A3A0(WiFiPlazaWork *work);
u32 ov45_0222A3BC(WiFiPlazaWork *work);
u32 ov45_0222A3D4(WiFiPlazaWork *work);
u32 ov45_0222A3EC(WiFiPlazaWork *work);
void ov45_0222A404(WiFiPlazaWork *work);
int ov45_0222A414(WiFiPlazaWork *work);
void *ov45_0222A424(WiFiPlazaWork *work);
void ov45_0222A430(WiFiPlazaWork *work, void *value);
void ov45_0222A43C(WiFiPlazaWork *work);
void ov45_0222A450(WiFiPlazaWork *work, int id, u32 index);
void ov45_0222A480(WiFiPlazaWork *work, u32 index);
void ov45_0222A498(WiFiPlazaWork *work, void *dest);
void ov45_0222A4A8(WiFiPlazaWork *work);
int ov45_0222A4B8(WiFiPlazaWork *work);
void ov45_0222A4C8(WiFiPlazaWork *work, u8 value);

// wifi_plaza_player.c
void ov45_0222A4D0(WiFiPlazaWork *work);
void ov45_0222A520(WiFiPlazaWork *work, int value);
int ov45_0222A53C(void *unused);
void ov45_0222A548(void);
BOOL ov45_0222A550(void *unused, int index);
WiFiPlazaPlayer *WiFiPlazaWork_GetPlayer(WiFiPlazaWork *work, u32 index);
WiFiPlazaPlayer *WiFiPlazaWork_GetMyPlayer(WiFiPlazaWork *work);
void ov45_0222A5E8(WiFiPlazaWork *work, int value);
void ov45_0222A704(WiFiPlazaWork *work, int index, int value);
void ov45_0222A72C(WiFiPlazaWork *work, u32 value);
void ov45_0222A770(WiFiPlazaWork *work, int a, int b);
void ov45_0222A7DC(WiFiPlazaWork *work, u32 a, u32 b);
void WiFiPlazaPlayer_CopyToProfile(WiFiPlazaPlayer *player, PlayerProfile *profile, enum HeapID heapID);
int ov45_0222A920(WiFiPlazaPlayer *player);
u8 ov45_0222A92C(WiFiPlazaPlayer *player, u32 index);
void *ov45_0222A964(WiFiPlazaPlayer *player, u32 index);
u32 ov45_0222A99C(WiFiPlazaPlayer *player);
u32 WiFiPlazaPlayer_GetTrainerID(WiFiPlazaPlayer *player);
int ov45_0222A9A4(WiFiPlazaPlayer *player);
void *ov45_0222A9C8(WiFiPlazaPlayer *player);
u32 WiFiPlazaPlayer_GetGender(WiFiPlazaPlayer *player);
u32 WiFiPlazaPlayer_GetLanguage(WiFiPlazaPlayer *player);
BOOL WiFiPlazaPlayer_IsLanguageValid(WiFiPlazaPlayer *player);
u8 ov45_0222AA54(WiFiPlazaPlayer *player);
u16 WiFiPlazaPlayer_GetAvatar(WiFiPlazaPlayer *player);
u16 WiFiPlazaPlayer_GetCountry(WiFiPlazaPlayer *player);
u8 WiFiPlazaPlayer_GetRegion(WiFiPlazaPlayer *player);
u8 ov45_0222AAC8(WiFiPlazaPlayer *player);
BOOL ov45_0222AADC(WiFiPlazaPlayer *player);
u16 ov45_0222AAEC(WiFiPlazaPlayer *player, u32 index);
void ov45_0222AB0C(WiFiPlazaPlayer *player, WiFiPlazaPlayerUnk8C *out);

// wifi_plaza_lobby.c
void *ov45_0222AB1C(WiFiPlazaWork *work);
int ov45_0222AB28(WiFiPlazaWork *work, int a1);
void ov45_0222AB38(WiFiPlazaWork *work, void *dest);
int ov45_0222AB48(WiFiPlazaWork *work, int a1);
u16 *ov45_0222AB58(WiFiPlazaWork *work, int a1);
u32 ov45_0222AB68(void);
int ov45_0222AB78(void *unused, int a1);
void ov45_0222AB94(void *unused, u32 slot0, u32 slot1);
void ov45_0222ABD0(void *unused, u32 slot0, u32 slot1, u8 value);
void ov45_0222AC14(void *unused, int selector, int count, int slot0, int slot1, int slot2, int slot3, int flag);
void ov45_0222ACB8(void *unused, int mode, u8 kind, u32 slot0, u32 slot1, u32 slot2, u32 slot3);
int ov45_0222AD2C(WiFiPlazaWork *work);
int ov45_0222AD3C(WiFiPlazaWork *work);
int ov45_0222AD4C(WiFiPlazaWork *work);
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
void ov45_0222AE64(WiFiPlazaWork *work);
int ov45_0222AE74(WiFiPlazaWork *work, int slot);
void ov45_0222AED8(WiFiPlazaWork *work, int a1);
void ov45_0222AF80(WiFiPlazaWork *work);
void ov45_0222AFC4(WiFiPlazaWork *work);
BOOL ov45_0222AFF8(WiFiPlazaWork *work);
BOOL ov45_0222B00C(WiFiPlazaWork *work);
u16 ov45_0222B020(WiFiPlazaWork *work);
u8 ov45_0222B028(WiFiPlazaWork *work);
u8 ov45_0222B034(WiFiPlazaWork *work);
u16 ov45_0222B040(WiFiPlazaWork *work);
BOOL ov45_0222B048(WiFiPlazaWork *work, u32 index);
BOOL ov45_0222B06C(WiFiPlazaWork *work);
void ov45_0222B094(WiFiPlazaWork *work);
void ov45_0222B0A4(WiFiPlazaWork *work);
u8 ov45_0222B0B0(WiFiPlazaWork *work);

// wifi_plaza_state.c
void ov45_0222B0BC(WiFiPlazaWork *work);
void ov45_0222B0D8(WiFiPlazaWork *work, int slot);
void ov45_0222B0E8(WiFiPlazaWork *work, int slot);
void IncrementGameStat47(WiFiPlazaWork *work);
void IncrementGameStat119(WiFiPlazaWork *work);
void ov45_0222B118(WiFiPlazaWork *work, int slot);
int ov45_0222B134(WiFiPlazaWork *work);
int ov45_0222B1B4(WiFiPlazaWork *work);
u32 ov45_0222B1DC(WiFiPlazaWork *work);
u32 ov45_0222B1EC(WiFiPlazaWork *work);
void ov45_0222B1FC(WiFiPlazaWork *work, const WiFiPlazaTallyCache *tally);
BOOL ov45_0222B21C(WiFiPlazaWork *work, WiFiPlazaTallyCache *tally);
void ov45_0222B244(WiFiPlazaWork *work);
void ov45_0222B270(WiFiPlazaWork *work);
BOOL ov45_0222B28C(const u16 *name, u32 count, enum HeapID heapID);
void WiFiPlaza_OnPlayerEnter(int id, WiFiPlazaPlayer *player, WiFiPlazaWork *work, BOOL isOwn);
void WiFiPlaza_OnPlayerLeave(int id, WiFiPlazaWork *work);
void WiFiPlaza_OnPlayerUpdate(int id, WiFiPlazaPlayer *player, WiFiPlazaWork *work);
void WiFiPlaza_OnLobbyEvent(u32 event, WiFiPlazaWork *work);
void ov45_0222B75C(const WiFiPlazaPlayer *data, u32 size, WiFiPlazaWork *work);
void ov45_0222B79C(WiFiPlazaProfile *profile, WiFiPlazaPlace *out);
void ov45_0222B840(WiFiPlazaWork *work);
void WiFiPlazaProfile_InitFromSave(WiFiPlazaProfile *profile, SaveData *save, enum HeapID heapID);
void ov45_0222BA3C(WiFiPlazaWork *work);
void WiFiPlazaProfile_UpdateChecksum(WiFiPlazaProfile *profile, SaveData *save);
BOOL WiFiPlazaProfile_IsChecksumValid(WiFiPlazaProfile *profile, SaveData *save);
void ov45_0222BB00(WiFiPlazaProfile *profile, const WiFiPlazaPlayer *src, u32 size);
void ov45_0222BB58(u32 *flags);
void ov45_0222BB60(WiFiPlazaUnk1FC *state, u32 *flags, WiFiPlazaUnk20C *a2, WiFiPlazaTimer *a3);
void ov45_0222BC3C(WiFiPlazaUnk1FC *state);
void ov45_0222BC84(WiFiPlazaUnk1FC *state);
void ov45_0222BCA0(WiFiPlazaUnk3A0 *a0);
int ov45_0222BCA8(WiFiPlazaUnk3A0 *a0);
void ov45_0222BCB8(WiFiPlazaUnk3A0 *a0);
void ov45_0222BCC8(PlayerProfile **profile, enum HeapID heapID);
void ov45_0222BCD8(PlayerProfile **profile);
void ov45_0222BCE4(PlayerProfile **profile, WiFiPlazaProfile *src, enum HeapID heapID);
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
void ov45_0222BD40(WiFiPlazaUnk1C0 *a0);
void ov45_0222BD4C(WiFiPlazaUnk1C0 *a0);
void ov45_0222BD5C(WiFiPlazaUnk1C0 *a0);
BOOL ov45_0222BD74(WiFiPlazaUnk1C0 *a0, u32 slot);
void ov45_0222BD94(WiFiPlazaUnk1C0 *a0, u32 slot);
void ov45_0222BDB0(WiFiPlazaUnk1C0 *a0, u32 slot);
void ov45_0222BDCC(WiFiPlazaUnk1C0 *a0, u32 slot);
void ov45_0222BDE8(WiFiPlazaUnk1C0 *a0, u32 slot);
void ov45_0222BE00(WiFiPlazaUnk1C0 *a0, int value);
void ov45_0222BE08(WiFiPlazaWork *work, int id, u16 value);
void ov45_0222BE28(WiFiPlazaWork *work, int id);
void ov45_0222BE48(WiFiPlazaUnk1C0 *a0);
void ov45_0222BE54(WiFiPlazaUnk1C0 *a0);
void ov45_0222BE5C(WiFiPlazaUnk1C0 *a0);
BOOL ov45_0222BE74(WiFiPlazaUnk1C0 *a0);
s16 ov45_0222BE94(WiFiPlazaUnk1C0 *a0);
void ov45_0222BE9C(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg);
void ov45_0222BF18(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg);
void ov45_0222BF98(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg);
void ov45_0222C0CC(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg);
void ov45_0222C130(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg);
void ov45_0222C1EC(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg);
void ov45_0222C2A8(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg);
void ov45_0222C36C(void);
void ov45_0222C370(WiFiPlazaWork *work, int which);
void ov45_0222C388(WiFiPlazaUnk20C *a0);
void ov45_0222C3A8(WiFiPlazaUnk20C *a0);

// wifi_plaza_groups.c
void ov45_0222C3B0(WiFiPlazaUnk20C *groups);
int ov45_0222C408(WiFiPlazaUnk20C *groups, u32 id, u32 value);
void ov45_0222C480(WiFiPlazaUnk20C *groups, u32 id);
int ov45_0222C4B4(WiFiPlazaUnk20C *groups, u32 place);
int ov45_0222C4E4(WiFiPlazaUnk20C *groups, u32 group);
int ov45_0222C4FC(WiFiPlazaUnk20C *groups, u32 group);
void ov45_0222C514(WiFiPlazaUnk20C *groups, u32 id);
int ov45_0222C54C(WiFiPlazaUnk20C *groups, u32 place);
void ov45_0222C580(WiFiPlazaUnk20C *groups, u32 place);
int ov45_0222C5B4(WiFiPlazaUnk20C *groups, int id);
void ov45_0222C5E8(WiFiPlazaUnk20C *groups, u32 place, u32 value);
BOOL ov45_0222C61C(WiFiPlazaUnk20C *groups, u32 place, u32 value);
u32 ov45_0222C658(WiFiPlazaUnk20C *groups, u32 place);

// wifi_plaza_receive.c
void ov45_0222C688(int id, u16 *data, u32 size, WiFiPlazaWork *work);
void ov45_0222C73C(int id, u16 *data, u32 size, WiFiPlazaWork *work);
void ov45_0222C79C(int id, u16 *data, u32 size, WiFiPlazaWork *work);
void ov45_0222C800(int id, u16 *data, u32 size, WiFiPlazaWork *work);
void ov45_0222C828(int id, WiFiPlazaLobbyMessage *msg, u32 size, WiFiPlazaWork *work);

// wifi_plaza_word_set.c
void ov45_0222C83C(int id, void *data, u32 size, WiFiPlazaWork *work);
void ov45_0222C858(int id, void *data, u32 size, WiFiPlazaWork *work);
void ov45_0222C88C(int id, void *data, u32 size, WiFiPlazaWork *work);
void ov45_0222C8AC(WiFiPlazaGroupWeights *weights);
void ov45_0222C8C8(WiFiPlazaGroupWeights *weights, u32 place, u32 slot);
u32 ov45_0222C900(WiFiPlazaGroupWeights *weights);
void ov45_0222C944(u8 *flags, u32 slot, u8 value);
u32 ov45_0222C95C(u8 *flags, u32 slot);
void WiFiPlazaWordSet_Init(WiFiPlazaWordSet *wordSet, enum HeapID heapID);
void WiFiPlazaWordSet_Free(WiFiPlazaWordSet *wordSet);
void ov45_0222C9A0(WiFiPlazaWordSet *wordSet, u32 slot, u8 flag, u32 code);
u32 ov45_0222C9D0(WiFiPlazaWordSet *wordSet, u32 slot);
u16 *ov45_0222C9EC(WiFiPlazaWordSet *wordSet, u32 slot);
void ov45_0222CA10(WallpaperPasswordBank *bank, u32 code, u16 *words);
void ov45_0222CA7C(WiFiPlazaTimer *timer, int start, int end);
void ov45_0222CA8C(WiFiPlazaTimer *timer);
void ov45_0222CAA0(WiFiPlazaTimer *timer);
u32 ov45_0222CB3C(WiFiPlazaTimer *timer);
u32 ov45_0222CB40(WiFiPlazaTimer *timer);

// wifi_plaza_history.c
void ov45_0222CB44(WiFiPlazaHistory *history, SaveData *save);
void ov45_0222CB74(WiFiPlazaHistory *history, int id, s32 value);
BOOL ov45_0222CBD0(WiFiPlazaHistory *history, s32 oldValue, s32 newValue);
u8 ov45_0222CC00(WiFiPlazaHistory *history);
u8 ov45_0222CC50(WiFiPlazaHistory *history, u32 index);
s32 ov45_0222CC7C(WiFiPlazaHistory *history, u32 index);
void ov45_0222CCA4(WiFiPlazaHistory *history);
void ov45_0222CCB8(WiFiPlazaHistory *history);
u16 ov45_0222CCDC(WiFiPlazaHistory *history);

// wifi_plaza_apps.c
int ov45_0222CCE4(u16 avatar);
u16 ov45_0222CD04(u32 index);
WiFiPlazaApps *WiFiPlazaApps_New(u32 unk_39, SaveData *save, void *unk_44, WiFiPlazaWork *work, enum HeapID heapID);
void WiFiPlazaApps_Delete(WiFiPlazaApps *apps);
void WiFiPlazaApps_StartFirst(WiFiPlazaApps *apps);
BOOL WiFiPlazaApps_Run(WiFiPlazaApps *apps);
void WiFiPlazaApps_VBlank(WiFiPlazaApps *apps);
void WiFiPlazaApps_StartOverlay(WiFiPlazaApps *apps, void **args, const OverlayManagerTemplate *template);
void WiFiPlazaApps_InitArgs(WiFiPlazaApps *apps, u32 app, enum HeapID heapID);
void WiFiPlazaApps_FreeArgs(WiFiPlazaApps *apps, u32 app);
void WiFiPlazaApps_Start(WiFiPlazaApps *apps, u32 app);
BOOL WiFiPlazaApps_End(WiFiPlazaApps *apps, u32 app);
void ov45_0222D434(WiFiPlazaApps *apps, void **args, u32 mode);
void ov45_0222D500(u32 *out, s64 *seconds);

// wifi_plaza_util.c
void ov45_0222D524(const WiFiPlazaTime *left, const WiFiPlazaTime *right, WiFiPlazaTime *out);
void ov45_0222D594(const WiFiPlazaTime *first, const WiFiPlazaTime *second, WiFiPlazaTime *out);
void ov45_0222D638(WiFiPlazaPlace *places, u32 country, u32 region, u32 value);
u16 ov45_0222D6B0(WiFiPlazaPlace *places, u32 index);
u8 ov45_0222D6D4(WiFiPlazaPlace *places, u32 index);
u8 ov45_0222D6FC(WiFiPlazaPlace *places, u32 index);
u8 ov45_0222D724(WiFiPlazaPlace *places, u32 index);
void ov45_0222D740(void *file);
BOOL ov45_0222D79C(u32 country, u32 region);
u8 ov45_0222D7C0(u32 index);
int ov45_0222D7CC(u32 errorCode, u32 context);
BOOL ov45_0222D7FC(u32 errorCode, u32 context);
BOOL ov45_0222D844(void);
WiFiPlazaNews *ov45_0222D860(enum HeapID heapID);
void ov45_0222D890(WiFiPlazaNews *news);
void ov45_0222D8A4(WiFiPlazaNews *news);

// wifi_plaza_news.c
void ov45_0222D8BC(WiFiPlazaNews *news, u32 *openTime);
void ov45_0222D8C8(WiFiPlazaNews *news, int slot, int state, int flags);
BOOL ov45_0222D8D4(WiFiPlazaNews *news, int slot);
void ov45_0222D8F0(WiFiPlazaNews *news, int slot);
void ov45_0222D940(WiFiPlazaNews *news, WiFiPlazaPairRequest *req);
void ov45_0222D990(WiFiPlazaNews *news, WiFiPlazaPairOptionRequest *req);
void ov45_0222D9EC(WiFiPlazaNews *news, WiFiPlazaGroupFlagRequest *req);
void ov45_0222DA80(WiFiPlazaNews *news, WiFiPlazaSingleRequest *req);
void ov45_0222DAE0(WiFiPlazaNews *news, WiFiPlazaGroupRequest *req);
void ov45_0222DB3C(WiFiPlazaNews *news, WiFiPlazaGroupRequest *req);
void ov45_0222DB98(WiFiPlazaNews *news, WiFiPlazaLobbyNotice *notice);
void ov45_0222DC08(WiFiPlazaNews *news, int *slot, const u8 *flags);
void ov45_0222DC64(WiFiPlazaNews *news, WiFiPlazaGroupOptionRequest *req);
void ov45_0222DCE8(WiFiPlazaNews *news);
u32 ov45_0222DCF4(WiFiPlazaNews *news, u32 *openTime);
BOOL ov45_0222DCFC(WiFiPlazaNews *news, int slot);
int ov45_0222DD08(WiFiPlazaNews *news, int slot);
BOOL ov45_0222DD14(WiFiPlazaNews *news, int slot);
BOOL ov45_0222DD20(WiFiPlazaNews *news, int slot);
BOOL ov45_0222DD2C(WiFiPlazaNews *news, int slot);
u16 ov45_0222DD38(WiFiPlazaNews *news);
BOOL ov45_0222DD44(WiFiPlazaNews *news);
u8 ov45_0222DD5C(WiFiPlazaNews *news);
BOOL ov45_0222DD78(WiFiPlazaNews *news, const u8 *flags, int unused, String *out, enum HeapID heapID);
int ov45_0222DDE4(WiFiPlazaNews *news, int unused, int index);
void ov45_0222DE1C(WiFiPlazaNewsSlots *slots);
void ov45_0222DE3C(WiFiPlazaNewsSlots *slots, int slot, int state, int flags);
void ov45_0222DE58(WiFiPlazaNewsSlots *slots, int slot, int state, int flags);
void ov45_0222DE74(WiFiPlazaNewsSlots *slots, int slot);
void ov45_0222DE8C(WiFiPlazaNewsSlots *slots, int slot, int flags);
void ov45_0222DEA4(WiFiPlazaNewsSlots *slots, int change);
void ov45_0222DEB8(WiFiPlazaNewsSlots *slots, int slot);
BOOL ov45_0222DECC(WiFiPlazaNewsSlots *slots, int slot);
int ov45_0222DEE0(WiFiPlazaNewsSlots *slots, int slot);
BOOL ov45_0222DEF4(WiFiPlazaNewsSlots *slots, int slot);
BOOL ov45_0222DF14(WiFiPlazaNewsSlots *slots, int slot);
int ov45_0222DF38(WiFiPlazaNewsSlots *slots, int slot);
u16 ov45_0222DF50(WiFiPlazaNewsSlots *slots);

// wifi_plaza_news_events.c
BOOL ov45_0222DF58(WiFiPlazaNewsSlots *slots, u32 slot);
void ov45_0222DF78(WiFiPlazaNewsEventList *list, enum HeapID heapID);
void ov45_0222DFD0(WiFiPlazaNewsEventList *list);
void ov45_0222E000(WiFiPlazaNewsEventList *list);
void ov45_0222E03C(WiFiPlazaNewsEventList *list);
WiFiPlazaNewsEvent *ov45_0222E04C(WiFiPlazaNewsEventList *list, u16 rank);
void ov45_0222E094(WiFiPlazaNewsEvent *at, WiFiPlazaNewsEvent *event);
void ov45_0222E0A4(WiFiPlazaNewsEventList *list, WiFiPlazaNewsEvent *event);
void ov45_0222E0CC(WiFiPlazaNewsEventList *list, WiFiPlazaNewsEvent *event);
void ov45_0222E0E0(WiFiPlazaNewsEvent *event, u32 a, u32 b, u32 c, const PlayerProfile *player0, const PlayerProfile *player1, const PlayerProfile *player2, const PlayerProfile *player3, u16 slot0, u16 slot1, u16 slot2, u16 slot3, u32 timer, u32 rank, u32 type);
BOOL ov45_0222E14C(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out);
BOOL ov45_0222E1A0(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out);
BOOL ov45_0222E200(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out);
BOOL ov45_0222E33C(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out);
BOOL ov45_0222E39C(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out);
BOOL ov45_0222E414(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out);
BOOL ov45_0222E484(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out);
BOOL ov45_0222E4A8(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out);
u16 ov45_0222E540(WiFiPlazaNewsEvent *event, u32 index);
u16 ov45_0222E550(WiFiPlazaNewsEvent *event, u32 index);
u16 ov45_0222E560(WiFiPlazaNewsEvent *event, u32 index);
u16 ov45_0222E574(WiFiPlazaNewsEvent *event, u32 index);
u16 ov45_0222E584(WiFiPlazaNewsEvent *event, u32 index);
u16 ov45_0222E598(WiFiPlazaNewsEvent *event, u32 index);
u16 ov45_0222E59C(WiFiPlazaNewsEvent *event, u32 index);

// wifi_plaza_lobby_client.c
extern WiFiPlazaLobbyClient *_022577C0;
u16 ov45_0222E5A0(WiFiPlazaNewsEvent *event, u32 index);
BOOL ov45_0222E5B4(u32 activity, u32 count);
void WiFiPlazaLobbyClient_Init(enum HeapID heapID, SaveData *save, u32 count, WiFiPlazaCallbacks *callbacks, void *callbackArg);
void WiFiPlazaLobbyClient_Free(void);
int WiFiPlazaLobbyClient_Update(void);
void ov45_0222E7CC(void);
void ov45_0222E7FC(void);
void ov45_0222E804(int value);
void ov45_0222E810(int value, int mode);
BOOL ov45_0222E8F4(void);
void ov45_0222E91C(void);
BOOL ov45_0222E944(void);
int ov45_0222E96C(void);
u8 ov45_0222E9BC(void);
int WiFiPlazaLobbyClient_GetOwnId(void);
void ov45_0222E9F8(WiFiPlazaPlayer *player);
WiFiPlazaPlayer *ov45_0222EA2C(int id);
void ov45_0222EA4C(int id, s64 *time);
int WiFiPlazaLobbyClient_GetOldestPlayer(void);
int WiFiPlazaLobbyClient_GetOldestOtherPlayer(int excluded);
BOOL ov45_0222EB38(int channel);
u8 ov45_0222EB74(void);
BOOL ov45_0222EB94(void);
BOOL ov45_0222EBC4(void);
BOOL ov45_0222EBF0(int channel);
void ov45_0222EC10(WiFiPlazaPlayerList *list);
void ov45_0222EC3C(u32 *out);
int ov45_0222EC68(int id);
int ov45_0222EC7C(int id);
int ov45_0222EC90(int slot);
int ov45_0222ECA4(int slot);
void ov45_0222ECB8(s64 *time);
u32 ov45_0222ECDC(int field);
int ov45_0222ED7C(void);
BOOL ov45_0222EDA8(void);
u32 ov45_0222EDC4(u8 slot);
u8 ov45_0222EDF0(u8 slot);
void WiFiPlazaLobbyClient_SetLobbyCommands(const void *commands, u32 count, void *arg);
void WiFiPlazaLobbyClient_SetChannelCommands(const void *commands, u32 count, void *arg);
void WiFiPlazaLobbyClient_ClearLobbyCommands(void);
void WiFiPlazaLobbyClient_ClearChannelCommands(void);
void WiFiPlazaLobbyClient_SendToLobby(int type, const void *data, int size);
void WiFiPlazaLobbyClient_SendToPlayer(int type, int id, const void *data, int size);
void WiFiPlazaLobbyClient_SendToChannel(int type, const void *data, int size);
BOOL ov45_0222F028(int index, int maxPlayers);
void ov45_0222F154(void);
void ov45_0222F1BC(void);
BOOL ov45_0222F218(void);
u8 ov45_0222F254(void);
BOOL ov45_0222F274(int index);
BOOL ov45_0222F294(int index);
int ov45_0222F2D4(int index);
u32 ov45_0222F314(int index);
BOOL ov45_0222F350(int index);
u32 ov45_0222F3E8(int index);
int ov45_0222F430(void);
u8 ov45_0222F464(void);
int ov45_0222F484(int id);
int ov45_0222F4AC(int id);
void ov45_0222F4D4(int index);
u32 ov45_0222F524(void);
u32 ov45_0222F544(int field);
void *ov45_0222F628(int field);
u8 ov45_0222F6B0(int which, int index);
void ov45_0222F70C(void);
BOOL WiFiPlazaLobbyClient_CheckResult(int result);
void ov45_0222F78C(void);
int ov45_0222F7A8(int libChannel);
int ov45_0222F7B0(int channel);

// wifi_plaza_lobby_players.c
BOOL ov45_0222F7B4(WiFiPlazaLobbyClient *client);
void ov45_0222F848(WiFiPlazaLobbyClient *client, u32 size, enum HeapID heapID);
void ov45_0222F878(WiFiPlazaLobbyClient *client);
void ov45_0222F88C(WiFiPlazaLobbyClient *client, int id, const void *data);
void ov45_0222F898(WiFiPlazaLobbyClient *client, int id, const void *data, u32 size);
WiFiPlazaPlayer *ov45_0222F8D8(WiFiPlazaLobbyClient *client, int id);
void ov45_0222F8FC(WiFiPlazaLobbyClient *client, int id);
void ov45_0222F928(WiFiPlazaLobbyClient *client, int id, const WiFiPlazaLobbyPlayerRecord *record);
WiFiPlazaLobbyPlayerRecord *ov45_0222F954(WiFiPlazaLobbyClient *client, int id);
void ov45_0222F978(WiFiPlazaLobbyClient *client, int id);
void ov45_0222F9B8(WiFiPlazaLobbyClient *client, u32 lobbyCapacity, u32 channelCapacity, enum HeapID heapID);
void ov45_0222FA10(WiFiPlazaLobbyClient *client);
void ov45_0222FA40(WiFiPlazaLobbyClient *client, int which);
void ov45_0222FA74(WiFiPlazaLobbyClient *client, int id, int which);
void ov45_0222FAF8(WiFiPlazaLobbyClient *client, int id, int which);
int ov45_0222FB24(WiFiPlazaLobbyClient *client, int id, int which);
int ov45_0222FB5C(WiFiPlazaLobbyClient *client, int slot, int which);
void ov45_0222FB84(WiFiPlazaLobbyClient *client, u32 channel);

// wifi_plaza_lobby_events.c
BOOL ov45_0222FBD8(WiFiPlazaLobbyClient *client, int channel);
void ov45_0222FC44(int result, u32 libChannel);
void ov45_0222FCE0(int id, const WiFiPlazaLobbyPlayerRecord *record, const void *data, u32 size);
void ov45_0222FD50(int id);
void ov45_0222FDD4(void);
void ov45_0222FDD8(int id, u32 libChannel, u32 type, const void *data, u32 size);
void ov45_0222FE84(int a0, int a1, int a2, int a3, const char *key, const void *data);
void ov45_0222FEC4(int id, const WiFiPlazaLobbyPlayerRecord *record);
void ov45_0222FF40(int id, const void *data);
void ov45_0222FF7C(int id, WiFiPlazaLobbyMatchInfo *info);
void ov45_02230008(int id, const WiFiPlazaLobbyMatchInfo *info);
void ov45_02230050(u32 event);
void ov45_02230064(int result, const WiFiPlazaPlayer *data, u32 size);
BOOL ov45_02230090(int result);
void ov45_022300B0(int result, const void *data, u32 size);
void ov45_022300DC(int result, const void *data);
void ov45_02230108(void);
void ov45_02230130(void);
void ov45_02230144(WiFiPlazaLobbyClient *client);

// wifi_plaza_lobby_matches.c
void ov45_02230164(WiFiPlazaLobbyClient *client, int index, int hostId, const WiFiPlazaLobbyMatchInfo *info);
BOOL ov45_022301E0(WiFiPlazaLobbyClient *client, int index, int hostId);
BOOL ov45_0223021C(WiFiPlazaLobbyClient *client, int index);
u16 ov45_0223023C(WiFiPlazaLobbyClient *client, int index);
WiFiPlazaLobbyMatch *ov45_022302B0(WiFiPlazaLobbyClient *client, int index);
void ov45_022302E4(WiFiPlazaLobbyClient *client);
void ov45_02230378(WiFiPlazaLobbyMatchInfo *info, const s64 *time);
void ov45_02230384(const WiFiPlazaLobbyMatchInfo *info, s64 *time);
void ov45_02230390(int result, void *arg);

// wifi_plaza_player_objects.c
void ov45_02230394(int result, void *arg);
u32 ov45_022303BC(WiFiPlazaLobbyClientUnk1B4 *unk, int key);
void ov45_022303E4(WiFiPlazaLobbyClientUnk6A0 *unk, int count, enum HeapID heapID);
void ov45_022303FC(WiFiPlazaLobbyClientUnk6A0 *unk);
void ov45_0223040C(WiFiPlazaLobbyClientUnk6A0 *unk, const void *data, u32 count);
BOOL ov45_02230434(WiFiPlazaLobbyClientUnk6A0 *unk, int id);
u32 ov45_0223045C(WiFiPlazaLobbyClientUnk6A0 *unk, int id);
void ov45_0223048C(u32 *dest, const void *data);
WiFiPlazaPlayerObjects *ov45_02230498(u32 count, BOOL mode, enum HeapID heapID, enum HeapID loadHeapID);
void ov45_02230638(WiFiPlazaPlayerObjects *objects);
void ov45_02230680(WiFiPlazaPlayerObjects *objects);
void ov45_022306B4(WiFiPlazaPlayerObjects *objects);
void ov45_022306F4(WiFiPlazaPlayerObjects *objects);
void ov45_02230700(WiFiPlazaPlayerObjects *objects, int alpha);
WiFiPlazaPlayerObject *ov45_0223070C(WiFiPlazaPlayerObjects *objects, UnkStruct_ov42_02228110 *player);
void ov45_02230884(WiFiPlazaPlayerObject *object);
void ov45_0223089C(WiFiPlazaPlayerObject *object, u32 mode);
void ov45_022308B0(WiFiPlazaPlayerObject *object, u32 lightFlags);
u32 ov45_022308B8(WiFiPlazaPlayerObject *object);
void ov45_022308C0(WiFiPlazaPlayerObject *object, const UnkStruct_ov44_02232914 *gridPos);
void ov45_022308E4(WiFiPlazaPlayerObject *object, const VecFx32 *position);
void ov45_02230908(WiFiPlazaPlayerObject *object, VecFx32 *position);
void ov45_02230920(WiFiPlazaPlayerObject *object, u32 sequence);
void ov45_0223093C(WiFiPlazaPlayerObject *object, u32 effect);
void ov45_02230968(WiFiPlazaPlayerObject *object);
void ov45_02230974(WiFiPlazaPlayerObject *object, u8 step);
void ov45_02230978(WiFiPlazaPlayerObject *object, int visible);
u32 ov45_02230994(WiFiPlazaPlayerObject *object);
const WiFiPlazaSpriteConfig *ov45_0223099C(int spriteId);
u32 ov45_022309C4(int standing, u32 sequence);
u32 ov45_022309D0(u32 divisor, u16 value, u32 factor);
void ov45_022309E8(WiFiPlazaObjectModel *model, NARC *narc, int fileId, enum HeapID heapID);
void ov45_02230A44(WiFiPlazaObjectModel *model);
void ov45_02230A4C(WiFiPlazaObjectModel *model, int alpha);
int ov45_02230A58(WiFiPlazaObjectModel *model);
void ov45_02230A5C(ModelResource *resource, ModelInstance *instance);
WiFiPlazaPlayerObject *ov45_02230A6C(WiFiPlazaPlayerObjects *objects);
BOOL ov45_02230AA4(WiFiPlazaPlayerObject *object);
void ov45_02230AB4(WiFiPlazaPlayerObject *object);
void ov45_02230AC0(WiFiPlazaPlayerObject *object);
void ov45_02230ACC(WiFiPlazaPlayerObject *object);
void ov45_02230B64(WiFiPlazaPlayerObject *object);
void ov45_02230B8C(WiFiPlazaPlayerObject *object);
void ov45_02230BFC(WiFiPlazaPlayerObject *object);
void ov45_02230C40(WiFiPlazaPlayerObject *object);
void ov45_02230CB0(WiFiPlazaPlayerObject *object);
void ov45_02230CD8(WiFiPlazaPlayerObject *object);
void ov45_02230D20(WiFiPlazaPlayerObject *object);
void ov45_02230D5C(WiFiPlazaPlayerObject *object);
BOOL ov45_02230DC4(u32 action);
void ov45_02230DF4(WiFiPlazaPlayerObject *object);
void ov45_02230E28(WiFiPlazaPlayerObject *object);
void ov45_02230E64(void *sprite, WiFiPlazaPlayerObject *object);

// wifi_plaza_sprite_culling.c
BOOL ov45_02230E78(void *sprite);

// wifi_plaza_time_zone.c
WiFiPlazaTime ov45_02230F94(int country, u16 region, u32 time, enum HeapID heapID);
void ov45_02231018(NARC *narc, enum HeapID heapID, int country, u16 region, s32 *longitude);

// data (asm)
extern const WiFiPlazaTimerPhase _02254A28[5];
extern const u8 ov45_02254BB4[];
extern const u8 ov45_02254BC8[];  // news event rank per type of ov45_0222DC64
extern const u16 ov45_02254BCA[]; // message per player count (ov45_0222E4A8)
extern const u16 ov45_02254BD2[];
extern const u32 ov45_02254BDC[]; // news event rank per type of ov45_0222D9EC
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

extern const u8 ov45_02254C34[4];   // sprite directions of effect ov45_02230CD8
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
