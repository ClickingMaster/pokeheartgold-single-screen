#pragma force_active on
#include "global.h"
#include "sys_task.h"
#include "heap.h"
typedef struct {u8 pad[0x20];void *pic;u8 rest[0x17c];u32 flag;} Battle;
typedef struct {void *owner;Battle *battle;u8 info[0x58];u8 param,actor,state,hide;u32 enable;} Context;
typedef struct {u8 bytes[88];} Buffer;
extern void *ov12_0223A8DC(void *);
extern void Pokepic_SetAttr(void *,s32,s32),ov12_022643C8(void *,s32,Buffer *,s32,s32,u32,u32,s32),ov12_02261B80(void *,Battle *,void *,Buffer *),ov12_02261CA8(void *,void *,void *,u32),ov07_02234A20(void *,s32),ov07_0221C394(void *),ov07_0221C3C0(void *),ov12_0226430C(void *,u32,u32);
extern s32 ov07_0221C3B0(void *);
extern void SysTask_Destroy(SysTask *);
void ov12_02260418(SysTask *task,Context *p);
#pragma force_active on
void ov12_02260418(SysTask *task,Context *p){
 Buffer first;u8 middle[80];Buffer second;void *scene=ov12_0223A8DC(p->owner);
 switch(p->state){case 0:if(p->battle->pic){Pokepic_SetAttr(p->battle->pic,6,p->hide);if(p->hide==1)p->state=255;else p->state++;}else p->state=255;break;
 case 1:if(p->enable&&p->battle->flag==0){ov12_022643C8(p->owner,0,&first,1,15,p->actor,p->actor,0);ov12_02261B80(p->owner,p->battle,scene,&first);p->state++;}else p->state=255;break;
 case 3:ov12_02261CA8(p->owner,p->info,middle,p->actor);ov07_02234A20(middle,5);ov12_022643C8(p->owner,0,&second,1,16,p->actor,p->actor,0);ov12_02261B80(p->owner,p->battle,scene,&second);p->battle->flag=1;p->state++;break;
 case 2:case 4:ov07_0221C394(scene);if(ov07_0221C3B0(scene)==0){ov07_0221C3C0(scene);p->state++;}break;
 default:ov12_0226430C(p->owner,p->actor,p->param);Heap_Free(p);SysTask_Destroy(task);break;}
}
