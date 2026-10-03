#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct { u8 key[32]; u8 session[48]; u32 tick,address; u16 port; u8 valid,pad; } Slot;
extern Slot _021D5E68[4];
extern int memcmp(const void *,const void *,u32);
void cache_session(void *,u32,u16);
void purge_session(void *);
void purge_session(void *source) {
 int i;OSIntrMode irq;Slot *cur;irq=OS_DisableInterrupts();cur=_021D5E68;
 for(i=0;i<4;i++,cur++) if(cur->valid && memcmp(cur,(u8*)source+0x74,32)==0) {cur->valid=0;break;}
 OS_RestoreInterrupts(irq);
}
