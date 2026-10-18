#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 byte;
} Allocator;

typedef struct {
    void *data;
    volatile u32 size;
    u32 capacity;
} Buffer;

extern void LobbyEventVector_Allocate(Buffer *, u32, Allocator);
void LobbyEventVector_Reallocate(Buffer *, Allocator, u32);

void LobbyEventVector_Reallocate(Buffer *b, Allocator incoming, u32 n) {
    u32 size = b->size;
    Allocator old = { 0 };
    volatile Allocator copy;
    (void)incoming;
    (void)old;
    b->size -= size;
    if (b->data) {
        if (b->data) {
            DWC_Free(0, b->data, 0);
        }
        b->data = 0;
        b->capacity = 0;
    }
    Allocator fresh = { 0 };
    copy.byte = 0;
    LobbyEventVector_Allocate(b, n, fresh);
}
