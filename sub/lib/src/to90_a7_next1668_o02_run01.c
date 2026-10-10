#pragma force_active on
#include <nitro/os/system.h>
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    volatile u32 source, destination, control;
} Dma40876;

void MI_WaitDma(u32 dmaNo);

typedef struct {
    volatile u32 source, destination, control;
} Dma40877;

void MI_StopDma(u32 dmaNo);
#pragma opt_common_subs off

void MI_WaitDma(u32 dmaNo) {
    OSIntrMode saved = OS_DisableInterrupts();
    while (*(volatile u32 *)(0x040000b0u + ((dmaNo * 3 + 2) << 2)) & 0x80000000u) {}
    if (dmaNo == 0) {
        Dma40876 *reg = &((Dma40876 *)0x040000b0)[dmaNo];
        reg->source = 0;
        reg->destination = 0;
        reg->control = 0x81400001u;
    }
    OS_RestoreInterrupts(saved);
}

#pragma opt_common_subs on

void MI_StopDma(u32 dmaNo) {
    OSIntrMode saved = OS_DisableInterrupts();
    volatile u16 *ctrl = &((volatile u16 *)0x040000b0)[dmaNo * 6 + 5];
    *ctrl &= ~0x3200u;
    *ctrl &= ~0x8000u;
    (void)*ctrl;
    (void)*ctrl;
    if (dmaNo == 0) {
        Dma40877 *reg = &((Dma40877 *)0x040000b0)[dmaNo];
        reg->source = 0;
        reg->destination = 0;
        reg->control = 0x81400001u;
    }
    OS_RestoreInterrupts(saved);
}
