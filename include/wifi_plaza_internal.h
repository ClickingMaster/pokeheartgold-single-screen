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
    u32 unk_f8[4];    // bit flags (ov45_0222A230 / A25C / A2A0 test and clear one, ov45_0222A288 tests one)
    u8 profile[0xb8]; // WiFiPlazaProfile_InitFromSave; CRC16 at 0xb4
    u8 unk_1c0[0x28];
    u8 unk_1e8[0x14];
    WiFiPlazaUnk1FC unk_1fc;
    u8 unk_20c[0x194];
    s16 unk_3a0[6]; // ov45_0222BCA0 / ov45_0222BCA8 / ov45_0222BD24 / ov45_0222BD2C
    u8 unk_3ac[0x34];
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

// wifi_plaza.c
BOOL WiFiPlaza_Init(OverlayManager *man, int *state);
BOOL WiFiPlaza_Main(OverlayManager *man, int *state);
BOOL WiFiPlaza_Exit(OverlayManager *man, int *state);
void WiFiPlaza_VBlankTask(SysTask *task, void *data);
WiFiPlazaWork *WiFiPlazaWork_New(SaveData *save, enum HeapID heapID);
void WiFiPlazaWork_Delete(WiFiPlazaWork *work);
void WiFiPlazaWork_Update(WiFiPlazaWork *work);

// defined in the other overlay 45 files
void ov45_0222AB0C(void *a0, u32 *a1);
void ov45_0222B2B4(void);
void ov45_0222B470(void);
void ov45_0222B530(void);
void ov45_0222B5A0(void);
void ov45_0222B75C(void);
void ov45_0222B79C(u8 *profile, void *out);
void ov45_0222B840(WiFiPlazaWork *work);
void WiFiPlazaProfile_InitFromSave(void *profile, SaveData *save, enum HeapID heapID);
void ov45_0222BB58(void *a0);
void ov45_0222BB60(void *a0, void *a1, void *a2, void *a3);
void ov45_0222BC3C(void *a0);
void ov45_0222BCA0(s16 *a0);
int ov45_0222BCA8(s16 *a0);
void ov45_0222BCB8(void *a0);
void ov45_0222BCC8(void *a0, enum HeapID heapID);
void ov45_0222BCD8(void *a0);
void ov45_0222BD24(u8 *a0);
u8 ov45_0222BD2C(u8 *a0);
void ov45_0222BD30(void *a0);
void ov45_0222BD40(void *a0);
void ov45_0222BE5C(void *a0);
void ov45_0222C388(void *a0);
void ov45_0222C3B0(void *a0);
void ov45_0222C8AC(void *a0);
void WiFiPlazaWordSet_Init(void *wordSet, enum HeapID heapID);
void WiFiPlazaWordSet_Free(void *wordSet);
void ov45_0222CAA0(void *a0);
void ov45_0222CB44(void *a0, SaveData *save);
int ov45_0222CCDC(void *a0);
void *WiFiPlazaApps_New(void *a0, SaveData *save, void *a2, WiFiPlazaWork *work, enum HeapID heapID);
void WiFiPlazaApps_Delete(void *apps);
void WiFiPlazaApps_StartFirst(void *apps);
int WiFiPlazaApps_Run(void *apps);
void WiFiPlazaApps_VBlank(void *apps);
void ov45_0222D500(u32 *a0, s64 *a1);
void *ov45_0222D860(enum HeapID heapID);
void ov45_0222D890(void *a0);
void ov45_0222D8A4(void *a0);
void ov45_0222E5D4(enum HeapID heapID, SaveData *save, u32 a2, WiFiPlazaCallbacks *callbacks, void *arg);
void ov45_0222E688(void);
u8 ov45_0222E9BC(void);
void *ov45_0222E9E0(void);
int ov45_0222EC68(int a0);
void ov45_0222ECB8(s64 *a0);
void ov45_0222EE20(void *a0, u32 a1, void *a2);
void ov45_0222EE80(void);
u32 ov45_0222ECDC(int field);

extern u8 ov45_02254AC4[];

// defined in other overlays / the ARM9 static part
void *sub_020318E8(SaveData *save);
void sub_020318FC(void *a0, u32 a1);
void sub_02031900(void *a0, u32 a1);

#endif // POKEHEARTGOLD_WIFI_PLAZA_INTERNAL_H
