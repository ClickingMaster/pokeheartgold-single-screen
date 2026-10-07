#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))
extern u32 OS_DisableInterrupts(void);extern void OS_RestoreInterrupts(u32),MIC_IrqHandler(void);extern u32 micIntrInfo[4];
void MIC_DisableMultipleInterrupt(void);
// @fn MIC_DisableMultipleInterrupt
void MIC_DisableMultipleInterrupt(void){if(W(0x0380fffc)==(u32)MIC_IrqHandler){u32 irq=OS_DisableInterrupts();W(0x0380fffc)=micIntrInfo[3];OS_RestoreInterrupts(irq);}}
