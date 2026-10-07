#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern void FSi_ReleaseCommand(FSFile*,FSResult);
static inline BOOL Suspended(volatile FSArchive *a){return (a->flag&8)?TRUE:FALSE;}
static inline BOOL Busy(volatile FSArchive *a){return (a->flag&16)?TRUE:FALSE;}
static inline BOOL Sync(volatile FSFile *f){return (f->stat&4)?TRUE:FALSE;}
int FSi_SendCommand(FSFile *f,FSCommandType command){volatile FSArchive *a=f->arc;u32 mask=1<<command;OSIntrMode mode;
 f->command=command;f->error=(FSResult)2;f->stat|=1;mode=OS_DisableInterrupts();
 if(a->flag&0x80){FSi_ReleaseCommand(f,(FSResult)3);OS_RestoreInterrupts(mode);return 0;}
 if(mask&0x1fc)f->stat|=4;
 {FSFile *tail=(FSFile*)&a->list;volatile FSFileLink *link=&f->link;FSFile *prev=link->prev,*next=link->next;if(prev)prev->link.next=next;if(next)next->link.prev=prev;while(tail->link.next)tail=tail->link.next;tail->link.next=f;f->link.prev=tail;f->link.next=0;}
 if(!Suspended(a)&&!Busy(a)){a->flag|=16;OS_RestoreInterrupts(mode);if(a->proc_flag&0x200)a->proc(f,(FSCommandType)9);mode=OS_DisableInterrupts();f->stat|=0x40;if(!Sync(f)){OS_RestoreInterrupts(mode);FSi_ExecuteAsyncCommand(f);return 1;}OS_RestoreInterrupts(mode);}
 else {if(!Sync(f)){OS_RestoreInterrupts(mode);return 1;}do {OS_SleepThread((OSThreadQueue*)f->queue);}while(!(f->stat&0x40));OS_RestoreInterrupts(mode);}
 return FSi_ExecuteSyncCommand(f);
}
