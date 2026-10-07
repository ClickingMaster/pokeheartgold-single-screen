#include "global.h"
#pragma force_active on
#include "heap.h"
#include "pokepic.h"
#include "sys_task_api.h"
#include "unk_02016EDC.h"
#include "pokepic_animation_internal.h"
void sub_02016F40(void*managerArg,void*picArg,struct UnkStruct_0207294C*in,u8 index){PicManager*w=managerArg;Pokepic*pic=picArg;int member=in->unk_0,value=in->unk_2;if(index>=w->count)GF_AssertFail();if(w->entries[index].active)GF_AssertFail();MI_CpuFill8(&w->entries[index],0,sizeof(PicEntry));w->entries[index].active=1;w->entries[index].pic=pic;if(member>=143){member=0;value=0;}w->entries[index].unk14=member;w->entries[index].enabled=w->flag?in->unk_4:0;w->entries[index].data=AllocAtEndAndReadWholeNarcMemberByIdPair((NarcId)90,w->entries[index].unk14,w->heapId);w->entries[index].cursor=w->entries[index].data;w->entries[index].done=0;w->entries[index].state=0;w->entries[index].flag1CD=0;w->entries[index].mode1CE=28;w->entries[index].rest1CF=0;w->entries[index].task=SysTask_CreateOnMainQueue(sub_020170C4,&w->entries[index],0);w->entries[index].delay=value;w->entries[index].originX=Pokepic_GetAttr(pic,0);w->entries[index].originY=Pokepic_GetAttr(pic,1);w->entries[index].clearWords[0]=0;w->entries[index].clearWords[1]=0;w->entries[index].clearWords[2]=0;w->entries[index].clearWords[3]=0;w->entries[index].clearWords[4]=0;w->entries[index].clearWords[5]=0;w->entries[index].clearWords[6]=0;}
