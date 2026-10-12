#ifndef POKEHEARTGOLD_OVERLAY_01_H
#define POKEHEARTGOLD_OVERLAY_01_H

#include "field/map_prop_animation.h"
#include "field/rock_smash_item.h"

#include "field_system.h"
#include "script.h"
#include "unk_0206793C.h"

// todo: split

struct ErrorContinueEnv {
    int state;
    Location location;
};

#define ROD_TYPE_OLD   0
#define ROD_TYPE_GOOD  1
#define ROD_TYPE_SUPER 2
#define ROD_TYPE_NONE  0xFF

struct FishingRodWork;

// TODO: does this struct have more members?
struct UnkStruct_Ov01_021EF4C4 {
    u8 textFrameDelay;
    u8 unk1;
    u8 fontId;
};

struct UnkStruct_ov01_021EDC28;

void FieldSystem_DrawMapNameAnimation(FieldSystem *fieldSystem);
u32 GetHoneySweetScentWorkSize(void);
BOOL Task_HoneyOrSweetScent(TaskManager *taskManager);
BOOL Task_OverworldFish(TaskManager *taskManager);
struct FishingRodWork *CreateFishingRodTaskEnv(FieldSystem *fieldSystem, enum HeapID heapID, int rod);
void FieldSystem_TryStorePlayerLocation(FieldSystem *fieldSystem);
void ov01_021F6A9C(FieldSystem *fieldSystem, int a1, void *a2);
int ov01_021F6B00(FieldSystem *fieldSystem);
int ov01_021F6AEC(FieldSystem *fieldSystem);
BOOL ov01_021F6B10(FieldSystem *fieldSystem);
UnkStruct_0206793C *FollowMonBall_Spawn(LocalMapObject *obj, int a1);
void FollowMon_MoveToPlayer(FieldSystem *fieldSystem, u8 a1);
void FollowMon_SpawnReleaseBall(LocalMapObject *object);
BOOL FollowMon_CheckVisibleFlag(FieldSystem *fieldSystem);
BOOL FollowMon_StartRecall(FieldSystem *fieldSystem);
BOOL Field_PlayerCanSurfOnTile(PlayerAvatar *fieldPlayerAvatar, int standingTile, int facingTile);
BOOL MetatileBehavior_IsRockClimbInDirection(int facingTile, int facingDirection);
BOOL FieldSystem_FacingModelIsHeadbuttTree(FieldSystem *fieldSystem);
BOOL Task_UseSweetScentInField(TaskManager *taskManager);
BOOL ov01_021EF348(ScriptContext *ctx);
void ov01_021EF4C4(struct UnkStruct_Ov01_021EF4C4 *, ScriptContext *ctx);
void ov01_021EF4DC(ScriptContext *ctx, MsgData *msg_data, u16 msg_no, u8 can_ab_speed_up, struct UnkStruct_Ov01_021EF4C4 *);
struct UnkStruct_ov01_021EDC28 *ov01_021EDF78(FieldSystem *fieldSystem, u8 x, u8 y, u8 initCursorPos, u8 cancellable, u16 *ret_p, MessageFormat *msgFmt, Window *window, MsgData *msgData);
struct UnkStruct_ov01_021EDC28 **ov01_021F6B20(FieldSystem *fieldSystem);
void ov01_021EDC7C(struct UnkStruct_ov01_021EDC28 *menu, u32 msgId, u32 value);
void ov01_021EDC84(struct UnkStruct_ov01_021EDC28 *menu);
void ov01_021EDF00(struct UnkStruct_ov01_021EDC28 *menu);
void ov01_021EDF38(struct UnkStruct_ov01_021EDC28 *menu);
void ov01_021EE014(struct UnkStruct_ov01_021EDC28 *menu, u16 a1);
void ov01_021EE0EC(struct UnkStruct_ov01_021EDC28 *menu, u16 *a1, u16 *a2);
void ov01_021EDFA4(struct UnkStruct_ov01_021EDC28 *menu);
void ov01_021EE974(struct UnkStruct_ov01_021EDC28 *menu, u8 a1);
void ov01_021EF018(struct UnkStruct_ov01_021EDC28 *menu, u8 a1);
void ov01_021EF034(struct UnkStruct_ov01_021EDC28 *menu, u8 a1);
void MoveTutorMenu_SetListItem(struct UnkStruct_ov01_021EDC28 *menu, int a1, int a2, int a3);
void ov01_021F7704(LocalMapObject *object);
void ov01_021F9408(LocalMapObject *object, u32 dir);
u8 FollowMonObject_IsLarge(LocalMapObject *object);
void MapObject_GetCoordsBehind(LocalMapObject *object, int *p_x, int *p_y);
void TallGrass_SpawnAt(LocalMapObject *object, int a1, int x, int y, int a4);
void VeryTallGrass_SpawnAt(LocalMapObject *object, int a1, int x, int y, int a4);
void ov01_021F729C(FieldSystem *fieldSystem);
void CallFieldTask_RockClimb(TaskManager *taskManager, int playerDirection, int partySlot);
void CallFieldTask_Surf(TaskManager *taskManager, int playerDirection, int partySlot);
void CallFieldTask_Waterfall(TaskManager *taskManager, int playerDirection, int partySlot);
void CallFieldTask_Whirlpool(TaskManager *taskManager, int playerDirection, int partySlot);
int MapNumToFloorNo(u32 mapId);
void PrintCurFloorInNewWindow(FieldSystem *fieldSystem, u8 x, u8 y, u16 *p_ret, MessageFormat *msgFmt, u16 floor);
int Field_SaveGameNormal(FieldSystem *fieldSystem);
void ov01_021E9AE8(FieldSystem *fieldSystem, int a1, int a2, u8 a3);
void ov01_021E9C00(FieldSystem *fieldSystem, u8 a1);
void ov01_021E9C20(FieldSystem *fieldSystem, u8 a1);
void ov01_021E9BB8(FieldSystem *fieldSystem, u8 a1);
void ov01_021E9BDC(FieldSystem *fieldSystem, u8 a1);
SysTask *Field_PlayerMovementSavingSet(FieldSystem *fieldSystem);
void Field_PlayerMovementSavingClear(SysTask *task);

