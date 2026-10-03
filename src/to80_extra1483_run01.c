#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct Link Link;struct Link{Link *next;};
typedef struct {u8 pad[256];u32 flags;Link *head;u16 count;u16 pad2;OSThreadQueue queue;} Pipe;
typedef struct {u8 pad[100];Pipe *in,*out;u32 pad2;u16 flags;u8 unknown;s8 mode;} Socket;
typedef struct {u8 pad[28];void(*free)(void*);} Config;
extern Config *_02110628;
extern void SOCLi_FreeCommandPipe(Pipe*),SOCLi_SocketUnregister(Socket*),SOCLi_SocketUnregisterTrash(Socket*);
void SOCLi_CleanupSocket(Socket*);
void SOCLi_CleanupSocket(Socket *s){OSIntrMode irq;if(!s)return;s->flags=0;if((s->mode==0 || s->mode==4)?TRUE:FALSE){SOCLi_FreeCommandPipe(s->out);SOCLi_FreeCommandPipe(s->in);}else if(s->mode==1){Link *p=s->in->head;while(p){Link *next=p->next;_02110628->free(p);p=next;}s->in->count=0;s->in->flags=0;s->in->head=0;OS_WakeupThread(&s->in->queue);SOCLi_FreeCommandPipe(s->in);}else if(s->mode==2)SOCLi_FreeCommandPipe(s->out);
irq=OS_DisableInterrupts();SOCLi_SocketUnregister(s);SOCLi_SocketUnregisterTrash(s);_02110628->free(s);OS_RestoreInterrupts(irq);}
