#pragma force_active on
#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/mi/dma.h>
#include <nitro/os/system.h>
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct DmaRegisters {
    volatile u32 source;
    volatile u32 destination;
    volatile u32 control;
} DmaRegisters;

void MI_WaitDma(u32 dmaNo);
void MI_StopDma(u32 dmaNo);

#pragma opt_common_subs off

void MI_WaitDma(u32 dmaNo) {
    OSIntrMode saved = OS_DisableInterrupts();
    while (*(volatile u32 *)(REG_DMA0SAD_ADDR + ((dmaNo * 3 + 2) << 2)) & MI_DMA_ENABLE) {}
    // Channel 0 is left with a harmless source/destination and control word.
    if (dmaNo == 0) {
        DmaRegisters *reg = &((DmaRegisters *)REG_DMA0SAD_ADDR)[dmaNo];
        reg->source = 0;
        reg->destination = 0;
        reg->control = MI_DMA_ENABLE | MI_DMA_SRC_FIX | MI_DMA_DEST_FIX | 1;
    }
    OS_RestoreInterrupts(saved);
}

#pragma opt_common_subs on

void MI_StopDma(u32 dmaNo) {
    OSIntrMode saved = OS_DisableInterrupts();
    volatile u16 *ctrl = &((volatile u16 *)REG_DMA0SAD_ADDR)[dmaNo * 6 + 5];
    *ctrl &= ~0x3200u;
    *ctrl &= ~0x8000u;
    (void)*ctrl;
    (void)*ctrl;
    if (dmaNo == 0) {
        DmaRegisters *reg = &((DmaRegisters *)REG_DMA0SAD_ADDR)[dmaNo];
        reg->source = 0;
        reg->destination = 0;
        reg->control = MI_DMA_ENABLE | MI_DMA_SRC_FIX | MI_DMA_DEST_FIX | 1;
    }
    OS_RestoreInterrupts(saved);
}
