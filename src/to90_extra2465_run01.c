#pragma force_active on
// clang-format off
#include "global.h"
#pragma force_active on
#include <nitro/code32.h>
// clang-format on
typedef struct { u8 key[32]; u8 session[48]; u32 tick,address; u16 port; u8 valid,pad; } Slot;
extern Slot _021D5E68[4];
extern int memcmp(const void *,const void *,u32);
void cache_session(void *,u32,u16);
void cache_session(void *source,u32 address,u16 port) {
 Slot *chosen;int i;u32 age;Slot *cur;OSIntrMode irq;u32 tick;
 irq=OS_DisableInterrupts();
 {u64 stamp=OS_GetTick();chosen=_021D5E68;tick=(u32)(stamp>>16);age=0;i=age;cur=chosen;
 for(;i<4;i++,cur++) {
  if(cur->valid && address && address==cur->address && port && port==cur->port) { chosen=cur; break; }
  if(age!=0xffffffff) {
   if(!cur->valid) {age=0xffffffff;chosen=cur;}
   else if(tick-cur->tick>age) {age=tick-cur->tick;chosen=cur;}
  }
 }
 MI_CpuCopy8((u8*)source+0x74,chosen,32);
 MI_CpuCopy8(source,chosen->session,48);
 chosen->tick=tick; chosen->valid=1; chosen->address=address;chosen->port=port;
 OS_RestoreInterrupts(irq);
 }
}
