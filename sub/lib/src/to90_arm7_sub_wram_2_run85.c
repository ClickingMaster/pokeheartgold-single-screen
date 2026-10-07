#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))
extern u32 OS_DisableInterrupts(void);extern void OS_RestoreInterrupts(u32),MIC_IrqHandler(void);extern u32 micIntrInfo[4];
void MIC_EnableMultipleInterrupt(void);
// @fn MIC_EnableMultipleInterrupt
void MIC_EnableMultipleInterrupt(void){u32 previous=W(0x0380fffc);if(previous!=(u32)MIC_IrqHandler){u32 irq;micIntrInfo[0]=0;micIntrInfo[1]=0x0380fe80;micIntrInfo[2]=64;micIntrInfo[3]=previous;irq=OS_DisableInterrupts();W(0x0380fffc)=(u32)MIC_IrqHandler;OS_RestoreInterrupts(irq);}}
