#pragma force_active on
#include "global.h"
#include "touchscreen.h"
typedef struct Menu67 {
    u8 unk000[0x49C];
    u32 state;
    u16 page;
    u16 unk4A2;
    void *grid;
    u32 selection;
} Menu67;
int GridInputHandler_HandleInput_NoHold(void *grid);
int GridInputHandler_GetNextInput(void *grid);
int System_GetTouchNew(void);
extern TouchscreenHitbox ov67_021E6D50[];
void ov67_021E6BB8(Menu67 *menu);
void ov67_021E6B6C(Menu67 *menu, int mode);
void ov67_021E6490(Menu67 *menu);
void ov67_021E6688(Menu67 *menu, int index);
void ov67_021E6530(Menu67 *menu);
void PlaySE(int seqNo);
int ov67_021E5C44(Menu67 *menu);
// @fn ov67_021E5C44
int ov67_021E5C44(Menu67 *menu) {
    switch ((u32)GridInputHandler_HandleInput_NoHold(menu->grid)) {
    case 12:
    case -2:
        PlaySE(1501);
        ov67_021E6BB8(menu);
        menu->state = 6;
        return 5;
    case -3:
        PlaySE(1500);
        if (menu->selection == 1) {
            ov67_021E6B6C(menu, 2);
            menu->state = 4;
            return 5;
        } else if (menu->selection == 2) {
            ov67_021E6B6C(menu, 3);
            menu->state = 4;
            return 5;
        }
        break;
    case -4:
    case -1:
        break;
    default:
        if (System_GetTouchNew() == 1) {
            PlaySE(1500);
        }
        break;
    }
    switch (TouchscreenHitbox_FindRectAtTouchNew(ov67_021E6D50)) {
    case 0:
        if (menu->page) {
            menu->page--;
        } else {
            menu->page = 2;
        }
        PlaySE(1504);
        ov67_021E6490(menu);
        ov67_021E6688(menu, menu->page * 12 + GridInputHandler_GetNextInput(menu->grid));
        ov67_021E6530(menu);
        ov67_021E6B6C(menu, 2);
        menu->state = 4;
        return 5;
    case 1:
        if (menu->page < 2) {
            menu->page++;
        } else {
            menu->page = 0;
        }
        PlaySE(1504);
        ov67_021E6490(menu);
        ov67_021E6688(menu, menu->page * 12 + GridInputHandler_GetNextInput(menu->grid));
        ov67_021E6530(menu);
        ov67_021E6B6C(menu, 3);
        menu->state = 4;
        return 5;
    }
    return 4;
}