struct SaveStatsPrinter;

struct SaveStatsPrinter *Field_SaveStatsPrinter_New(FieldSystem *fieldSystem, enum HeapID x, int y);
void Field_SaveStatsPrinter_Print(struct SaveStatsPrinter *printer);
void Field_SaveStatsPrinter_RemoveFromScreen(struct SaveStatsPrinter *printer);
void Field_SaveStatsPrinter_Delete(struct SaveStatsPrinter *printer);
void FieldSystem_ShowEmoteBubble(FieldSystem *fieldSystem, LocalMapObject *partnerPokeObj, int a2);
void ov01_02205AEC(FieldSystem *fieldSystem);
void MapObject_SetPositionNextTo(LocalMapObject *playerObj, LocalMapObject *tsurePokeObj, int a2, int a3);
void ov01_021F6ABC(FieldSystem *fieldSystem, int a1, int a2, void *a3);

typedef BOOL (*UnkCallback_021F6B34)(struct UnkStruct_ov01_021EDC28 *menu, int idx);

void ov01_021F6B34(FieldSystem *fieldSystem, UnkCallback_021F6B34 cb, struct UnkStruct_ov01_021EDC28 *menu);
void ov01_021EEF9C(struct UnkStruct_ov01_021EDC28 *menu, int idx);

struct UnkStruct_020FACDC {
    int filler_00[12 / sizeof(int)];
};

BOOL FollowMon_StartFlash(FieldSystem *fieldSystem, u8 a1, u8 a2);

struct BankTransactionWorkSub {
    u32 max;
    int selected;
};

struct BankTransactionWork {
    struct BankTransactionWorkSub *sub;
    int mode;
};

Window *ov01_021EEF68(FieldSystem *fieldSystem, u16 a1);
void ov01_021EEF88(Window *window);
BOOL FollowMon_IsNextToPlayer(FieldSystem *fieldSystem);

void Field_SaveStatsPrinter_PrintTouchSave(struct SaveStatsPrinter *printer);
void Field_SaveStatsPrinter_RemoveTouchSave(struct SaveStatsPrinter *printer);
struct SaveStatsPrinter *Field_SaveStatsPrinter_NewTouchSave(FieldSystem *fieldSystem, enum HeapID heapID, u8 bgId);
void Field_SaveStatsPrinter_DeleteTouchSave(struct SaveStatsPrinter *printer);

#endif // POKEHEARTGOLD_OVERLAY_01_H
