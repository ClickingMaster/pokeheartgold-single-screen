#pragma force_active on
#include "global.h"

int ov45_0222EC68(void *a);
BOOL ov45_0222BD74(void *state, u32 index);
void ov45_0222BD4C(void *state);
void ov45_0222BE54(void *state);
void ov45_0222C73C(void *a, u16 *event, void *c, u8 *work);

// @fn ov45_0222C73C
void ov45_0222C73C(void *a, u16 *event, void *c, u8 *work) {
    int id = ov45_0222EC68(a);
    if (work[0x1CA] != 0 && *(u16 *)(work + 0x1C4) == id) {
        if (*event == 1) {
            if (ov45_0222BD74(work + 0x1C0, id) == FALSE) {
                work[0x1CB] = TRUE;
            } else {
                work[0x1CB] = FALSE;
            }
            work[0x1C6] = 2;
        } else {
            ov45_0222BD4C(work + 0x1C0);
        }
        ov45_0222BE54(work + 0x1C0);
    }
}
