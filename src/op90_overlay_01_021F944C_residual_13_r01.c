#pragma force_active on
#include "global.h"
#include "sys_task_api.h"
#include "map_object.h"
typedef struct{u16 count,arg;u32 unused;u8 *first,*second,*third;u32 reserved;SysTask*main1,*main2,*vblank,*vwait;} Data;
typedef struct{u8 prefix[256];Data*data;MapObjectManager*manager;}Work;
void ov01_021FA564(SysTask*,void*),ov01_021FA7F8(SysTask*,void*),ov01_021FA6E0(SysTask*,void*),ov01_021FA71C(SysTask*,void*);
void ov01_021FA854(Work*,u32,u32);
// @fn ov01_021FA854
void ov01_021FA854(Work*w,u32 count,u32 arg){u32 size,s16,s12,s24,priority;Data*d;s12=count*12;s16=count*16;s24=s12*2;size=s24+s16+sizeof(Data);d=Heap_Alloc((enum HeapID)4,size);if(!d)GF_AssertFail();memset(d,0,size);d->count=count;d->arg=arg;d->first=(u8*)d+sizeof(Data);d->second=d->first+s12;d->third=d->second+s16;w->data=d;priority=MapObjectManager_GetPriority(w->manager);d->main1=SysTask_CreateOnMainQueue(ov01_021FA564,w,priority+1);d->main2=SysTask_CreateOnMainQueue(ov01_021FA7F8,w,priority+2);d->vblank=SysTask_CreateOnVBlankQueue(ov01_021FA6E0,w,255);d->vwait=SysTask_CreateOnVWaitQueue(ov01_021FA71C,w,255);}
