#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {void *freeHead,*freeTail,*head,*tail;u32 pad[2];int write,pending;u32 tag;} CommandState;
extern CommandState sFreeList;extern u8 sCommandArray[];extern void *sWaitingCommandListQueue[];extern void RequestCommandProc(void);struct SNDCommand;extern const struct SNDCommand *SND_RecvCommandReply(u32);
BOOL SND_FlushCommand(u32);
BOOL SND_FlushCommand(u32 flags){OSIntrMode irq=OS_DisableInterrupts();if(!sFreeList.head){OS_RestoreInterrupts(irq);return 1;}if(sFreeList.pending>=8){if(!(flags&1)){OS_RestoreInterrupts(irq);return 0;}do{SND_RecvCommandReply(1);}while(sFreeList.pending>=8);if(!sFreeList.head){OS_RestoreInterrupts(irq);return 1;}}DC_FlushRange(sCommandArray,0x1800);if(PXI_SendWordByFifo((PXIFifoTag)7,(u32)sFreeList.head,0)<0){if(!(flags&1)){OS_RestoreInterrupts(irq);return 0;}while(sFreeList.pending>=8||PXI_SendWordByFifo((PXIFifoTag)7,(u32)sFreeList.head,0)<0){OS_RestoreInterrupts(irq);SND_RecvCommandReply(0);irq=OS_DisableInterrupts();DC_FlushRange(sCommandArray,0x1800);if(!sFreeList.head){OS_RestoreInterrupts(irq);return 1;}}}sWaitingCommandListQueue[sFreeList.write]=sFreeList.head;sFreeList.write++;if(sFreeList.write>8)sFreeList.write=0;sFreeList.head=0;sFreeList.tail=0;sFreeList.pending++;sFreeList.tag++;OS_RestoreInterrupts(irq);if(flags&2)RequestCommandProc();return 1;}
