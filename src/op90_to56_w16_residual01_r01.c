#pragma force_active on
#include "global.h"

typedef struct Ov13W16State {
    u32 unk00;
    void *(*alloc)(u32 size);
    u8 unk08[0x38];
    void *aligned;
    u8 unk44[0x10];
    void *allocation;
    u8 unk58[4];
    void *workspace;
    u8 unk60[0x10];
    u32 count;
} Ov13W16State;

extern Ov13W16State ov13_0224DF30;
void ov13_02223E44(void);
void ov13_02223EA4(u32 a, int b);
int ov13_02223D24(void *owner, u8 *buffer, int size);
int ov13_02223E6C(void);
void OS_Sleep(u32 msec);
int ov13_02223EE0(u32 count);

static inline u32 Mask(u32 v) {
    s16 m1 = 31;
    return v & (m1 - 63);
}

#pragma push
#pragma opt_foldconstants off
#pragma opt_loop_invariants off
#pragma opt_strength_reduction off
// @fn ov13_02223EE0
int ov13_02223EE0(u32 count)
{
    u16 m = 31;
    s16 k = -32;
    int event;
    int running = TRUE;
    int result;
    u32 size;
    u32 aligned;
    u32 ws;
    ov13_0224DF30.count = count;
    ov13_02223E44();
    size = count * 0xD0;
    ov13_0224DF30.allocation = ov13_0224DF30.alloc(size + 0x24D0 + count * 0xC0);
    if (!ov13_0224DF30.allocation) {
        return -1;
    }
    aligned = Mask((u32)ov13_0224DF30.allocation + m);
    ws = aligned + (size + 0x2490);
    ws += m;
    ws &= k;
    ov13_0224DF30.aligned = (void *)aligned;
    ov13_0224DF30.workspace = (void *)ws;
    if (!ov13_02223D24(ov13_02223EA4, (u8 *)aligned, size + 0x2490)) {
        return -2;
    }
    do {
        OS_Sleep(10);
        event = ov13_02223E6C();
        while (event != 0) {
            if (event != 4 && event != 5) {
                if (event == 6) {
                    running = FALSE;
                    result = 1;
                } else {
                    running = FALSE;
                    result = -2;
                }
            }
            event = ov13_02223E6C();
        }
    } while (running);
    return result;
}
#pragma pop
