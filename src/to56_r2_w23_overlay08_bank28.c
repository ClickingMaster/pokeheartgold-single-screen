#include "global.h"

typedef struct W23BattleInner {
    void *battleSystem;
    u8 pad04[0xC];
    int field10;
} W23BattleInner;

typedef struct W23BattleOuter {
    W23BattleInner *inner;
} W23BattleOuter;

typedef struct W23Windows {
    void *owner;
    void *bgConfig;
    u8 pad08[0x14];
    u8 window[0x10];
    void *windows;
    u8 count;
} W23Windows;

typedef u32 W23WindowTemplate[2];

extern const W23WindowTemplate sBattleBagMessageWindowTemplate;
extern const W23WindowTemplate sBattleBagUseItemWindows[];
extern const W23WindowTemplate sBattleBagPocketMenuWindows[];
extern const W23WindowTemplate sBattleBagItemListWindows[];

void *BattleSystem_GetBattleContext(void *);
void ov12_022581D4(void *, void *, int, int);
void *BattleSystem_GetBag(void *);
void Bag_TakeItem(void *, int, int, int);
void *BattleSystem_GetBagCursor(void *);
void BagCursor_Battle_SetLastUsedItem(void *, int, int);
void AddWindow(void *, void *, const void *);
void *AllocWindows(int, int);

void BattleBagMenu_GetBattlerSelectedMonIndex(W23BattleOuter *outer);
void BattleBagMenu_ConsumeItem(void *battleSystem, int item, int pocket, int quantity);
void BattleBagMenu_AddWindows(W23Windows *work);
void BattleBagMenu_AddPageWindows(W23Windows *work, int kind);

void BattleBagMenu_GetBattlerSelectedMonIndex(W23BattleOuter *outer) {
    W23BattleInner *inner = outer->inner;
    void *context = BattleSystem_GetBattleContext(inner->battleSystem);
    ov12_022581D4(outer->inner->battleSystem, context, 2, inner->field10);
}

void BattleBagMenu_ConsumeItem(void *battleSystem, int item, int pocket, int quantity) {
    Bag_TakeItem(BattleSystem_GetBag(battleSystem), item, 1, quantity);
    BagCursor_Battle_SetLastUsedItem(BattleSystem_GetBagCursor(battleSystem), item, pocket);
}

void BattleBagMenu_AddWindows(W23Windows *work) {
    AddWindow(work->bgConfig, work->window, sBattleBagMessageWindowTemplate);
    BattleBagMenu_AddPageWindows(work, *((u8 *)work + 0x114C));
}

void BattleBagMenu_AddPageWindows(W23Windows *work, int kind) {
    const W23WindowTemplate *templates;
    u8 i;
    switch (kind) {
    case 0:
        templates = sBattleBagPocketMenuWindows;
        work->count = 5;
        break;
    case 1:
        templates = sBattleBagItemListWindows;
        work->count = 26;
        break;
    case 2:
        templates = sBattleBagUseItemWindows;
        work->count = 4;
        break;
    }
    work->windows = AllocWindows(*((int *)work->owner + 3), work->count);
    for (i = 0; i < work->count; i++) {
        AddWindow(work->bgConfig, (u8 *)work->windows + i * 16, templates[i]);
    }
}
