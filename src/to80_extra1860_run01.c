#pragma force_active on
// clang-format off
#define NITRO_CARD_BACKUP_BACKUP_H_
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Config{struct{u32 low:14;u32 recycle:1;u32 rest:17;}flags;char pad4[0x2e];u16 start;char pad34[8];u16 end,pad3e;}Config;
typedef struct Ref{Config*data;}Ref;
typedef struct Emitter{struct Emitter*next;char pad4[8];void*particles;char pad10[8];void*children;int pad1c;Ref*ref;struct{u32 remove:1;u32 bit1:1;u32 pause:1;u32 bit3:1;u32 started:1;u32 rest:27;}flags;char pad28[0x24];u16 age;char pad4e[0x32];struct{u32 low:16;u32 interval:3;u32 rest:13;}period;}Emitter;
typedef struct List{Emitter*head,*tail;int count;}List;
typedef struct Work{char pad[4];List active,free;char pad1c[0x48-0x1c];u16 frame,pad4a;}Work;
extern void spl_calc(Work*,Emitter*),spl_push_front(List*,Emitter*);extern Emitter*spl_del(List*,Emitter*);
void SPL_Calc(Work*);
void SPL_Calc(register Work*w){Work*work=w;Emitter*e;Emitter*next;Config*c;e=work->active.head;if(e){do{c=e->ref->data;next=e->next;if(!e->flags.started&&e->age>=c->start){e->flags.started=1;e->age=0;}if(!e->flags.pause){u32 period=e->period.interval;if(!period||work->frame==period-1)spl_calc(work,e);}if((c->flags.recycle&&c->end&&e->flags.started&&e->age>c->end)||e->flags.remove){if(!e->particles&&!e->children)spl_push_front(&work->free,spl_del(&work->active,e));}e=next;}while(e);}++work->frame;if(work->frame>1)work->frame=0;}
