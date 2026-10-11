#ifndef POKEHEARTGOLD_WIFI_PLAZA_INTERNAL_H
#define POKEHEARTGOLD_WIFI_PLAZA_INTERNAL_H

// Wi-Fi Plaza (overlay 45): the overlay's work data and the functions shared between its source files.

#include "global.h"

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
    u8 unk_18[4];
    void *unk_1c;
    u8 unk_20[0x18];
    u8 gender; // >= 2: taken from the avatar table
    u8 language;
    u16 unk_3a; // mapped to the avatar by ov45_0222CD04
    u16 country;
    u8 region;
    u8 unk_3f[2];
    u8 unk_41; // < 27
    u8 unk_42;
    u8 unk_43; // < 15
    u8 unk_44[8];
    u8 slots[12]; // < 24: used
    void *slotData[12];
    u16 unk_88[2]; // < 18
    WiFiPlazaPlayerUnk8C unk_8c;
} WiFiPlazaPlayer;

// The own plaza profile (WiFiPlazaProfile_InitFromSave); WiFiPlazaProfile_UpdateChecksum and WiFiPlazaProfile_IsChecksumValid keep its CRC.
typedef struct WiFiPlazaProfile {
    u8 unk_00[0x20];
    WiFiPlazaPlayer player;
    u16 crc;
    u8 unk_b6[2];
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
    u32 unk_00;
    u16 unk_04; // lobby slot of the other player
    u8 unk_06;  // state
    u8 unk_07;
    u16 unk_08;
    u8 unk_0a; // kind: 0 = none, 1 / 2 (ov45_0222AFF8 / ov45_0222B00C)
    u8 unk_0b;
    u8 unk_0c[20]; // per lobby slot, < 6 (ov45_0222B048)
    u16 unk_20;    // sent with ov45_0222EF4C (4 bytes from here)
    u16 unk_22;
    u8 unk_24[4];
} WiFiPlazaUnk1C0;

