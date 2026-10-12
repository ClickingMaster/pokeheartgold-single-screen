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
struct MessagePrintOptions {
    u8 textFrameDelay;
    u8 autoScroll;
    u8 fontId;
};

struct ScriptMenu;

void FieldSystem_DrawMapNameAnimation(FieldSystem *fieldSystem);
u32 GetHoneySweetScentWorkSize(void);
BOOL Task_HoneyOrSweetScent(TaskManager *taskManager);
BOOL Task_OverworldFish(TaskManager *taskManager);
struct FishingRodWork *CreateFishingRodTaskEnv(FieldSystem *fieldSystem, enum HeapID heapID, int rod);
void FieldSystem_TryStorePlayerLocation(FieldSystem *fieldSystem);
void FieldSystem_SwitchBottomScreenApp(FieldSystem *fieldSystem, int a1, void *a2);
int FieldSystem_GetBottomScreenAppId(FieldSystem *fieldSystem);
int FieldSystem_GetTouchscreenChoiceMenuState(FieldSystem *fieldSystem);
BOOL FieldSystem_GetBottomScreenAppState(FieldSystem *fieldSystem);
UnkStruct_0206793C *FollowMonBall_Spawn(LocalMapObject *obj, int a1);
void FollowMon_MoveToPlayer(FieldSystem *fieldSystem, u8 a1);
void FollowMon_SpawnReleaseBall(LocalMapObject *object);
BOOL FollowMon_CheckVisibleFlag(FieldSystem *fieldSystem);
BOOL FollowMon_StartRecall(FieldSystem *fieldSystem);
BOOL Field_PlayerCanSurfOnTile(PlayerAvatar *fieldPlayerAvatar, int standingTile, int facingTile);
BOOL MetatileBehavior_IsRockClimbInDirection(int facingTile, int facingDirection);
BOOL FieldSystem_FacingModelIsHeadbuttTree(FieldSystem *fieldSystem);
BOOL Task_UseSweetScentInField(TaskManager *taskManager);
BOOL ScrNative_WaitTextPrinter(ScriptContext *ctx);
void ovFieldMain_InitMessagePrintOptions(struct MessagePrintOptions *, ScriptContext *ctx);
void ovFieldMain_ShowMessage(ScriptContext *ctx, MsgData *msg_data, u16 msg_no, u8 can_ab_speed_up, struct MessagePrintOptions *);
struct ScriptMenu *ScriptMenu_Create(FieldSystem *fieldSystem, u8 x, u8 y, u8 initCursorPos, u8 cancellable, u16 *ret_p, MessageFormat *msgFmt, Window *window, MsgData *msgData);
struct ScriptMenu **FieldSystem_GetTouchscreenChoiceMenuList(FieldSystem *fieldSystem);
void ScriptMenu_AddItem(struct ScriptMenu *menu, u32 msgId, u32 value);
void ScriptMenu_ShowMultichoice(struct ScriptMenu *menu);
void ScriptMenu_Free(struct ScriptMenu *menu);
void ScriptMenu_CloseMultichoice(struct ScriptMenu *menu);
void ScriptMenu_ShowListWithWidth(struct ScriptMenu *menu, u16 a1);
void ScriptMenu_ShowListAtPosition(struct ScriptMenu *menu, u16 *a1, u16 *a2);
void ScriptMenu_ShowList(struct ScriptMenu *menu);
void ScriptMenu_ShowMultichoiceGrid(struct ScriptMenu *menu, u8 a1);
void ScriptMenu_SetAnchorRight(struct ScriptMenu *menu, int set);
void ScriptMenu_SetAnchorBottom(struct ScriptMenu *menu, int set);
void ScriptMenu_AddListItem(struct ScriptMenu *menu, int a1, int a2, int a3);
void FollowMonDraw_ClearBob(LocalMapObject *object);
void MapObject_SetFacingAndUpdateDraw(LocalMapObject *object, u32 dir);
u8 FollowMonObject_IsLarge(LocalMapObject *object);
void MapObject_GetCoordsBehind(LocalMapObject *object, int *p_x, int *p_y);
void TallGrass_SpawnAt(LocalMapObject *object, int a1, int x, int y, int a4);
void VeryTallGrass_SpawnAt(LocalMapObject *object, int a1, int x, int y, int a4);
void FieldSystem_StartPalPadRegistration(FieldSystem *fieldSystem);
void CallFieldTask_RockClimb(TaskManager *taskManager, int playerDirection, int partySlot);
void CallFieldTask_Surf(TaskManager *taskManager, int playerDirection, int partySlot);
void CallFieldTask_Waterfall(TaskManager *taskManager, int playerDirection, int partySlot);
void CallFieldTask_Whirlpool(TaskManager *taskManager, int playerDirection, int partySlot);
int MapNumToFloorNo(int mapId);
void PrintCurFloorInNewWindow(FieldSystem *fieldSystem, u8 x, u8 y, u16 *p_ret, MessageFormat *msgFmt, u16 floor);
int Field_SaveGameNormal(FieldSystem *fieldSystem);
void DoorAnimation_Load(FieldSystem *fieldSystem, int x, int z, u8 tag);
void DoorAnimation_WaitForEnd(FieldSystem *fieldSystem, u8 tag);
void DoorAnimation_Unload(FieldSystem *fieldSystem, u8 tag);
void DoorAnimation_PlayOpen(FieldSystem *fieldSystem, u8 tag);
void DoorAnimation_PlayClose(FieldSystem *fieldSystem, u8 tag);
SysTask *Field_PlayerMovementSavingSet(FieldSystem *fieldSystem);
void Field_PlayerMovementSavingClear(SysTask *task);

