#pragma force_active on
#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct {
    u8 byte;
} Allocator;

typedef struct {
    void *data;
    volatile u32 size;
    u32 capacity;
} Buffer;

extern void DWC_Free(int, void *, int);
extern void ov45_0223CE30(Buffer *, u32, Allocator);
void ov45_0223BD24(Buffer *, Allocator, u32);

void ov45_0223BD24(Buffer *b, Allocator incoming, u32 n) {
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
    ov45_0223CE30(b, n, fresh);
}
