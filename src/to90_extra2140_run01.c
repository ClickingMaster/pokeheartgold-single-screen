#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u16 initialized,pad;void*buffer;}State;extern State _021E4220;extern int WMi_CheckInitialized(void),WMi_CheckStateEx(int,...);extern void WmClearFifoRecvFlag(void);int WM_Finish(void);
int WM_Finish(void){OSIntrMode irq=OS_DisableInterrupts();int result;if(WMi_CheckInitialized()){OS_RestoreInterrupts(irq);return 3;}result=WMi_CheckStateEx(1,0);if(result)return result;WmClearFifoRecvFlag();PXI_SetFifoRecvCallback((PXIFifoTag)10,0);_021E4220.buffer=0;_021E4220.initialized=0;OS_RestoreInterrupts(irq);return 0;}
