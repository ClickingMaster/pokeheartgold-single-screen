#pragma force_active on
#include "global.h"
#include "item.h"
#include "constants/items.h"

typedef struct Root {
    u8 pad[8];
    void *bag;
    enum HeapID heapID;
    u8 pad2[0x2C - 0x10];
    u8 row[5];
} Root;

typedef struct Group {
    ItemSlot entries[36];
} Group;

typedef struct Work {
    Root *root;
    u8 pad[0x38];
    Group groups[30];
    u8 pad2[0x114F - 0x3C - 30 * 0x90];
    u8 count[5];
    u8 page[5];
} Work;

extern u8 sBattleBagPocketOrder[];
ItemSlot *Bag_GetPocketSlotN(void *bag, u16 pocket, u16 slot);
void BattleBagMenu_SortItemsIntoPockets(Work *work);

static inline void AddItem(Work *work, ItemSlot *item) {
    u32 i;
    u32 mask = GetItemAttr(item->id, ITEMATTR_BATTLE_POCKET, work->root->heapID);
    for (i = 0; i < 5; i++) {
        if (mask & (1 << i)) {
            work->groups[sBattleBagPocketOrder[i]].entries[work->count[sBattleBagPocketOrder[i]]] = *item;
            work->count[sBattleBagPocketOrder[i]]++;
        }
    }
}

// @fn BattleBagMenu_SortItemsIntoPockets
void BattleBagMenu_SortItemsIntoPockets(Work *work) {
    u32 pocket, slot, i;
    ItemSlot *item;

    for (pocket = 0; pocket < 8; pocket++) {
        for (slot = 0;; slot++) {
            item = Bag_GetPocketSlotN(work->root->bag, pocket, slot);
            if (item == NULL) {
                break;
            }
            if (item->id != 0 && item->quantity != 0) {
                AddItem(work, item);
            }
        }
    }
    for (i = 0; i < 5; i++) {
        if (work->count[i] == 0) {
            work->page[i] = 0;
        } else {
            work->page[i] = (work->count[i] - 1) / 6;
        }
        if (work->page[i] < work->root->row[i]) {
            work->root->row[i] = work->page[i];
        }
    }
}
