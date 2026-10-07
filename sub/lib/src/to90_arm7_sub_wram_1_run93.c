#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma dollar_identifiers on
extern void MI_StopDma(u32),CTRDG_VibPulseEdgeUpdate(u32),SND_BeginSleep(void),WVR_Shutdown(void),OS_Terminate(void),OS_RestoreInterrupts(u32);extern u32 OS_DisableInterrupts(void);
void CARDi_CallbackForPulledOut(u32 tag,u32 data,int error);
// @fn CARDi_CallbackForPulledOut
void CARDi_CallbackForPulledOut(u32 tag,u32 data,int error){(void)tag;(void)error;if((data&63)==1){u32 irq;MI_StopDma(0);MI_StopDma(1);MI_StopDma(2);MI_StopDma(3);CTRDG_VibPulseEdgeUpdate(0);irq=OS_DisableInterrupts();SND_BeginSleep();WVR_Shutdown();OS_RestoreInterrupts(irq);OS_Terminate();}else OS_Terminate();}
