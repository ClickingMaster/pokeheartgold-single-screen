#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u16 id;
    u8 pad[3];
    u8 existing : 1, pulled : 1, other : 6;
} W;

extern int CTRDG_IsExisting(void);
int CTRDG_IsPulledOut(void);

int CTRDG_IsPulledOut(void) {
    volatile W *w = (volatile W *)0x27ffc30;
    if (w->id == 65535) {
        return 0;
    }
    if (!w->pulled) {
        CTRDG_IsExisting();
    }
    return w->pulled;
}
