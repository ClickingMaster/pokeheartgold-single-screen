#pragma force_active on
#include "global.h"
#pragma force_active on
#include <string.h>
typedef struct {u8 pad0[0xc30];u8 data[20];u8 padc44[0x178];void *task;} Work;
extern void ov85_021E80D0(void*,void*);
extern void *SysTask_CreateOnMainQueue(void(*)(void*,void*),void*,u32);
void ov85_021E80E0(Work*);
void ov85_021E80E0(Work *work) {memset(work->data,0,sizeof(work->data));work->task=SysTask_CreateOnMainQueue(ov85_021E80D0,work,255);GF_ASSERT(work->task);}
