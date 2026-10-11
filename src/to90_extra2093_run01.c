#pragma force_active on
#include "global.h"
#include "heap.h"
#include "sys_task.h"
#include <string.h>
typedef struct {void *scene;u32 a,b;void *pic;s16 x,y,top,bottom;void *effect;u8 tween[76];} Context;
extern enum HeapID BattleAnimSystem_GetHeapID(void *);
extern s32 BattleAnimSystem_GetAttacker(void *),ov07_02222004(void *,s32),BattleAnimUtil_GetBattlerType(void *,s32),BattleAnimSystem_GetBgLayer(void *,s32),ov07_0221FAE8(void *),Pokepic_GetAttr(void *,s32);
extern void *BattleAnimSystem_GetBattlerPokepic(void *,s32),*ov07_0221FAF8(void *,s32);
extern SysTask *BattleAnimSystem_CreateTaskDefaultPriority(void *,void (*)(SysTask *,Context *),Context *);
extern void ov07_0222C918(SysTask *,Context *),Pokepic_SetAttr(void *,s32,s32),ov07_02222268(void *,s32,s32,s32,s32,s32),SetBgPriority(u8,u8);
extern s32 ov07_02222D88(u16,u16);
extern void *ov07_02222CCC(u8,u8,s32,s32,s32,void *,u32,s32,enum HeapID);
void ov07_0222CA8C(void *scene);
#pragma force_active on
void ov07_0222CA8C(void *scene){
 Context *p=Heap_Alloc(BattleAnimSystem_GetHeapID(scene),0x68);SysTask *task; s32 direction,bg;void *obj;u32 priority;s32 value;
 memset(p,0,0x68);p->scene=scene;task=BattleAnimSystem_CreateTaskDefaultPriority(scene,ov07_0222C918,p);
 p->pic=BattleAnimSystem_GetBattlerPokepic(p->scene,BattleAnimSystem_GetAttacker(p->scene));p->x=Pokepic_GetAttr(p->pic,0);p->y=Pokepic_GetAttr(p->pic,1);p->y-=Pokepic_GetAttr(p->pic,41);p->x-=40;p->y-=40;Pokepic_SetAttr(p->pic,6,1);
 direction=ov07_02222004(p->scene,BattleAnimSystem_GetAttacker(p->scene));p->top=p->y-8;p->bottom=p->y+88;if(p->top<0)p->top=0;if(p->bottom>191)p->bottom=191;
 obj=ov07_0221FAF8(scene,1);priority=SysTask_GetPriority(task);value=ov07_02222D88(p->x,p->y);p->effect=ov07_02222CCC(p->top,p->bottom,910,direction<<15,80,obj,priority+1,value,BattleAnimSystem_GetHeapID(scene));
 ov07_02222268(p->tween,0,80,0,40,24);*(s32*)(p->tween+8)*=direction;
 if((u32)(BattleAnimUtil_GetBattlerType(p->scene,BattleAnimSystem_GetAttacker(scene))-3)<=1){bg=BattleAnimSystem_GetBgLayer(p->scene,1);SetBgPriority(bg,ov07_0221FAE8(p->scene));SetBgPriority(0,ov07_0221FAE8(p->scene)+1);}ov07_0222C918(task,p);
}
