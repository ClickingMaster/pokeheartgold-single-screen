#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void *owner,*sprites;int state,timer;u8 pad[0x9c-16];u8 fade[20];} Work;
extern void ov07_02231E08(void *,int,int),ov07_02222AC4(void *,int,int,int,int,int),ov07_0222F434(Work *),ov07_0222F600(Work *),ov07_0221C448(void *,void *),Heap_Free(void *),SpriteSystem_DrawSprites(void *);
extern int ov07_02222AF4(void *),ov07_0222F4D4(Work *),ov07_0222F6C0(Work *);
void ov07_0222F764(void *,Work *);
void ov07_0222F764(void *task,Work *w){switch(w->state){case 0:ov07_02231E08(w->owner,0,16);ov07_02222AC4(w->fade,0,8,16,8,8);w->state++;break;case 1:if(ov07_02222AF4(w->fade)){w->state++;ov07_0222F434(w);}break;case 2:if(ov07_0222F4D4(w)){w->state++;w->timer=2;}break;case 3:if(--w->timer<0){ov07_0222F600(w);w->state++;}break;case 4:if(ov07_0222F6C0(w)){w->state++;ov07_02222AC4(w->fade,8,0,8,16,8);}break;case 5:if(ov07_02222AF4(w->fade))w->state++;break;case 6:ov07_0221C448(w->owner,task);Heap_Free(w);break;}SpriteSystem_DrawSprites(w->sprites);}
