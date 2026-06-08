#ifndef POKEHEARTGOLD_OVERLAY_03_H
#define POKEHEARTGOLD_OVERLAY_03_H

#include "dex_mon_measures.h"
#include "game_stats.h"
#include "list_menu.h"
#include "mart.h"
#include "save_vars_flags.h"
#include "script.h"
#include "unk_02031B0C.h"

#include "pokeathlon/pokeathlon_save.h"

enum PokeathlonData {
    OVERALL_DATA,
    EVENT_DATA,
    ACTION_DATA
};

typedef struct UnkStruct_ov03 {
    String *string[8];
    Window windows[3];
    MessageFormat *messageFormat[3];
    ListMenu *listMenu;
    u32 unk60;
    LISTMENUITEM *items;
    FieldSystem *fieldSystem;
    void (*unk6C)();
    u32 unk70;
    MsgData *msgData;
    PlayerProfile *playerProfile[2];
    u8 unk80[8];
    u32 unk88;
    u16 unk8C;
    u16 unk8E;
    u8 unk90;
    u8 unk91;
    u8 unk92;
    u8 unk93;
    u8 printerID;
    u8 unk95;
    u8 unk96;
    u8 unk97;
    u8 unk98;
    u8 unk99;
    u16 unk9A;
    void *unk9C;
    u8 unkA0;
    u8 unkA1;
    u16 unkA2;
    u32 unkA4;
    u32 unkA8;
} UnkStruct_ov03;

enum {
    REGULATION_MENU_REGULATIONS,
    REGULATION_MENU_CONFIRM,
    REGULATION_MENU_MAX,
};

enum {
    REGULATION_MENU_STRING_FMT,
    REGULATION_MENU_STRING_DESTINATION,
    REGULATION_MENU_STRING_REGULATION_NAME,
    REGULATION_MENU_STRING_CUP_NAME,
    REGULATION_MENU_STRING_MAX,
};

enum {
    REGULATION_MENU_WINDOW_RULES,
    REGULATION_MENU_WINDOW_REGULATIONS,
    REGULATION_MENU_WINDOW_CONFIRM,
    REGULATION_MENU_WINDOW_MSGBOX,
    REGULATION_MENU_WINDOW_MAX,
};

typedef struct BattleRegulationMenu {
    ListMenu *listMenu[REGULATION_MENU_MAX];
    LISTMENUITEM *items[REGULATION_MENU_MAX];
    FieldSystem *fieldSystem;
    String *strings[REGULATION_MENU_STRING_MAX];
    Window windows[REGULATION_MENU_WINDOW_MAX];
    MessageFormat *messageFormat;
    MsgData *msgData;
    PokedexData *pokedexData;
    u16 *result;
    int printerID;
    u16 itemsAbove[REGULATION_MENU_MAX];
    int state;
    u16 unk80[REGULATION_MENU_MAX];
} BattleRegulationMenu;

extern u32 ov03_02259130;
extern u32 ov03_0225913C[2];

enum MartTypes {
    MART_TYPE_NORMAL = 0,
    MART_TYPE_1, // MART_TYPE_DECOR? Would be unused in HGSS if true. Interesting that it was reserved instead of replaced.
    MART_TYPE_2, // MART_TYPE_SEAL?
    MART_TYPE_3, // MART_TYPE_FRONTIER?
    MART_TYPE_4, // MART_TYPE_POKEATHLON?
};

typedef struct MartData {
    BgConfig *bgConfig;
    u32 unk4;
    Window windows[6];
    u8 unk68[0x20];
    MsgData *messageData;
    MessageFormat *messageFormat;
    Camera *camera;
    SpriteList *spriteList;
    u8 unk98[0x12C];
    GF_2DGfxResMan *gfxResManagers[2];
    u8 unk1CC[0x2C];
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
    u32 unk29C;
    u32 unk2A0;
} MartData;

