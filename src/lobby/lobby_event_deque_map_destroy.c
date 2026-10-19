#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    void *allocator;
    u32 unused;
    u32 size;
} Container;

Container *LobbyEventDequeMap_Destroy(Container *);

Container *LobbyEventDequeMap_Destroy(Container *s) {
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
