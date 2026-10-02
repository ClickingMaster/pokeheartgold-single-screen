#pragma force_active on
#include "global.h"
typedef struct{void*owner;u8 state,frames,tick,pad;s16 x,y;int vx,vy,px,py;void*sprite;} Job;typedef struct{int x,y,z;} Vec;
extern void Sprite_SetMatrix(void*,Vec*),Sprite_SetPositionXY(void*,s16,s16),Heap_Free(void*),SysTask_Destroy(void*);

void ov59_02239C90(void*task,Job*j);

void ov59_02239C90(void*task,Job*j){void*owner=j->owner;switch(j->state){case 0:{Vec v;u8 old;v.x=j->px;v.y=j->py;v.z=0;Sprite_SetMatrix(j->sprite,&v);j->px+=j->vx;j->py+=j->vy;old=j->tick++;if(old>=j->frames)j->state++;break;}case 1:default:Sprite_SetPositionXY(j->sprite,j->x,j->y);*((u8*)owner+0x50)-=1;Heap_Free(j);SysTask_Destroy(task);break;}}
