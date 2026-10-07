#pragma force_active on
#include "global.h"
#include "system.h"

typedef struct Ov112Confirm {
    u8 pad0[0x1E530];
    void *cursorSprite;
    u8 pad1E534[0x1EC50 - 0x1E534];
    int selection;
    u8 pad1EC54[0x1F2E2 - 0x1EC54];
    u16 state;
} Ov112Confirm;

u32 ov112_021E9888(int index);
void ov112_021EA51C(Ov112Confirm *work, int selection);
void ov112_021EA584(Ov112Confirm *work);
void PlaySE(u16 se);
BOOL ManagedSprite_IsAnimated(void *sprite);
int ov112_021E9750(Ov112Confirm *work);

// @fn ov112_021E9750
int ov112_021E9750(Ov112Confirm *work) {
    if (work->state == 0) {
        switch (ov112_021E9888(0)) {
        case 0:
            work->selection = 0;
            ov112_021EA51C(work, 0);
            ov112_021EA584(work);
            PlaySE(0x5DD);
            work->state = 1;
            break;
        case 1:
            work->selection = 2;
            ov112_021EA51C(work, 1);
            ov112_021EA584(work);
            PlaySE(0x5DD);
            work->state = 1;
            break;
        case -1:
            if (gSystem.newKeys & PAD_KEY_UP) {
                if (work->selection != 0) {
                    PlaySE(0x5DC);
                }
                ov112_021EA51C(work, 0);
                work->selection = 0;
            } else if (gSystem.newKeys & PAD_KEY_DOWN) {
                if (work->selection != 1) {
                    PlaySE(0x5DC);
                }
                ov112_021EA51C(work, 1);
                work->selection = 1;
            } else if (gSystem.newKeys & PAD_BUTTON_A) {
                work->state = 1;
                ov112_021EA584(work);
                PlaySE(0x5DD);
            } else if (gSystem.newKeys & PAD_BUTTON_B) {
                ov112_021EA51C(work, 1);
                ov112_021EA584(work);
                work->selection = 1;
                work->state = 1;
                PlaySE(0x5DC);
            }
            break;
        }
    } else if (work->state == 1) {
        if (!ManagedSprite_IsAnimated(work->cursorSprite)) {
            work->state = 2;
        }
    } else if (work->state == 2) {
        if (work->selection != 0) {
            return 1;
        }
        return 0;
    }
    return 2;
}