void ov03_02253E20(s32 strno, BOOL arg1);
void ov03_02253ED0(ListMenuTemplate listMenuTemplate, u8 x, u8 y, u8 width, u8 height, u16 baseTile);
void ov03_02253F74(FieldSystem *fieldSystem);
void Unk03_Delete();
void ov03_022540E8(UnkStruct_ov03 *unk_ov03);
void ov03_02254120(ListMenu *list, enum ListMenuAttr attr_unused, s32 unk2_unused);
void ov03_022542C8(UnkStruct_ov03* unk_ov03);
void ov03_022543AC(UnkStruct_ov03* unk_ov03_unused);
void ov03_02254420();
void ov03_02254B2C(UnkStruct_ov03 *unk_ov03);
void ov03_02254B44();
void ov03_02254B4C(void *);
void ov03_02254BEC();
void ov03_02254C9C(UnkStruct_ov03 *unk_ov03_unused);
void ov03_02254D64(SysTask *task, void *data);
void ov03_02254D78(s32 arg0_unused,  UnkStruct_ov03 *unk_ov03);
void ov03_02254E70(s32 arg0, void* arg1);
void ov03_02255078(void *arg0, UnkStruct_ov03 *unk_ov03);
void ov03_02255280(void *arg0, UnkStruct_ov03 *unk_ov03);
void ov03_0225530C(void *arg0_unused, UnkStruct_ov03 *unk_ov03);
void ov03_022554E0(void *arg0, UnkStruct_ov03 *unk_ov03);
void ov03_02254B58(UnkStruct_ov03 *unk_ov03);
void ov03_0225574C(void *arg0, UnkStruct_ov03 *unk_ov03);
void ov03_022557CC(UnkStruct_ov03 *unk_ov03);
int ov03_02255B70();
int ov03_02255B84();
int ov03_02255B98();
void ov03_02255BB0(FieldSystem *fieldSystem, u32 arg1, u16 arg2, u16 arg3);
u32 ov03_02255BFC();
void ov03_02255C18(FieldSystem *fieldSystem, u32 arg1, u16 arg2, u16 arg3);
u32 ov03_02255C64();
BOOL ov03_02255C80();
void ov03_02255C84(UnkStruct_ov03 *unk_ov03);
BOOL ov03_02255CA0(UnkStruct_ov03 *unk_ov03);
BOOL ov03_02255CD0(UnkStruct_ov03 *unk_ov03);
void ov03_02255CE4(UnkStruct_ov03 *unk_ov03);

void BattleRegulationMenu_GetRegulationName(BattleRegulationMenu *data, int index);
void BattleRegulationMenu_ShowListMenuRegulations(BattleRegulationMenu *menu);
void BattleRegulationMenu_RemoveListMenuRegulations(BattleRegulationMenu *data);
void BattleRegulationMenu_PrintMessage(BattleRegulationMenu *data, int entryID);
int BattleRegulationMenu_ProcessListMenuInputConfirm(BattleRegulationMenu *data);
BOOL BattleRegulationMenu_HandleValidationResult(BattleRegulationMenu *menu);
int BattleRegulationMenu_ProcessListMenuInputRegulations(BattleRegulationMenu *data);
void BattleRegulationMenu_RemoveMsgBox(BattleRegulationMenu *data, BOOL copyToVram);
void BattleRegulationMenu_RemoveRulesWindow(BattleRegulationMenu *menu);
void BattleRegulationMenu_ShowListMenuConfirm(BattleRegulationMenu *menu);
void BattleRegulationMenu_ShowRules(BattleRegulationMenu *menu);
BOOL Task_BattleRegulationMenu(TaskManager *taskManager);
BattleRegulationMenu *BattleRegulationMenu_New(FieldSystem *fieldSystem);
void StartTask_BattleRegulationMenu(TaskManager *taskManager, u16 *result);
void ov03_022566D0(FieldSystem *fieldSystem, MessageFormat *messageFormat, u32 ruleset);
void ov03_02256710(FieldSystem *fieldSystem, u16 a1);
void ov03_02256730(FieldSystem *fieldSystem, Window *window, u32 ruleset);

u16 ov03_02256A2C(FieldSystem *fieldSystem, MessageFormat *msgFmt, u32 a2);
u32 ov03_02256B40(int);
void ov03_02256BA8(FieldSystem *fieldSystem, u8 index);
int ov03_02257814(MartData *data, u32 unkAmount);

void ov03_02258910(FieldSystem *fieldSystem);
void ov03_02258CFC(TaskManager *taskManager, enum PokeathlonData data);\

void sub_020351DC(u16 cursorPos, PlayerProfile *profile);
BOOL sub_02037700();

#endif // POKEHEARTGOLD_OVERLAY_03_H
