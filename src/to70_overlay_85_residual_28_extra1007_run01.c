#pragma force_active on
#include "global.h"
#include <string.h>
extern void ov85_021E7F50(void*,void*);extern void*SysTask_CreateOnMainQueue(void(*)(void*,void*),void*,int);typedef struct{u8 pad[0xdb8];void*volatile task;}Work;

void ov85_021E7F74(Work*w);

void ov85_021E7F74(Work*w){memset((u8*)w+0xbb8,0,120);w->task=SysTask_CreateOnMainQueue(ov85_021E7F50,w,258);if(w->task==0)GF_AssertFail();}
