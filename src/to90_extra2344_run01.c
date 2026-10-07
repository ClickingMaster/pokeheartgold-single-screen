#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern void *NNS_FndGetNextListObject(void *,void *);extern void NNS_FndRemoveListObject(void *,void *),FreeCommandBuffer(void *);
void RemoveCommandByPlayer(void *,void *);
#pragma force_active on
#pragma opt_lifetimes off
void RemoveCommandByPlayer(void *list,void *player){
 OSIntrMode irq=OS_DisableInterrupts();void *current=NNS_FndGetNextListObject(list,NULL),*next;
 while(current){next=NNS_FndGetNextListObject(list,current);if(*(void **)((u8 *)current+8)==player){NNS_FndRemoveListObject(list,current);FreeCommandBuffer(current);}current=next;}
 OS_RestoreInterrupts(irq);
}