struct SaveStatsPrinter;

struct SaveStatsPrinter *Field_SaveStatsPrinter_New(FieldSystem *fieldSystem, enum HeapID x, int y);
void Field_SaveStatsPrinter_Print(struct SaveStatsPrinter *printer);
void Field_SaveStatsPrinter_RemoveFromScreen(struct SaveStatsPrinter *printer);
void Field_SaveStatsPrinter_Delete(struct SaveStatsPrinter *printer);
void FieldSystem_ShowEmoteBubble(FieldSystem *fieldSystem, LocalMapObject *partnerPokeObj, int a2);
void FollowMon_StartHopIntoBall(FieldSystem *fieldSystem);
void MapObject_SetPositionNextTo(LocalMapObject *playerObj, LocalMapObject *tsurePokeObj, int a2, int a3);
void FieldSystem_SetTouchscreenChoiceMenuState(FieldSystem *fieldSystem, int a1, int a2, void *a3);

typedef BOOL (*UnkCallback_021F6B34)(struct ScriptMenu *menu, int idx);

void FieldSystem_SetTouchscreenChoiceMenuCallback(FieldSystem *fieldSystem, UnkCallback_021F6B34 cb, struct ScriptMenu *menu);
void ScriptMenu_PrintItemHelpMessage(struct ScriptMenu *menu, int idx);

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

Window *BattleRegulationWindow_New(FieldSystem *fieldSystem, u16 a1);
void BattleRegulationWindow_Delete(Window *window);
BOOL FollowMon_IsNextToPlayer(FieldSystem *fieldSystem);

void Field_SaveStatsPrinter_PrintTouchSave(struct SaveStatsPrinter *printer);
void Field_SaveStatsPrinter_RemoveTouchSave(struct SaveStatsPrinter *printer);
struct SaveStatsPrinter *Field_SaveStatsPrinter_NewTouchSave(FieldSystem *fieldSystem, enum HeapID heapID, u8 bgId);
void Field_SaveStatsPrinter_DeleteTouchSave(struct SaveStatsPrinter *printer);

#endif // POKEHEARTGOLD_OVERLAY_01_H
