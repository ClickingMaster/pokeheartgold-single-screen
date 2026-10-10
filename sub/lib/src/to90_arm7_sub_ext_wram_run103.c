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

typedef struct {
    u8 pad[0x17c];
    u32 mode, arena, heap;
} Alloc;

extern void __VENEER_OS_FreeToHeap(u32, u32, void *);
extern int DeleteHeapBuf(HM *, HB *);
int ReleaseHeapBuf(HM *m, HB *b);

int ReleaseHeapBuf(HM *m, HB *b) {
    Alloc *w = *(Alloc *volatile *)0x380fff4;
    int result;
    if (b->magic != 0xbf1d) {
        return 1;
    }
    result = DeleteHeapBuf(m, b);
    if (!result) {
        switch (w->mode) {
        case 0:
            __VENEER_OS_FreeToHeap(w->arena, w->heap, b);
            break;
        case 1:
            ((void (*)(void *))w->heap)(b);
            break;
        }
    }
    return result;
}
