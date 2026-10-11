#include "global.h"
extern void *sBattleBagUseItemHitboxes;
extern int BattleBagMenu_CheckTouchHitbox(void *, void *);
extern int BattleDpadCursor_HandleInput(void *);
extern void BattleBagMenu_DeactivateCursor(void *);
extern void PlaySE(int);
extern int BattleBagMenu_GetItemOnPage(void *, int);
extern void BattleBagMenu_StartButtonAnim(void *, int, int);
extern int BattleBagMenu_TryUseItem(void *);
int BattleBagMenu_HandleUseItemInput(u8 *work);
int BattleBagMenu_HandleUseItemInput(u8 *work) {
    int choice = BattleBagMenu_CheckTouchHitbox(work, &sBattleBagUseItemHitboxes);
    if (choice == -1) {
        choice = BattleDpadCursor_HandleInput(*(void **)(work + 0x34));
        if (choice == -2) choice = 1;
    } else {
        BattleBagMenu_DeactivateCursor(work);
    }
    switch (choice) {
    case 0: {
        PlaySE(0x5DD);
        *(u16 *)(*(u8 **)work + 0x1C) = BattleBagMenu_GetItemOnPage(work, (*(u8 **)work + work[0x114D])[0x27]);
        (*(u8 **)work)[0x1E] = work[0x114D];
        BattleBagMenu_StartButtonAnim(work, 15, 0);
        return BattleBagMenu_TryUseItem(work);
    }
    case 1: {
        PlaySE(0x5DD);
        work[0x114B] = 5;
        BattleBagMenu_StartButtonAnim(work, 16, 0);
        return 11;
    }
    default:
        return 3;
    }
}
