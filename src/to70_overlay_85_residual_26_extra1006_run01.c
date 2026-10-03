#pragma force_active on
#include "global.h"
#include <string.h>
extern void ov85_021E7E18(void*,void*);extern void*SysTask_CreateOnMainQueue(void(*)(void*,void*),void*,int);typedef struct{u8 pad[0xdb4];void*volatile task;}Work;

void ov85_021E7E3C(Work*w);

void ov85_021E7E3C(Work*w){memset((u8*)w+0xb54,0,100);w->task=SysTask_CreateOnMainQueue(ov85_021E7E18,w,257);if(w->task==0)GF_AssertFail();}
