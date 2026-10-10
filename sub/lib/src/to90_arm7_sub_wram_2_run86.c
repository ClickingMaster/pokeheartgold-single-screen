#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))
extern u32 OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(u32);
extern void MIC_IrqHandler(void);
extern u32 micIntrInfo[4];
void MIC_DisableMultipleInterrupt(void);

void MIC_DisableMultipleInterrupt(void) {
    if (W(0x0380fffc) == (u32)MIC_IrqHandler) {
        u32 irq = OS_DisableInterrupts();
        W(0x0380fffc) = micIntrInfo[3];
        OS_RestoreInterrupts(irq);
    }
}
