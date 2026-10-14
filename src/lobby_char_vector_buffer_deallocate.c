#pragma force_active on
#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    void *data;
    volatile u32 size;
} Work;

void DWC_Free(int, void *, u32);
Work *LobbyCharVectorBuffer_Deallocate(Work *);

static inline void init(u8 *p) {
    *p = 0;
}

Work *LobbyCharVectorBuffer_Deallocate(Work *w) {
    if (w->data) {
        u32 old = w->size;

        struct {
            volatile u8 zero;
        } init = { 0 };

        w->size -= old;
        if (w->data) {
            DWC_Free(0, w->data, 0);
        }
    }
    return w;
}
