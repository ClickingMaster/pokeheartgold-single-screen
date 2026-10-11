#ifndef POKEHEARTGOLD_WIFI_PLAZA_INTERNAL_H
#define POKEHEARTGOLD_WIFI_PLAZA_INTERNAL_H

// Wi-Fi Plaza (overlay 45): the overlay's work data and the functions shared between its source files.

#include "global.h"

#include "easy_chat.h"
#include "heap.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "save.h"
#include "sys_task_api.h"

// Arguments of the Wi-Fi Plaza overlay (field_transition_tasks_2.c).
typedef struct WiFiPlazaArgs {
    void *unk_00;
    SaveData *save;
    void *unk_08;
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
    void *unk_1c; // ov45_02230F94 of country, region and unk_18 (4 bytes)
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
    u16 unk_20;    // sent with ov45_0222EF4C (4 bytes from here)
    u16 unk_22;
    u16 unk_24; // ov45_0222BE48 sets it, ov45_0222BE54 clears it
    s16 unk_26; // counts up to 900 while unk_24 is set (ov45_0222BE5C)
} WiFiPlazaUnk1C0;

// A message to the lobby (ov45_0222EEF0 type 4).
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
    void *unk_1c; // sub_02078DD8 (ov45_0222B244 / ov45_0222B270)
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

typedef struct WiFiPlazaWork {
    SaveData *save;
    void *unk_04; // ov45_0222D860
    u32 unk_08;
    u8 unk_0c[0xc8]; // filled by ov45_0222B79C from the profile
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

// Result of ov45_02230F94, kept in WiFiPlazaPlayer.unk_1c.
typedef struct WiFiPlazaPlayerUnk1C {
    u32 value;
} WiFiPlazaPlayerUnk1C;

// Lobby callbacks given to ov45_0222E5D4 (with the work as the last argument).
typedef struct WiFiPlazaCallbacks {
    void (*onPlayerEnter)(int id, WiFiPlazaPlayer *player, WiFiPlazaWork *work, BOOL isOwn);
    void (*onPlayerLeave)(int id, WiFiPlazaWork *work);
    void (*onPlayerUpdate)(int id, WiFiPlazaPlayer *player, WiFiPlazaWork *work);
    void (*onLobbyEvent)(u32 event, WiFiPlazaWork *work);
    void (*onOwnRecord)(const WiFiPlazaPlayer *data, u32 size, WiFiPlazaWork *work);
} WiFiPlazaCallbacks;

// wifi_plaza_work.c
void ov45_0222A1F8(WiFiPlazaWork *work);
u32 ov45_0222A1FC(WiFiPlazaWork *work);
void ov45_0222A208(void *a0, void *a1, void *a2);
void *ov45_0222A210(WiFiPlazaWork *work);
void *ov45_0222A214(WiFiPlazaWork *work);
void *ov45_0222A22C(WiFiPlazaWork *work);
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
void ov45_0222A450(WiFiPlazaWork *work, void *player, u32 index);
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
void ov45_0222A72C(WiFiPlazaWork *work, u8 value);
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
int ov45_0222AD2C(void);
int ov45_0222AD3C(void);
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
void ov45_0222B79C(WiFiPlazaProfile *profile, void *out);
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

// data (asm)
extern const WiFiPlazaTimerPhase _02254A28[5];
// start time of each group
extern const s32 ov45_02254A3C[9];
// handlers per lobby message type
extern void (*const ov45_02254A60[9])(WiFiPlazaWork *work, WiFiPlazaLobbyMessage *msg);

// defined in the other overlay 45 files
u16 ov45_0222CCE4(u16 avatar);
u16 ov45_0222CD04(u16 a0);
void *WiFiPlazaApps_New(void *a0, SaveData *save, void *a2, WiFiPlazaWork *work, enum HeapID heapID);
void WiFiPlazaApps_Delete(void *apps);
void WiFiPlazaApps_StartFirst(void *apps);
int WiFiPlazaApps_Run(void *apps);
void WiFiPlazaApps_VBlank(void *apps);
void ov45_0222D500(u32 *a0, s64 *a1);
void ov45_0222D638(void *a0, u16 country, u8 region, int a3);
BOOL ov45_0222D79C(u16 country, u8 region);
void *ov45_0222D860(enum HeapID heapID);
void ov45_0222D890(void *a0);
void ov45_0222D8A4(void *a0);
void ov45_0222D8BC(void *a0, u32 *a1);
void ov45_0222D8C8(void *a0, int slot, int gender, int a3);
int ov45_0222D8D4(void *a0, int slot);
void ov45_0222D8F0(void *a0, int slot);
void ov45_0222D940(void *a0, void *request);
void ov45_0222D990(void *a0, void *request);
void ov45_0222D9EC(void *a0, void *request);
void ov45_0222DA80(void *a0, void *request);
void ov45_0222DAE0(void *a0, void *request);
void ov45_0222DB3C(void *a0, void *request);
void ov45_0222DB98(void *a0, void *notice);
void ov45_0222DC08(void *a0, int *slot, void *a2);
void ov45_0222DC64(void *a0, void *request);
void ov45_0222E5D4(enum HeapID heapID, SaveData *save, u32 a2, WiFiPlazaCallbacks *callbacks, void *arg);
void ov45_0222E688(void);
int ov45_0222E96C(void);
u8 ov45_0222E9BC(void);
void *ov45_0222E9E0(void);
void ov45_0222E9F8(WiFiPlazaPlayer *player);
WiFiPlazaPlayer *ov45_0222EA2C(int id);
void ov45_0222EA4C(int id, s64 *time);
int ov45_0222EA78(int id);
int ov45_0222EAD4(int id);
void ov45_0222EC10(WiFiPlazaPlayerList *list);
void ov45_0222EC3C(u32 *out);
int ov45_0222EC68(int a0);
int ov45_0222EC90(int slot);
int ov45_0222ECA4(int a0);
void ov45_0222ECB8(s64 *a0);
void ov45_0222EE20(void *a0, u32 a1, void *a2);
void ov45_0222EE80(void);
void ov45_0222EEF0(int type, const void *data, int size);
void ov45_0222EF4C(int type, int id, void *data, int size);
u32 ov45_0222ECDC(int field);
u32 ov45_0222EDC4(u8 index);
u8 ov45_0222EDF0(u8 index);
int ov45_0222F484(int id);
int ov45_0222F4AC(int id);
WiFiPlazaPlayerUnk1C ov45_02230F94(u16 country, u8 region, u32 time, enum HeapID heapID);
void ov45_022320C4(void *a0);

extern const WiFiPlazaAvatarEntry ov45_02254A84[16];
extern u8 ov45_02254AC4[];

// defined in other overlays / the ARM9 static part
void *Save_PlazaSurvey_Get(SaveData *save);
void PlazaSurvey_SetSurveyId(void *a0, u32 a1);
void PlazaSurvey_SetAnswer(void *a0, u32 a1);
void *sub_02078DD8(PlayerProfile *profile, enum HeapID heapID);
void sub_02078E28(void *a0);

#endif // POKEHEARTGOLD_WIFI_PLAZA_INTERNAL_H
