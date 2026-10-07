#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u32 init; void (*callback)(int,int,int);u8 pad[0x30]; volatile u16 error,busy;} TPState;
extern TPState _021E36B0;
void TP_RequestAutoSamplingStopAsync(void);
void TP_RequestAutoSamplingStopAsync(void){OSIntrMode irq=OS_DisableInterrupts();int ok=PXI_SendWordByFifo(6,0x03000200,0)>=0;if(!ok){OS_RestoreInterrupts(irq);_021E36B0.error|=4;if(_021E36B0.callback)((void(*)(int,int,int))_021E36B0.callback)(2,4,0);return;}_021E36B0.busy|=4;_021E36B0.error&=~4;OS_RestoreInterrupts(irq);}
