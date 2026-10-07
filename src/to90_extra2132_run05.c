#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u32 init;void (*callback)(int,int,int);u8 gap[8];u16 cursor,frequency;TPData *buffer;u16 count;u8 padding[0x1a];u16 calibration,other,error,busy;} TouchState;
extern TouchState _021E36B0;
static inline int SendStart(u16 timing,u16 frequency){if(PXI_SendWordByFifo(PXI_FIFO_TAG_TOUCHPANEL,(frequency&255)|0x02000100,FALSE)<0)return 0;if(PXI_SendWordByFifo(PXI_FIFO_TAG_TOUCHPANEL,timing|0x01010000,FALSE)<0)return 0;return 1;}
void TP_RequestAutoSamplingStartAsync(u16,u16,TPData*,u16);
void TP_RequestAutoSamplingStartAsync(u16 timing,u16 frequency,TPData *buffer,u16 count){u32 i;OSIntrMode irq;_021E36B0.buffer=buffer;_021E36B0.cursor=0;_021E36B0.frequency=frequency;_021E36B0.count=count;for(i=0;i<count;i++)((TPData*)_021E36B0.buffer)[i].touch=0;irq=OS_DisableInterrupts();if(!(SendStart(timing,frequency)&255)){OS_RestoreInterrupts(irq);_021E36B0.error|=2;if(_021E36B0.callback)((void(*)(int,int,int))_021E36B0.callback)(1,4,0);return;}_021E36B0.busy|=2;_021E36B0.error&=~2;OS_RestoreInterrupts(irq);}
