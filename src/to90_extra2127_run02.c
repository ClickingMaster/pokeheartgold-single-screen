#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Command{int result;u8 pad[8];void*src,*dst;u32 length;u8 pad2[8];u32 page;}Command;
typedef struct Card{Command*common;u8 pad[24];u8*src,*dst;u32 remaining,unused;int command,tries,mode;void(*callback)(void*);void*arg;u8 pad2[4];u8 thread[200];u8 queue[8];volatile u32 flags;u8 pad3[8];u8 buffer[256];}Card;
extern const char _version_NINTENDO_BACKUP[];void OSi_ReferSymbol(const void*);u32 CARD_GetBackupSectorSize(void);int CARDi_Request(Card*,int,int);void CARDi_RequestStreamCommandCore(Card*);
void CARDi_RequestStreamCommandCore(Card*c){int mode;u32 block;u8*buffer;int command;u32 n;int tries;command=c->command;mode=c->mode;tries=c->tries;block=256;OSIntrMode irq;void*arg;void(*callback)(void*);OSi_ReferSymbol(_version_NINTENDO_BACKUP);if(command==11)block=CARD_GetBackupSectorSize();else if(command==15)block=((Command*)cardi_common.cmd)->page;buffer=c->buffer;do{n=c->remaining;if(block<n)n=block;c->common->length=n;if(c->flags&0x40){c->flags &=~0x40;c->common->result=7;break;}switch(mode){case 0:DC_InvalidateRange(buffer,n);c->common->src=c->src;c->common->dst=buffer;break;case 1:case 2:MI_CpuCopy8(c->src,buffer,n);DC_FlushRange(buffer,n);DC_WaitWriteBufferEmpty();c->common->src=buffer;c->common->dst=c->dst;break;case 3:c->common->src=c->src;c->common->dst=c->dst;break;}if(!CARDi_Request(c,command,tries))break;if(mode==2){if(!CARDi_Request(c,9,1))break;}else if(mode==0)MI_CpuCopy8(buffer,c->dst,n);c->src+=n;c->dst+=n;c->remaining-=n;}while(c->remaining);callback=c->callback;arg=c->arg;irq=OS_DisableInterrupts();c->flags &=~0x4c;OS_WakeupThread((OSThreadQueue*)c->queue);if(c->flags&0x10)OS_WakeupThreadDirect((OSThread*)c->thread);OS_RestoreInterrupts(irq);if(callback)callback(arg);}
