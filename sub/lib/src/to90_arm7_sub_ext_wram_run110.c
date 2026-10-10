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
extern void InitHeapBufMan(HM *, u32);
extern void *AllocateHeapBuf(void *, u32);
void InitializeHeapBuf(u32 *r);

void InitializeHeapBuf(u32 *r) {
    u8 *w = *(u8 *volatile *)0x380fff4;
    u8 *param = w + 0x344;
    *(u32 *)(w + 0x17c) = r[0];
    *(u32 *)(w + 0x180) = r[1];
    *(u32 *)(w + 0x184) = r[2];
    InitHeapBufMan((HM *)(w + 0x188), 2);
    InitHeapBufMan((HM *)(w + 0x194), 3);
    InitHeapBufMan((HM *)(w + 0x1a0), 4);
    InitHeapBufMan((HM *)(w + 0x1ac), 5);
    InitHeapBufMan((HM *)(w + 0x1b8), 6);
    InitHeapBufMan((HM *)(w + 0x1c4), 7);
    InitHeapBufMan((HM *)(w + 0x1d0), 8);
    InitHeapBufMan((HM *)(w + 0x1dc), 9);
    InitHeapBufMan((HM *)(w + 0x1e8), 10);
    InitHeapBufMan((HM *)(w + 0x1f4), 11);
    InitHeapBufMan((HM *)(w + 0x200), 12);
    *(u32 *)(param + 0x9c) = (u32)AllocateHeapBuf((HM *)(w + 0x188), 129) + 12;
    *(u16 *)(param + 0xa0) = 0;
    *(u16 *)(param + 0xa4) = 0;
}
