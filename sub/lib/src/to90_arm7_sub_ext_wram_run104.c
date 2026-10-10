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
extern int DeleteHeapBuf(HM *, HB *);
extern int AddHeapBuf(HM *, HB *);
int MoveHeapBuf(HM *from, HM *to, HB *b);

int MoveHeapBuf(HM *from, HM *to, HB *b) {
    int result;
    u32 irq;
    if (b->magic != 0xbf1d) {
        return 1;
    }
    irq = __VENEER_OS_DisableIrqMask(0x1000000);
    result = DeleteHeapBuf(from, b);
    if (!result) {
        result = AddHeapBuf(to, b);
    }
    __VENEER_OS_EnableIrqMask(irq);
    return result;
}
