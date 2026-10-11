#include "global.h"
extern void *BattleDpadCursor_New(u32);
extern void BattleBagMenu_InitBgs(void *);
extern void BattleBagMenu_LoadBgGraphics(void *);
extern void BattleBagMenu_InitText(void *);
extern void FontID_Alloc(int, u32);
extern void *BattleSystem_GetBagCursor(void *);
extern int BagCursor_Battle_GetPocket(void *);
extern void BattleBagMenu_SortItemsIntoPockets(void *);
extern void BattleBagMenu_DrawPageButtons(void *, int);
extern void BattleBagMenu_AddWindows(void *);
extern void BattleBagMenu_PrintPageText(void *, int);
extern void BattleBagMenu_InitSprites(void *);
extern void BattleBagMenu_ShowPageSprites(void *, int);
extern void BattleDpadCursor_SetActive(void *, int);
extern void BattleBagMenu_SetPageCursor(void *, int);
extern void BattleBagMenu_UpdateTutorialFinger(void *, int);
extern u8 PaletteData_BeginPaletteFade(void *, u16, u16, s8, u8, u8, u16);
int BattleBagMenu_Setup(u8 *work);
int BattleBagMenu_Setup(u8 *work) {
    *(volatile u16 *)0x04001050 = 0;
    *(void **)(work + 0x34) = BattleDpadCursor_New(*(u32 *)(*(u8 **)work + 0xC));
    BattleBagMenu_InitBgs(work);
    BattleBagMenu_LoadBgGraphics(work);
    BattleBagMenu_InitText(work);
    FontID_Alloc(4, *(u32 *)(*(u8 **)work + 0xC));
    work[0x114D] = BagCursor_Battle_GetPocket(BattleSystem_GetBagCursor(*(void **)(*(u8 **)work)));
    BattleBagMenu_SortItemsIntoPockets(work);
    BattleBagMenu_DrawPageButtons(work, work[0x114C]);
    BattleBagMenu_AddWindows(work);
    BattleBagMenu_PrintPageText(work, work[0x114C]);
    BattleBagMenu_InitSprites(work);
    BattleBagMenu_ShowPageSprites(work, work[0x114C]);
    if ((*(u8 **)work)[0x25]) BattleDpadCursor_SetActive(*(void **)(work + 0x34), 1);
    BattleBagMenu_SetPageCursor(work, work[0x114C]);
    BattleBagMenu_UpdateTutorialFinger(work, work[0x114C]);
    PaletteData_BeginPaletteFade(*(void **)(work + 8), 10, 0xFFFF, -8, 16, 0, 0);
    if (*(u32 *)(*(u8 **)work + 0x14) == 1) return 12;
    return 1;
}

extern void *sBattleBagPocketMenuHitboxes;
extern int PaletteData_GetSelectedBuffersBitmask(void *);
extern int BattleBagMenu_CheckTouchHitbox(void *, void *);
extern int BattleDpadCursor_HandleInput(void *);
extern void BattleBagMenu_DeactivateCursor(void *);
extern void PlaySE(int);
extern void BattleBagMenu_StartButtonAnim(void *, int, int);
extern void BattleBagMenu_SelectLastUsedItem(void *);
int BattleBagMenu_HandlePocketMenuInput(u8 *work);
int BattleBagMenu_HandlePocketMenuInput(u8 *work) {
    int choice;
    u8 slot;
    if (PaletteData_GetSelectedBuffersBitmask(*(void **)(work + 8))) return 1;
    choice = BattleBagMenu_CheckTouchHitbox(work, &sBattleBagPocketMenuHitboxes);
    if (choice == -1) {
        choice = BattleDpadCursor_HandleInput(*(void **)(work + 0x34));
        if (choice == -2) choice = 5;
    } else {
        BattleBagMenu_DeactivateCursor(work);
    }
    switch (choice) {
    case 0: case 1: case 2: case 3:
        PlaySE(0x5DD);
        slot = choice;
        work[0x114D] = slot;
        work[0x114B] = 5;
        BattleBagMenu_StartButtonAnim(work, slot, 0);
        return 11;
    case 4:
        if (!*(u16 *)(*(u8 **)work + 0x20)) break;
        PlaySE(0x5DD);
        work[0x114D] = (*(u8 **)work)[0x1F];
        work[0x114B] = 6;
        BattleBagMenu_SelectLastUsedItem(work);
        BattleBagMenu_StartButtonAnim(work, 4, 0);
        return 11;
    case 5:
        PlaySE(0x5DD);
        *(u16 *)(*(u8 **)work + 0x1C) = 0;
        (*(u8 **)work)[0x1E] = 4;
        BattleBagMenu_StartButtonAnim(work, 5, 0);
        return 13;
    }
    return 1;
}
