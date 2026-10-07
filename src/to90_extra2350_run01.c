#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include <string.h>
typedef struct {u8 id[32],record[48];u32 pad,connected;u16 busy;u8 valid,pad2;} Session;
typedef struct {u8 record[48],present;u8 pad[0x43];u8 id[32];} Request;
extern Session _021D5E68[4];
void find_session_from_id(Request *);
void find_session_from_id(Request *p){
 int i;OSIntrMode irq;Session *q;
 irq=OS_DisableInterrupts();i=0;q=_021D5E68;p->present=0;
 for(;i<4;i++,q++){
  if(q->valid && q->connected==0 && q->busy==0 && memcmp(q->id,p->id,32)==0){MI_CpuCopy8(q->record,p,48);p->present=1;break;}
 }
 OS_RestoreInterrupts(irq);
}
