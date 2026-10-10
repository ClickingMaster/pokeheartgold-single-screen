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
extern int ReleaseHeapBuf(HM *, HB *);
void ReleaseAllHeapBuf(HM *m);

void ReleaseAllHeapBuf(HM *m) {
    HB *b = m->first;
    if (m->count) {
        while (b != (HB *)0xffffffff) {
            HB *next = b->next;
            ReleaseHeapBuf(m, b);
            b = next;
        }
    }
}
