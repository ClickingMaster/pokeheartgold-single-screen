#include "global.h"

extern int BattlePartyMenu_GetSlotType(u8 *work, int index);
extern void BattlePartyMenu_DrawButton(u8 *work, int index, int mode, int flag);
extern void BattlePartyMenu_ScrollButtonWindows(u8 *work, int index, int mode);
extern void BattlePartyMenu_OffsetButtonSprites(u8 *work, int index, int mode);
extern u8 BattlePartyMenu_HasMultipleNonEggMons(void *work);
void BattlePartyMenu_UpdateButtonAnim(u8 *work);
void BattlePartyMenu_DrawPageButtons(u8 *work, unsigned int state);

typedef struct B20State {
    u8 pad[0x1FA0];
    u8 state;
    u8 pad1;
    u8 index;
    u8 low:4;
    u8 mode:3;
    u8 active:1;
} B20State;
void BattlePartyMenu_UpdateButtonAnim(u8 *work) {
    B20State *view = (B20State *)work;
    if (view->active) {
        switch (view->state) {
        case 0:
            BattlePartyMenu_DrawButton(work, view->index, 1, view->mode);
            BattlePartyMenu_ScrollButtonWindows(work, view->index, 1);
            BattlePartyMenu_OffsetButtonSprites(work, view->index, 1);
            work[0x1FA1] = 0;
            work[0x1FA0] = 1;
            break;
        case 1:
            BattlePartyMenu_DrawButton(work, view->index, 2, view->mode);
            BattlePartyMenu_ScrollButtonWindows(work, view->index, 2);
            BattlePartyMenu_OffsetButtonSprites(work, view->index, 2);
            work[0x1FA1] = 0;
            work[0x1FA0] = 2;
            break;
        case 2:
            BattlePartyMenu_DrawButton(work, view->index, 0, view->mode);
            BattlePartyMenu_ScrollButtonWindows(work, view->index, 0);
            BattlePartyMenu_OffsetButtonSprites(work, view->index, 0);
            work[0x1FA1] = 0;
            view->active = 0;
            break;
        }
    }
}

typedef struct B20Entry {
    u8 pad[0x1B];
    u8 flags:7;
    u8 disabled:1;
    u8 pad1[0x50 - 0x1C];
} B20Entry;
typedef struct B20Tail {
    u8 pad[0x2077];
    u8 low:4;
    u8 high:4;
} B20Tail;
void BattlePartyMenu_DrawPageButtons(u8 *work, unsigned int state) {
    u16 i;
    switch (state) {
    case 0:
        for (i = 0; i < 6; i++) {
            int value = BattlePartyMenu_GetSlotType(work, i);
            if (value == 0) {
                BattlePartyMenu_DrawButton(work, (u8)i, 3, 1);
            } else if (value == 1) {
                BattlePartyMenu_DrawButton(work, (u8)i, 0, 0);
            } else if (value == 2) {
                BattlePartyMenu_DrawButton(work, (u8)i, 0, 1);
            }
        }
        if ((*(u8 **)work)[0x35] == 1) {
            BattlePartyMenu_DrawButton(work, 6, 3, 0);
        } else {
            BattlePartyMenu_DrawButton(work, 6, 0, 0);
        }
        break;
    case 1:
        BattlePartyMenu_DrawButton(work, 6, 0, 0);
        BattlePartyMenu_DrawButton(work, 7, 0, 0);
        if (((B20Entry *)work)[(*(u8 **)work)[0x11]].disabled) {
            BattlePartyMenu_DrawButton(work, 8, 3, 0);
            BattlePartyMenu_DrawButton(work, 10, 3, 0);
        } else {
            BattlePartyMenu_DrawButton(work, 8, 0, 0);
            BattlePartyMenu_DrawButton(work, 10, 0, 0);
        }
        break;
    case 2:
        if (BattlePartyMenu_HasMultipleNonEggMons(work) == 1) {
            BattlePartyMenu_DrawButton(work, 12, 0, 0);
            BattlePartyMenu_DrawButton(work, 13, 0, 0);
        } else {
            BattlePartyMenu_DrawButton(work, 12, 3, 0);
            BattlePartyMenu_DrawButton(work, 13, 3, 0);
        }
        BattlePartyMenu_DrawButton(work, 11, 0, 0);
        BattlePartyMenu_DrawButton(work, 6, 0, 0);
        break;
    case 3:
        if (BattlePartyMenu_HasMultipleNonEggMons(work) == 1) {
            BattlePartyMenu_DrawButton(work, 12, 0, 0);
            BattlePartyMenu_DrawButton(work, 13, 0, 0);
        } else {
            BattlePartyMenu_DrawButton(work, 12, 3, 0);
            BattlePartyMenu_DrawButton(work, 13, 3, 0);
        }
        for (i = 0; i < 4; i++) {
            if (*(u16 *)(work + (*(u8 **)work)[0x11] * 0x50 + i * 8 + 0x34) != 0) {
                BattlePartyMenu_DrawButton(work, (u8)(i + 14), 0, 0);
            } else {
                BattlePartyMenu_DrawButton(work, (u8)(i + 14), 3, 0);
            }
        }
        BattlePartyMenu_DrawButton(work, 9, 0, 0);
        BattlePartyMenu_DrawButton(work, 6, 0, 0);
        break;
    case 4:
        BattlePartyMenu_DrawButton(work, 6, 0, 0);
        for (i = 0; i < 4; i++) {
            if ((*(u8 **)work)[0x34] == i) {
                BattlePartyMenu_DrawButton(work, (u8)(i + 30), 2, 0);
            } else {
                BattlePartyMenu_DrawButton(work, (u8)(i + 30), 0, 0);
            }
        }
        break;
    case 5:
        for (i = 0; i < 4; i++) {
            if (*(u16 *)(work + (*(u8 **)work)[0x11] * 0x50 + i * 8 + 0x34) != 0) {
                BattlePartyMenu_DrawButton(work, (u8)(i + 19), 0, 0);
            } else {
                BattlePartyMenu_DrawButton(work, (u8)(i + 19), 3, 0);
            }
        }
        BattlePartyMenu_DrawButton(work, 6, 0, 0);
        break;
    case 6:
    case 8:
        BattlePartyMenu_DrawButton(work, 23, 0, 0);
        BattlePartyMenu_DrawButton(work, 24, 0, 0);
        BattlePartyMenu_DrawButton(work, 25, 0, 0);
        BattlePartyMenu_DrawButton(work, 26, 0, 0);
        BattlePartyMenu_DrawButton(work, 27, 0, 0);
        BattlePartyMenu_DrawButton(work, 6, 0, 0);
        if (((B20Tail *)work)->high == 1) {
            BattlePartyMenu_DrawButton(work, 18, 0, 0);
        }
        break;
    case 7:
        BattlePartyMenu_DrawButton(work, 28, 0, 0);
        BattlePartyMenu_DrawButton(work, 6, 0, 0);
        if (((B20Tail *)work)->high == 1) {
            BattlePartyMenu_DrawButton(work, 18, 0, 0);
        }
        break;
    case 9:
        BattlePartyMenu_DrawButton(work, 29, 0, 0);
        BattlePartyMenu_DrawButton(work, 6, 0, 0);
        if (((B20Tail *)work)->high == 1) {
            BattlePartyMenu_DrawButton(work, 18, 0, 0);
        }
        break;
    }
}
