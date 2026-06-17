#ifndef POKEHEARTGOLD_OVERLAY_03_H
#define POKEHEARTGOLD_OVERLAY_03_H

#include "bag_types_def.h"
#include "dex_mon_measures.h"
#include "game_stats.h"
#include "list_menu.h"
#include "mart.h"
#include "safari_zone.h"
#include "save_vars_flags.h"
#include "script.h"
#include "unk_02031B0C.h"

#include "field/ov01_021E7FDC.h"
#include "pokeathlon/pokeathlon_save.h"

enum PokeathlonData {
    OVERALL_DATA,
    EVENT_DATA,
    ACTION_DATA
};

enum MartTypes {
    MART_TYPE_NORMAL = 0,
    MART_TYPE_1, // MART_TYPE_DECOR? Would be unused in HGSS if true. Interesting that it was reserved instead of replaced.
    MART_TYPE_SEAL,
    MART_TYPE_3, // MART_TYPE_FRONTIER?
    MART_TYPE_4, // MART_TYPE_POKEATHLON?
};

typedef struct UnkMiniStruct {
    u8 unk0[4];
} UnkMiniStruct;

typedef struct MartData {
    BgConfig *bgConfig;
    BagView *bagView;
    Window windows[6];
    u8 unk68[0x20];
    MsgData *messageData;
    MessageFormat *messageFormat;
    Camera *camera;
    UnkStruct_ov01_021E7FDC unk_ov01_021E7FDC;
    Sprite *sprites[19];
    u16 spriteDrawn[2]; // Draw flags for sprites 0 and 1.
    PlayerProfile *playerProfile;
    void *inventory;
    Options *options;
    PokeathlonSave *pokeathlonSave;
    SaveApricornBox *apricornBox;
    GameStats *gameStats;
    SaveVarsFlags *varsFlags;
    u32 unk264;
    u16 *unk268;
    const struct MartItem *priceOverrides;
    u8 unk270;
    u8 unk271;
    u8 state;
    u8 unk273;
    String *string;
    u8 bgPriority[4];
    u32 engineAPlanes;
    u8 printerID;
    u8 unk281;
    u8 unk282;
    u8 martType;
    u16 item;
    s16 quantity;
    u16 unk288;
    u16 unk28A;
    int cost;
    int unk290;
    u32 unk294;
    u32 unk298;
    UnkMiniStruct unk29C;
    u32 unk2A0;
} MartData;

typedef struct UnkCommStruct {
    void (*func)();
    SafariZone *safariZone;
    u8 unk8[0x5D0];
} UnkCommStruct; // Size: 0x5D8

typedef struct UnkStruct_02258CFC {
    int state;
    enum PokeathlonData pokeathlonDataType;
    FieldSystem *fieldSystem;
    BgConfig *bgConfig;
    Window window;
    PokeathlonSave *pokeathlonSave;
    Pokeathlon_UnkSubStruct_B00 *pokeathlonStruct;
    MsgData *msgData;
    MessageFormat *messageFormat;
    String *strings[4];
} UnkStruct_02258CFC;

// Shared between files within the overlay:

// Called from outside the overlay:
void ov03_02255BB0(FieldSystem *fieldSystem, u32 arg1, u16 arg2, u16 arg3);
void ov03_02255BEC();
u32 ov03_02255BFC();
void ov03_02255C18(FieldSystem *fieldSystem, u32 arg1, u16 arg2, u16 arg3);
void ov03_02255C54();
u32 ov03_02255C64();

void StartTask_BattleRegulationMenu(TaskManager *taskManager, u16 *result);
void ov03_022566D0(FieldSystem *fieldSystem, MessageFormat *messageFormat, u32 ruleset);
void ov03_02256710(FieldSystem *fieldSystem, u16 a1);
void ov03_02256730(FieldSystem *fieldSystem, Window *window, u32 ruleset);
u16 ov03_02256A2C(FieldSystem *fieldSystem, MessageFormat *msgFmt, u32 a2);
u32 ov03_02256B40(int);

BOOL Task_Mart(TaskManager *taskManager);
int ov03_02257814(MartData *data, u32 unkAmount);
int ov03_02257978(MartData *data, int itemID);
u32 ov03_02258120(MartData *data, u16 itemID);
void ov03_022581BC(MartData *data);
void ov03_02258910(FieldSystem *fieldSystem);
void ov03_02258CFC(TaskManager *taskManager, enum PokeathlonData data);

// TODO: Move these to where they belong.
BOOL ov01_021F3B30();
void ov01_021F630C(int, FieldSystemUnkSub2C *, s32 *);
u8 ov01_021F6320(FieldSystemUnkSub2C *);
void ov01_021F3B2C(int, int);
void ov01_021F3B0C(VecFx32 *vec, int);
int ov01_021F3B44(int, u8 index);
u16 *ov01_021F65E4(FieldSystemUnkSub2C *, u8);
u8 *ov01_021F65F0(FieldSystemUnkSub2C *, u8);

u16 ov02_0224E698(FieldSystem *fieldSystem);
u16 ov02_0224E754(FieldSystem *fieldSystem, u16 *varPointer);

s32 sub_020348F0();
int sub_0203511C();
u32 sub_02035150(u32 index);
u8 sub_02035184();
void sub_02035198();
void sub_020351DC(u16 cursorPos, PlayerProfile *profile);
void sub_020356C0(u32 arg0);
int sub_02037454();
void sub_020376D4(s32, SafariZoneAreaSet *areaSet);
BOOL sub_02037700();
int sub_0203774C(u32 arg0);
int sub_0203775C(u32 arg0);
BOOL sub_02037A10();
void sub_02037B8C(u32, u8);
s32 sub_02037BA0(s32, s32);
void sub_02037BC8();
void sub_02037EC0(u32 arg0);
void sub_020580E4(FieldSystem *fieldSystem, s32 arg1, s32 arg2);
void sub_02058124(FieldSystem *fieldSystem, u8, u32);
void sub_02058164(u16 arg0);
void sub_02058180();
void sub_02058284();
int sub_020881C0(s16 *quantity, u16 a1);
BOOL sub_02092B04(TaskManager *taskManager);

#endif // POKEHEARTGOLD_OVERLAY_03_H
