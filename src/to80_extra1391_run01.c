#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    void *object;
    u32 state;
    u8 pad[40];
    int active;
    u64 start;
} Work;

extern Work *ov00_0221A90C[];
extern int ov00_021EC230(void);
extern void ov00_021EF8C0(void);
extern void ov00_021FFD9C(void *);
extern void ov00_021EF3C0(int, int);
void ov00_021EF2AC(void);

void ov00_021EF2AC(void) {
    Work *w = ov00_0221A90C[1];
    if (!w) {
        return;
    }
    if (ov00_021EC230()) {
        return;
    }
    w = ov00_0221A90C[1];
    switch (w->state) {
    case 0:
    case 5:
        break;
    case 1:
        ov00_021EF8C0();
        break;
    case 2:
    case 3:
    case 4:
        if (w->object && *(void **)w->object) {
            ov00_021FFD9C(w->object);
        }
        if (ov00_0221A90C[1]->active) {
            if (((OS_GetTick() - ov00_0221A90C[1]->start) << 6) / 33514 > 60000) {
                ov00_021EF3C0(6, -61070);
                ov00_0221A90C[1]->active = 0;
            }
        }
        break;
    }
}
