#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#include "to91_arm7_rtc_work_private.h"
void RtcReturnResult(u32,u32);void RtcPxiCallback(PXIFifoTag,u32,BOOL);
// @fn RtcPxiCallback
void RtcPxiCallback(PXIFifoTag channel,u32 word,BOOL error){u16 command;if(error)return;command=(word&0x7f00)>>8;switch(command){case 0:case 1:case 16:case 17:case 18:case 19:case 20:case 21:case 22:case 23:case 24:case 25:case 35:case 36:case 37:case 38:case 39:case 40:case 41:if(rtcWork.busy)RtcReturnResult(command,3);else{rtcWork.busy=1;rtcWork.command=command;if(!OS_SendMessage(&rtcWork.queue,0,0))RtcReturnResult(command,4);}break;default:RtcReturnResult(command,1);break;}}
