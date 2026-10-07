#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u32 init; void (*callback)(int,int,int);u8 pad[0x30]; volatile u16 error,busy;} TPState;
extern TPState _021E36B0;
void TP_RequestSamplingAsync(void);
void TP_RequestSamplingAsync(void){OSIntrMode irq=OS_DisableInterrupts();int ok=PXI_SendWordByFifo(6,0x3000000,0)>=0;if(!ok){OS_RestoreInterrupts(irq);_021E36B0.error|=1;if(_021E36B0.callback)((void(*)(int,int,int))_021E36B0.callback)(0,4,0);return;}_021E36B0.busy|=1;_021E36B0.error&=~1;OS_RestoreInterrupts(irq);}