// A message to the lobby (ov45_0222EEF0 type 4).
typedef struct WiFiPlazaLobbyMessage {
    u32 ids[4]; // ov45_0222EC90 of lobby slots
    u8 unk_10;
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

// Part of WiFiPlazaWork at 0x508 (ov45_0222BCC8 / ov45_0222BCD8 set it up and free it).
typedef struct WiFiPlazaUnk508 {
    void *unk_00;
    void *unk_04;
    u8 unk_08[0x18];
} WiFiPlazaUnk508;

// Plaza work data shared by the sub-applications (WiFiPlazaWork_New).
typedef struct WiFiPlazaWork {
    SaveData *save;
    void *unk_04; // ov45_0222D860
    u32 unk_08;
    u8 unk_0c[0xc8]; // filled by ov45_0222B79C from the profile
    u32 unk_d4;
    s64 unk_d8; // ov45_0222ECB8, refreshed every frame
    u8 unk_e0[8];
    PlayerProfile *profiles[4];
    u32 unk_f8[4]; // bit flags (ov45_0222A230 / A25C / A2A0 test and clear one, ov45_0222A288 tests one)
    WiFiPlazaProfile profile;
    WiFiPlazaUnk1C0 unk_1c0;
    u8 unk_1e8[0x14];
    WiFiPlazaUnk1FC unk_1fc;
    u8 unk_20c[0x194];
    s16 unk_3a0[6]; // ov45_0222BCA0 / ov45_0222BCA8 / ov45_0222BD24 / ov45_0222BD2C
    u8 unk_3ac[0x20];
    u8 unk_3cc[0x14]; // copied out by ov45_0222AB38
    u8 unk_3e0[4];
    u8 wordSet[0xb8]; // WiFiPlazaWordSet_Init
    u8 unk_49c[0x20];
    u8 unk_4bc[0x4c];
    WiFiPlazaUnk508 unk_508;
    enum HeapID heapID;
    u32 unk_52c; // error flag
} WiFiPlazaWork;

// Callbacks given to ov45_0222E5D4.
typedef struct WiFiPlazaCallbacks {
    void (*func0)(void);
    void (*func1)(void);
    void (*func2)(void);
    void (*func3)(void);
    void (*func4)(void);
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
int ov45_0222AB58(WiFiPlazaWork *work, int a1);
u32 ov45_0222AB68(void);
int ov45_0222AB78(void *unused, int a1);
void ov45_0222AB94(void *unused, u32 slot0, u32 slot1);
void ov45_0222ABD0(void *unused, u32 slot0, u32 slot1, u8 value);
void ov45_0222AC14(void *unused, int selector, int count, int slot0, int slot1, int slot2, int slot3, int flag);
void ov45_0222ACB8(void *unused, int mode, u8 kind, u32 slot0, u32 slot1, u32 slot2, u32 slot3);
int ov45_0222AD2C(void);
int ov45_0222AD3C(void);
int ov45_0222AD4C(void);
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

// wifi_plaza.c
BOOL WiFiPlaza_Init(OverlayManager *man, int *state);
BOOL WiFiPlaza_Main(OverlayManager *man, int *state);
BOOL WiFiPlaza_Exit(OverlayManager *man, int *state);
void WiFiPlaza_VBlankTask(SysTask *task, void *data);
WiFiPlazaWork *WiFiPlazaWork_New(SaveData *save, enum HeapID heapID);
void WiFiPlazaWork_Delete(WiFiPlazaWork *work);
void WiFiPlazaWork_Update(WiFiPlazaWork *work);

// defined in the other overlay 45 files
void ov45_0222B2B4(void);
void ov45_0222B470(void);
void ov45_0222B530(void);
void ov45_0222B5A0(void);
void ov45_0222B75C(void);
void ov45_0222B118(WiFiPlazaWork *work, int slot);
BOOL ov45_0222B28C(const u16 *name, u32 count, enum HeapID heapID);
void ov45_0222B79C(WiFiPlazaProfile *profile, void *out);
void ov45_0222B840(WiFiPlazaWork *work);
void ov45_0222BA3C(WiFiPlazaWork *work);
void WiFiPlazaProfile_UpdateChecksum(WiFiPlazaProfile *profile, SaveData *save);
BOOL WiFiPlazaProfile_IsChecksumValid(WiFiPlazaProfile *profile, SaveData *save);
void WiFiPlazaProfile_InitFromSave(WiFiPlazaProfile *profile, SaveData *save, enum HeapID heapID);
void ov45_0222BB58(void *a0);
void ov45_0222BB60(void *a0, void *a1, void *a2, void *a3);
void ov45_0222BC3C(void *a0);
void ov45_0222BC84(WiFiPlazaUnk1FC *a0);
void ov45_0222BCA0(s16 *a0);
int ov45_0222BCA8(s16 *a0);
void ov45_0222BCB8(void *a0);
void ov45_0222BCC8(void *a0, enum HeapID heapID);
void ov45_0222BCD8(void *a0);
void ov45_0222BD24(u8 *a0);
u8 ov45_0222BD2C(u8 *a0);
void ov45_0222BD30(void *a0);
void ov45_0222BD40(WiFiPlazaUnk1C0 *a0);
void ov45_0222BD4C(WiFiPlazaUnk1C0 *a0);
void ov45_0222BD5C(WiFiPlazaUnk1C0 *a0);
void ov45_0222BE00(WiFiPlazaUnk1C0 *a0, int a1);
void ov45_0222BE28(WiFiPlazaWork *work, int id);
void ov45_0222BE48(WiFiPlazaUnk1C0 *a0);
void ov45_0222BE5C(WiFiPlazaUnk1C0 *a0);
BOOL ov45_0222BE74(WiFiPlazaUnk1C0 *a0);
void ov45_0222BE94(WiFiPlazaUnk1C0 *a0);
void ov45_0222C388(void *a0);
void ov45_0222C3B0(void *a0);
void ov45_0222C408(void *a0, int a1, int a2);
void ov45_0222C480(void *a0, int a1);
int ov45_0222C4E4(void *a0, int a1);
int ov45_0222C4FC(void *a0, int a1);
void ov45_0222C514(void *a0, int a1);
void ov45_0222C54C(void *a0, int a1);
void ov45_0222C580(void *a0, int a1);
int ov45_0222C5B4(void *a0, int a1);
void ov45_0222C5E8(void *a0, int a1, int a2);
void ov45_0222C658(void *a0, int a1);
void ov45_0222C8AC(void *a0);
int ov45_0222C95C(void *a0, int a1);
void WiFiPlazaWordSet_Init(void *wordSet, enum HeapID heapID);
void WiFiPlazaWordSet_Free(void *wordSet);
int ov45_0222C9D0(void *wordSet, int a1);
int ov45_0222C9EC(void *wordSet, int a1);
void ov45_0222CB74(void *a0, int a1, int a2);
void ov45_0222CAA0(void *a0);
void ov45_0222CB44(void *a0, SaveData *save);
int ov45_0222CCDC(void *a0);
u16 ov45_0222CD04(u16 a0);
void *WiFiPlazaApps_New(void *a0, SaveData *save, void *a2, WiFiPlazaWork *work, enum HeapID heapID);
void WiFiPlazaApps_Delete(void *apps);
void WiFiPlazaApps_StartFirst(void *apps);
int WiFiPlazaApps_Run(void *apps);
void WiFiPlazaApps_VBlank(void *apps);
void ov45_0222D500(u32 *a0, s64 *a1);
BOOL ov45_0222D79C(u16 country, u8 region);
void *ov45_0222D860(enum HeapID heapID);
void ov45_0222D890(void *a0);
void ov45_0222D8A4(void *a0);
void ov45_0222E5D4(enum HeapID heapID, SaveData *save, u32 a2, WiFiPlazaCallbacks *callbacks, void *arg);
void ov45_0222E688(void);
u8 ov45_0222E9BC(void);
void *ov45_0222E9E0(void);
WiFiPlazaPlayer *ov45_0222EA2C(int id);
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

extern const WiFiPlazaAvatarEntry ov45_02254A84[16];
extern u8 ov45_02254AC4[];

// defined in other overlays / the ARM9 static part
void *Save_PlazaSurvey_Get(SaveData *save);
void PlazaSurvey_SetSurveyId(void *a0, u32 a1);
void PlazaSurvey_SetAnswer(void *a0, u32 a1);

#endif // POKEHEARTGOLD_WIFI_PLAZA_INTERNAL_H
