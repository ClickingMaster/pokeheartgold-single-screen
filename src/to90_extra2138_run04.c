#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u16 pending,received;u8 data[508];} Slot;
typedef struct {Slot slots[4];u8 gap[8];u16 index;u8 gap2[4];u16 enabled,length;u8 gap3[6];u16 next;} Sharing;
extern void *WmGetSharedDataAddress(Sharing*,u32,void*,u32);
void WmDataSharingReceiveData(Sharing*,u32,const void*);
void WmDataSharingReceiveData(Sharing*w,u32 participant,const void*data){u16 bit=(u16)(1U<<participant);u32 enabled=w->enabled;u32 index;if(!(enabled&bit))return;index=w->index;if(!(w->slots[index].pending&bit)){if(w->next!=1)return;index=(index+1)&3;if(!(w->slots[index].pending&bit))return;}void*dest=WmGetSharedDataAddress(w,enabled,w->slots[index].data,participant);if(data)MIi_CpuCopy16(data,dest,w->length);else MIi_CpuClear16(0,dest,w->length);OSIntrMode irq=OS_DisableInterrupts();w->slots[index].pending&=~bit;w->slots[index].received|=bit;OS_RestoreInterrupts(irq);}
