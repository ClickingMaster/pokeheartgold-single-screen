#pragma force_active on
#include "global.h"
typedef struct {void*save;u8 pad[5];u8 id,pad2,mode;void*other;} Work;
int ov90_0225886C(Work*,u32);void*Save_PlayerData_GetProfile(void*);void*sub_02034818(u32);void*ov90_022588CC(Work*,u32);
void*ov90_022588CC(Work*w,u32 idx){if(idx>=4){if(idx>=4)GF_AssertFail();return 0;}if(w->mode==1){int result=ov90_0225886C(w,idx);return result==w->id ? Save_PlayerData_GetProfile(w->save):sub_02034818(idx);}else{if(!w->other)GF_AssertFail();return ((void**)((u8*)w->other+0x20))[idx];}}
