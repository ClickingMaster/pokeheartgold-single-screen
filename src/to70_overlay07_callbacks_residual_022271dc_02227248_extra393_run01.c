#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct I{u8 pad[4];void*ctx;u8 gap[20];void*sprites[4];int count;}I;
typedef struct W{int ticks,limit;u8 state;u8 pad[3];void*ctx;u8 gap[12];void*sprites[4];int count;u8 extra[4];}W;
extern void*ov07_022324D8(void*,int);extern void ov07_02231FE4(void*,void*);
extern u32 ov07_0221C4A8(void *p, int i);extern void*ov07_0221C4E8(void*,int);extern void ManagedSprite_SetDrawPriority(void*,int);extern const int ov07_022378B0[4];extern void*ov07_0221C410(void*,void(*)(void*,W*),W*);extern void ov07_0222719C(void*,W*);extern int ov07_0221FAF8(void*,int);extern void*ov07_02222D88(int,int),*ov07_0221BFD0(void*),*ov07_02222CCC(int,int,int,int,int,int,int,void*,void*);extern void ov07_02222D3C(void*),ov07_0221C448(void*,void*),Heap_Free(void*);void ov07_022271DC(void*),ov07_02227248(void*,W*);
void ov07_022271DC(void*ctx){I*w=ov07_022324D8(ctx,52);int i;const int*priority;ov07_02231FE4(ctx,w);if(((s32)ov07_0221C4A8(ctx, 0))==0)w->count=3;else w->count=((s32)ov07_0221C4A8(ctx, 0));priority=ov07_022378B0;for(i=0;i<4;i++){w->sprites[i]=ov07_0221C4E8(w->ctx,i);if(w->sprites[i])ManagedSprite_SetDrawPriority(w->sprites[i],priority[i]);}ov07_0222719C(ov07_0221C410(w->ctx,ov07_0222719C,(W*)w),(W*)w);}
void ov07_02227248(void*task,W*w){switch(w->state){case 0:{int value;void*resource;void*other;w->ticks=0;value=ov07_0221FAF8(w->ctx,2);resource=ov07_02222D88(0,0);other=ov07_0221BFD0(w->ctx);w->sprites[2]=ov07_02222CCC(0,160,341,131072,200,value,0,resource,other);w->state++;break;}case 1:w->ticks++;if(w->ticks<w->limit)break;ov07_02222D3C(w->sprites[2]);w->state++;default:ov07_0221C448(w->ctx,task);Heap_Free(w);break;}}
