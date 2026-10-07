#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on

#define W09_RAM_ROOT (*(u8 **)0x0380fff4)
#define W09_MMIO16(address) (*(volatile u16 *)(u32)(address))
typedef struct W09TxDescriptor {
    u8 opaque0[8];
    u8 *frame;
    u8 opaqueC[8];
} W09TxDescriptor;

int WCheckTxBufIdBeforeFrame(void *descriptor);
int WCheckTxBufIdBeforeFrame(void *descriptor)
{
    volatile u16 *prefix;
    u8 *frame = ((W09TxDescriptor *)descriptor)->frame;
    prefix = (volatile u16 *)((u32)frame - 4);
    if (*prefix == 0xb6b8 && W09_MMIO16((u32)prefix + 2) == 0x1d46) {
        return 0;
    }
    W09_MMIO16((u32)frame + 10) = 1;
    *prefix = 0xb6b8;
    W09_MMIO16((u32)prefix + 2) = 0x1d46;
    frame = ((W09TxDescriptor *)descriptor)->frame;
    W09_MMIO16((u32)frame + 12) = 0xb6b8;
    W09_MMIO16((u32)frame + 14) = 0x1d46;
    ++*(u16 *)(W09_RAM_ROOT + 0x3fa);
    return 1;
}
