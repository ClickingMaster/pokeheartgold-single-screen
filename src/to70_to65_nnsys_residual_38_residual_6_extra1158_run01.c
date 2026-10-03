#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u32 x,commandOffset,materialOffset,shapeOffset;u8 a[5];u8 type,mode;u8 b[5];u32 extra1,extra2;} Model;
typedef struct Input Input;typedef struct Output Output;
struct Input {u32 flags;Model*model;u8 pad[0x20-8];void*slot;u8 slotIndex,slotMode;u16 pad2;void(*callback)(Output*);int pad3;void*command;};
struct Output {void*volatile command;Input*volatile input;u32 flags;void*slots[32];u8 modes[32];u8 pad[0xc4-0xac];int active;u8 gap[0xd4-0xc8];void*modelBase,*materials,*shapes;u32 extra1,extra2;void*handlerA,*handlerB,*handlerC;u8 rest[0x188-0xf4];};
extern void*_02110A80[],*_02110A74[],*_02110A8C[];extern void G3dDrawInternal_Loop_(Output*);
static inline void *Materials(Model*m){if(m){u32 off=m->materialOffset;if(off)return(u8*)m+off;}return 0;}
static inline void *Shapes(Model*m){if(m){u32 off=m->shapeOffset;if(off)return(u8*)m+off;}return 0;}

void G3dDrawInternal_(Output*w,Input*p);

void G3dDrawInternal_(Output*w,Input*p){void*commands;MIi_CpuClearFast(0,(u32*)w,0x188);w->active=1;w->flags=1;commands=p->command;if(commands==0)commands=(u8*)p->model+p->model->commandOffset;w->command=commands;w->input=p;w->modelBase=p->model?(u8*)p->model+0x40:0;w->materials=Materials(p->model);w->shapes=Shapes(p->model);w->handlerA=_02110A80[p->model->type];w->handlerB=_02110A74[p->model->type];w->handlerC=_02110A8C[p->model->mode];w->extra1=p->model->extra1;w->extra2=p->model->extra2;if(p->slot && p->slotIndex<32){w->slots[p->slotIndex]=p->slot;w->modes[p->slotIndex]=p->slotMode;}if(p->flags&1)w->flags|=0x80;if(p->flags&2)w->flags|=0x100;if(p->flags&4)w->flags|=0x200;if(p->flags&8)w->flags|=0x400;if(p->callback)p->callback(w);G3dDrawInternal_Loop_(w);p->flags&=~1;}
