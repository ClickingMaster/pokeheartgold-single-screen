#include "global.h"
#include <string.h>
extern void *CreateSysTaskAndEnvironment(void (*)(void *, void *), u32, u32, u32);
extern void *SysTask_GetData(void *);
extern void *BattleSystem_GetBgConfig(void *);
extern void *BattleSystem_GetPaletteData(void *);
extern void *BattleSystem_GetBagCursor(void *);
extern void BagCursor_Battle_PocketGetPosition(void *, u32, u8 *, u8 *);
extern u16 BagCursor_Battle_GetLastUsedItem(void *);
extern u8 BagCursor_Battle_GetLastUsedPocket(void *);
extern void BattleBagMenu_ValidateLastUsedItem(void *);
extern u32 BattleSystem_GetBattleType(void *);
extern void BattleBagMenu_Task(void *, u8 *);
void BattleBagMenu_Create(u8 *state);
void BattleBagMenu_Create(u8 *state) {
    u8 *work;
    void *cursor;
    u8 i;
    work = SysTask_GetData(CreateSysTaskAndEnvironment((void (*)(void *, void *))BattleBagMenu_Task, 0x115C, 100, *(u32 *)(state + 0xC)));
    memset(work, 0, 0x115C);
    *(u8 **)work = state;
    *(void **)(work + 4) = BattleSystem_GetBgConfig(*(void **)state);
    *(void **)(work + 8) = BattleSystem_GetPaletteData(*(void **)state);
    work[0x114A] = 0;
    cursor = BattleSystem_GetBagCursor(*(void **)state);
    for (i = 0; i < 5; i++) {
        BagCursor_Battle_PocketGetPosition(cursor, i, *(u8 **)work + 0x27 + i, *(u8 **)work + 0x2C + i);
    }
    *(u16 *)(*(u8 **)work + 0x20) = BagCursor_Battle_GetLastUsedItem(cursor);
    (*(u8 **)work)[0x1F] = BagCursor_Battle_GetLastUsedPocket(cursor);
    BattleBagMenu_ValidateLastUsedItem(work);
    if (BattleSystem_GetBattleType(*(void **)(*(u8 **)work)) & 0x400) {
        *(u32 *)(*(u8 **)work + 0x14) = 1;
    }
}
#include "global.h"
extern int BattleBagMenu_Setup(void *);
extern int BattleBagMenu_HandlePocketMenuInput(void *);
extern int BattleBagMenu_HandleItemListInput(void *);
extern int BattleBagMenu_HandleUseItemInput(void *);
extern int BattleBagMenu_ShowPocketMenuPage(void *);
extern int BattleBagMenu_ShowItemListPage(void *);
extern int BattleBagMenu_ShowUseItemPage(void *);
extern int BattleBagMenu_ChangeItemPage(void *);
extern int BattleBagMenu_CloseMessage(void *);
extern int BattleBagMenu_WaitMessage(void *);
extern int BattleBagMenu_WaitMessageInput(void *);
extern int BattleBagMenu_WaitButtonAnim(void *);
extern int BattleBagMenu_RunTutorial(void *);
extern int BattleBagMenu_StartFadeOut(void *);
extern int BattleBagMenu_Exit(void *, void *);
extern void BattleBagMenu_UpdateButtonAnim(void *);
extern void SpriteSystem_DrawSprites(void *);
void BattleBagMenu_Task(void *task, u8 *work);
void BattleBagMenu_Task(void *task, u8 *work) {
    int state = work[0x114A];
    switch (state) {
    case 0: work[0x114A] = BattleBagMenu_Setup(work); break;
    case 1: work[0x114A] = BattleBagMenu_HandlePocketMenuInput(work); break;
    case 2: work[0x114A] = BattleBagMenu_HandleItemListInput(work); break;
    case 3: work[0x114A] = BattleBagMenu_HandleUseItemInput(work); break;
    case 4: work[0x114A] = BattleBagMenu_ShowPocketMenuPage(work); break;
    case 5: work[0x114A] = BattleBagMenu_ShowItemListPage(work); break;
    case 6: work[0x114A] = BattleBagMenu_ShowUseItemPage(work); break;
    case 7: work[0x114A] = BattleBagMenu_ChangeItemPage(work); break;
    case 8: work[0x114A] = BattleBagMenu_CloseMessage(work); break;
    case 9: work[0x114A] = BattleBagMenu_WaitMessage(work); break;
    case 10: work[0x114A] = BattleBagMenu_WaitMessageInput(work); break;
    case 11: work[0x114A] = BattleBagMenu_WaitButtonAnim(work); break;
    case 12: work[0x114A] = BattleBagMenu_RunTutorial(work); break;
    case 13: work[0x114A] = BattleBagMenu_StartFadeOut(work); break;
    case 14: if (BattleBagMenu_Exit(task, work) == 1) return; break;
    }
    BattleBagMenu_UpdateButtonAnim(work);
    SpriteSystem_DrawSprites(*(void **)(work + 0x30C));
}
