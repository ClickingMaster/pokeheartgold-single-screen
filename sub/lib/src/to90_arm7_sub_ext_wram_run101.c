#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern int __VENEER_OS_ReceiveMessage(void*,void**,int);extern void ExecuteMessage(void**),AddTask(u32,u32);
void LowestIdleTask(void);
// @fn LowestIdleTask
void LowestIdleTask(void){void*message;__VENEER_OS_ReceiveMessage(*(void**)(*(u8*volatile*)0x380fff4+0x308),&message,1);ExecuteMessage(&message);AddTask(3,12);}
