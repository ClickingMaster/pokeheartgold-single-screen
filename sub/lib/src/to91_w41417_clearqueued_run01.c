#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
typedef void (*W40852Done)(void *, u32);

typedef struct {
    u16 state, pad, count, reserved;
    volatile u16 *frame;
    u8 *out;
    W40852Done done;
} W40852Slot;

#define W40852Root (*(u8 **)0x0380fff4)
void ClearQueuedPri(u32);

void ClearQueuedPri(u32 pri) {
    W40852Slot *slot = (W40852Slot *)(W40852Root + 0x42c) + pri;
    if (slot->state) {
        u16 value = *slot->frame;
        if (value == 0) {
            *(u16 *)(slot->out + 8) = 2;
        } else {
            *(u16 *)(slot->out + 8) = value;
        }
        slot->done(slot->out, 0);
    }
}
