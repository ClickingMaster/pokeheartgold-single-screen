#pragma force_active on
#include "global.h"
typedef struct {u32 a;u16 state;u8 p[0x2c-6];void*task;} Work;
extern void ov90_0225BEE0(void*,void*),ov90_0225C06C(void*,void*),PlaySE(int);extern void*SysTask_CreateOnMainQueue(void(*)(void*,void*),void*,int);
void ov90_02258C8C(Work*),ov90_02258CB0(Work*);
void ov90_02258C8C(Work*w){if(w->task)GF_AssertFail();w->task=SysTask_CreateOnMainQueue(ov90_0225BEE0,w,0);w->state=1;}
void ov90_02258CB0(Work*w){if(w->task)GF_AssertFail();w->task=SysTask_CreateOnMainQueue(ov90_0225C06C,w,0);w->state=1;PlaySE(1521);}
