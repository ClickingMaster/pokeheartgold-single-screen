#pragma force_active on
#pragma force_active on
#include "global.h"
#include <nitro/fx/fx.h>
#include "heap.h"
#include "sys_task_api.h"
typedef struct {void *owner;u8 a,steps,state,index;s16 x,y;int dx,dy,px,py;void *sprite;} Move;
extern u16 ov59_0223C6C4[],ov59_0223C6C6[];extern void Sprite_GetPositionXY(void *,s16 *,s16 *),ov59_02239C90(SysTask *,void *);void ov59_0223919C(u8 *,int,int);
static inline int ab(int n){return n<0?-n:n;}
void ov59_0223919C(u8 *owner,int index,int type){Move *m=Heap_Alloc(*(enum HeapID*)owner,0x20);s16 x,y;s16 dx,dy;float steps;u16 base;MI_CpuFill8(m,0,0x20);m->owner=owner;m->index=owner[0x4d];if(index==0)m->sprite=*(void**)(owner+0x254+(owner[0x4d]+2)*4);else m->sprite=*(void**)(owner+0x258);Sprite_GetPositionXY(m->sprite,&x,&y);if(type==0){m->x=192+index*4;m->y=96+index*-10;}else{base=ov59_0223C6C4[m->index*2];m->x=base+index*4;base=ov59_0223C6C6[m->index*2];m->y=base+index*-10;}dx=m->x-x;dy=m->y-y;if(ab(dx)>=ab(dy))m->steps=ab(dx)/8;else m->steps=ab(dy)/8;m->px=(int)(x>0?0.5f+(float)(x<<12):(float)(x<<12)-0.5f);m->py=(int)(y>0?0.5f+(float)(y<<12):(float)(y<<12)-0.5f);m->state=0;if(m->steps>0)steps=0.5f+(float)(m->steps<<12);else steps=(float)(m->steps<<12)-0.5f;m->dx=FX_Div(dx>0?0.5f+(float)(dx<<12):(float)(dx<<12)-0.5f,(int)steps);if(m->steps>0)steps=0.5f+(float)(m->steps<<12);else steps=(float)(m->steps<<12)-0.5f;m->dy=FX_Div(dy>0?0.5f+(float)(dy<<12):(float)(dy<<12)-0.5f,(int)steps);SysTask_CreateOnMainQueue(ov59_02239C90,m,0);owner[0x50]++;}
