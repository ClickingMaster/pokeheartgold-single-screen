#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
typedef struct HB HB;

struct HB {
    HB *prev, *next;
    u16 owner, magic;
};

typedef struct {
    HB *first, *last;
    volatile u16 count;
    u16 id;
} HM;

extern u32 __VENEER_OS_DisableIrqMask(u32);
extern void __VENEER_OS_EnableIrqMask(u32);
typedef void *(*W40824Allocator)(u32);

typedef union {
    u32 arena;
    W40824Allocator callback;
} W40824Provider;

typedef struct {
    u8 pad[0x17c];
    u32 mode;
    W40824Provider provider;
    u32 heap;
    HM managers[11];
    u8 remaining[0x694 - 0x20c];
} Alloc;

extern void *__VENEER_OS_AllocFromHeap(u32, u32, u32);
extern int AddHeapBuf(HM *, HB *);
typedef char W40824HeaderSize[(sizeof(HB) == 12) ? 1 : -1];
void *AllocateHeapBuf(void *manager, u32 size);

void *AllocateHeapBuf(void *manager, u32 size) {
    HM *m = manager;
    Alloc *w = *(Alloc **)0x380fff4;
    HB *b;
    if (!size) {
        return 0;
    }
    switch (w->mode) {
    case 0:
        b = __VENEER_OS_AllocFromHeap(w->provider.arena, w->heap, size + sizeof(HB));
        break;
    case 1:
        b = w->provider.callback(size + sizeof(HB));
        break;
    }
    if (!b) {
        return 0;
    }
    b->magic = 0xbf1d;
    b->owner = 0;
    AddHeapBuf(m, b);
    return b;
}
