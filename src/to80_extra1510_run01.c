#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "heap.h"
#pragma force_active on
typedef struct {BgConfig *p[3];} Parent;
typedef struct {u8 pad[12];int first[6],second[6];s16 value;u8 p2[10];int timer;u8 flag1,flag2,done1,done2,p3[32];} Data;
typedef struct {int state,flag;u32 pad;Data *data;Parent *parent;int *done;} Context;
extern const BgTemplate ov120_022601BC,ov120_022601D8;
extern void ov120_0225FECC(void*,Context*),ov01_021EFEC8(void*,int,int,int,int),ov120_0225F6AC(void*,int,int,int),ov01_021EFCF8(int,int,int,void*,int),ov01_021F12B4(void*,int),ov01_021F12E8(void*,int,int,int,int,int,void*,int,int),sub_0200FC20(int),ov01_021F12D0(void*),ov01_021EFCDC(Context*,void*),ov120_0225F6BC(void*);
extern int ov120_0225FEE8(void*),ov01_021EFF28(void*);
void ov120_0225FF00(void*,Context*,int);
void ov120_0225FF00(void *task,Context *c,int mode){Data *d=c->data;switch(c->state){case 0:c->data=Heap_Alloc((enum HeapID)4,112);memset(c->data,0,112);d=c->data;GfGfx_EngineATogglePlanes(2,1);GfGfx_EngineATogglePlanes(8,1);*(volatile u32*)0x4000000=(*(volatile u32*)0x4000000 &0xffffe0ff)|0x1500;FreeBgTilemapBuffer(c->parent->p[2],1);InitBgFromTemplate(c->parent->p[2],1,&ov120_022601BC,0);BgClearTilemapBufferAndCommit(c->parent->p[2],1);FreeBgTilemapBuffer(c->parent->p[2],3);InitBgFromTemplate(c->parent->p[2],3,&ov120_022601D8,0);BgClearTilemapBufferAndCommit(c->parent->p[2],3);SetBgPriority(2,0);SetBgPriority(8,0);GfGfx_EngineASetPlanes(21);ov120_0225FECC((u8*)d+0x50,c);c->state=1;break;
case 1:if(!ov120_0225FEE8((u8*)d+0x50))break;BG_SetMaskColor(1,0);ov01_021EFEC8(d->first,0,0x140000,0,18);ov01_021EFEC8(d->second,0,0x140000,0x8000,30);ov120_0225F6AC(&d->value,0,16,15);ScheduleSetBgPosText(c->parent->p[2],3,(enum BgPosAdjustOp)3,d->first[0]>>12);ScheduleSetBgPosText(c->parent->p[2],3,(enum BgPosAdjustOp)0,0);ScheduleSetBgPosText(c->parent->p[2],1,(enum BgPosAdjustOp)3,d->second[0]>>12);ScheduleSetBgPosText(c->parent->p[2],1,(enum BgPosAdjustOp)0,0);{int brightness;if(mode)brightness=16;else brightness=-16;ov01_021EFCF8(1,brightness,brightness,&c->flag,2);}c->state=2;break;
case 2:if(c->flag){ov01_021F12B4(d,4);c->state=3;}break;
case 3:d->timer=10;ov01_021F12E8(d,0,191,3410,0x3000,400,(void*)0x4000010,0,4);d->flag2=1;GfGfx_EngineATogglePlanes(8,1);c->state=4;
case 4:if(d->timer--==0)c->state=5;break;
case 5:d->flag1=1;GfGfx_EngineATogglePlanes(2,1);c->state=6;break;
case 6:if(d->done1)c->state=7;break;
case 7:sub_0200FC20(0);if(c->done)*c->done=1;ov01_021F12D0(d);ov01_021EFCDC(c,task);return;
}if(d->flag1){d->done1=ov01_021EFF28(d->first);ScheduleSetBgPosText(c->parent->p[2],1,(enum BgPosAdjustOp)3,d->first[0]>>12);}if(d->flag2){d->done2=ov01_021EFF28(d->second);ov120_0225F6BC(&d->value);ScheduleSetBgPosText(c->parent->p[2],3,(enum BgPosAdjustOp)3,d->second[0]>>12);ScheduleSetBgPosText(c->parent->p[2],3,(enum BgPosAdjustOp)0,d->value);}}
