#include "global.h"
#include <string.h>
#pragma force_active on
#include "communication_core_internal.h"
#include "overlay_37_work.h"
typedef Ov37Work W;
void ov37_021E755C(W*);
void ov37_021E755C(W*w){if(Comm_GetLocalNetId()==0){if(sub_0203783C()){w->unk_4381=w->unk_93F4;{u32 remaining;u8*destination;const u8*source;source=((const u8*)w+offsetof(W,unk_4378));destination=w->unk_93BD;for(remaining=10;remaining!=0;remaining--){*destination=*source;source++;destination++;}}sub_02037108((void *)120,w->unk_93BD,50);}}else{if(sub_02037854())Comm_SendCommand(119,((const u8*)w+offsetof(W,unk_4378)),10);}}
