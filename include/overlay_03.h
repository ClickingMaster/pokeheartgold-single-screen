#ifndef POKEHEARTGOLD_OVERLAY_03_H
#define POKEHEARTGOLD_OVERLAY_03_H

#include "list_menu.h"
#include "script.h"

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

extern u32 ov03_02259130;
extern u32 ov03_0225913C[2];

extern u32 ov03_022554E0;

void ov03_02253E20(s32 strno, BOOL arg1);
void ov03_02253ED0(ListMenuTemplate listMenuTemplate, u8 x, u8 y, u8 width, u8 height, u16 baseTile);
void ov03_02254120(ListMenu *list, enum ListMenuAttr attr_unused, s32 unk2_unused);
void ov03_022543AC(UnkStruct_ov03* unk_ov03_unused);
void ov03_02254420();
void ov03_02254B44();
void ov03_02254B4C(void *);
void ov03_02254BEC();
void ov03_02254C9C(UnkStruct_ov03 *unk_ov03_unused);
void ov03_02254D64(SysTask *task, void *);
void ov03_02254D78(s32 arg0_unused,  UnkStruct_ov03 *unk_ov03);
void ov03_02254E70(s32 arg0, void* arg1);
void ov03_02255078(void *arg0, UnkStruct_ov03 *unk_ov03);
void ov03_02255280(void *arg0, UnkStruct_ov03 *unk_ov03);
void ov03_0225530C(void *arg0_unused, UnkStruct_ov03 *unk_ov03);
void ov03_0225574C(void *arg0, UnkStruct_ov03 *unk_ov03);
void ov03_022557CC(UnkStruct_ov03 *unk_ov03);
s32 ov03_02255B70();
s32 ov03_02255B84();
u32 ov03_02255B98();
BOOL ov03_02255C80();
void ov03_02255C84(UnkStruct_ov03 *unk_ov03);
int ov03_02255CA0(UnkStruct_ov03 *unk_ov03);
BOOL ov03_02255CD0(UnkStruct_ov03 *unk_ov03);
void ov03_02255CE4(UnkStruct_ov03 *unk_ov03);

void ov03_02255BB0(FieldSystem *fieldSystem, u16 a1, u16 a2, u16 a3);
int ov03_02255BFC(void);
void ov03_02255C18(FieldSystem *fieldSystem, u16 a1, u16 a2, u16 a3);
int ov03_02255C64(void);
void ov03_022566B0(TaskManager *taskManager, u16 *p_var);
void ov03_022566D0(FieldSystem *fieldSystem, MessageFormat *msgFmt, u16 ruleset);
u16 ov03_02256A2C(FieldSystem *fieldSystem, MessageFormat *msgFmt, u16 a2);
void ov03_02256710(FieldSystem *fieldSystem, u16 a1);
void ov03_02258910(FieldSystem *fieldSystem);
void ov03_02258CFC(TaskManager *taskManager, enum PokeathlonData data);
u32 ov03_02256B40(int);

void sub_020351DC(u16 cursorPos, PlayerProfile *profile);
BOOL sub_02037700();

#endif // POKEHEARTGOLD_OVERLAY_03_H
