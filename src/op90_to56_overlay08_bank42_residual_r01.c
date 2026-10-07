#pragma force_active on
#include "global.h"
#include "system.h"
#include "unk_02020A0C.h"

typedef struct Ov08Menu {
    void *cursor;
    const DpadMenuBox *boxes;
    u8 unk8;
    u8 cur;
    u8 prev;
    u32 enabled;
} Ov08Menu;

BOOL ov08_02224BFC(Ov08Menu *menu);
BOOL ov08_02224C48(const DpadMenuBox *box, u32 direction);
void ov12_0226BAFC(void *cursor, u8 left, u8 right, u8 top, u8 bottom);
void PlaySE(u16 se);
int ov08_02224C94(Ov08Menu *menu);

// @fn ov08_02224C94
int ov08_02224C94(Ov08Menu *menu) {
    u8 input;
    u8 direction;
    u8 neighborIsEnabled;
    u8 r1;
    u8 x, y, w, h;

    if (ov08_02224BFC(menu) == FALSE) {
        return -1;
    }
    if (gSystem.newKeys & PAD_KEY_UP) {
        input = DpadMenuBox_GetNeighborInDirection(menu->boxes, NULL, NULL, NULL, NULL, menu->cur, 0);
        direction = 0;
    } else if (gSystem.newKeys & PAD_KEY_DOWN) {
        input = DpadMenuBox_GetNeighborInDirection(menu->boxes, NULL, NULL, NULL, NULL, menu->cur, 1);
        direction = 1;
    } else if (gSystem.newKeys & PAD_KEY_LEFT) {
        input = DpadMenuBox_GetNeighborInDirection(menu->boxes, NULL, NULL, NULL, NULL, menu->cur, 2);
        direction = 2;
    } else if (gSystem.newKeys & PAD_KEY_RIGHT) {
        input = DpadMenuBox_GetNeighborInDirection(menu->boxes, NULL, NULL, NULL, NULL, menu->cur, 3);
        direction = 3;
    } else {
        input = 0xFF;
    }
    if (input != 0xFF) {
        neighborIsEnabled = TRUE;
        if (input & 0x80) {
            if (menu->prev != 0xFF) {
                input = menu->prev;
            } else {
                input ^= 0x80;
            }
        }
        while (TRUE) {
            if (menu->enabled & (1 << input)) {
                break;
            }
            neighborIsEnabled = FALSE;
            r1 = DpadMenuBox_GetNeighborInDirection(menu->boxes, NULL, NULL, NULL, NULL, input, direction) & 0x7F;
            if (r1 == input || r1 == menu->cur) {
                input = menu->cur;
                break;
            }
            input = r1;
        }
        if (menu->cur != input) {
            DpadMenuBox_GetPosition(&menu->boxes[input], &x, &y);
            DpadMenuBox_GetDimensions(&menu->boxes[input], &w, &h);
            if (ov08_02224C48(&menu->boxes[input], direction) == TRUE && neighborIsEnabled) {
                menu->prev = menu->cur;
            } else {
                menu->prev = 0xFF;
            }
            menu->cur = input;
            ov12_0226BAFC(menu->cursor, x, w, y, h);
            PlaySE(0x5DC);
        }
        return -1;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        return menu->cur;
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(0x5DD);
        return -2;
    }
    return -1;
}
