#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    void *allocator;
    u32 unused;
    volatile u32 size;
} Container;

extern void DWC_Free(int, void *, int);
Container *ov45_02238508(Container *);

Container *ov45_02238508(Container *s) {
    if (s->allocator) {
        u32 start = s->size;

        struct {
            u8 control;
        } local = { 0 };

        s->size = s->size - start;
        (void)local;
        if (s->allocator) {
            DWC_Free(0, s->allocator, 0);
        }
    }
    return s;
}
