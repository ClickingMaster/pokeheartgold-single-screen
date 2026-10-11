#pragma force_active on
#include "global.h"
#include "heap.h"
#include "sys_task.h"
extern SysTask *SysTask_CreateOnMainQueue(void (*)(SysTask *,void *),void *,u32);
extern int ov07_02232608(void *),ov07_02234530(void *);
extern void ParticleSystem_CreateEmitterWithParam(void *,int,int,void *),ParticleSystem_SetPerspectiveType(void *,int),ov07_02232C64(void),ov07_02232AC0(SysTask *,void *);
typedef struct {int index,value;void *object;int *done;void *a,*b;void (*callback)(void);} Job;
void ov07_02232AFC(void *,int);
#pragma force_active on
void ov07_02232AFC(void *argument,int value){
 Job *job;u8 *work=argument,*cursor;int i,*done;
 if(!*(u32 *)(work+0x94)){int count=ov07_02232608(*(void **)(work+0x98));for(i=0;i<count;i++)ParticleSystem_CreateEmitterWithParam(*(void **)(work+0x14),i,value,work);*(int *)(work+0x5c)=1;ParticleSystem_SetPerspectiveType(*(void **)(work+0x14),1);return;}
 i=0;if(*(int *)(work+0x10)>0){done=(int *)work;cursor=work;done=(int *)((u8 *)done+0x5c);do{
 job=Heap_Alloc((enum HeapID)*(u32 *)work,0x1c);GF_ASSERT(job!=NULL);job->index=i;job->object=*(void **)(work+4);job->callback=ov07_02232C64;*(int *)(cursor+0x5c)=0;job->done=done;job->a=*(void **)(cursor+0x14);job->b=*(void **)(cursor+0x38);GF_ASSERT(job->b!=NULL);job->value=ov07_02234530(job->b);SysTask_CreateOnMainQueue(ov07_02232AC0,job,1001);
 i++;cursor+=4;done++;}while(i<*(int *)(work+0x10));}
}
