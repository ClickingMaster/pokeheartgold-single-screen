#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u8 wmspMPIntervalAlarm[],wmspMPAckAlarm[];
extern void __VENEER_OS_CreateAlarm(void*);
void WMSP_InitAlarm(void);
// @fn WMSP_InitAlarm
void WMSP_InitAlarm(void){__VENEER_OS_CreateAlarm(wmspMPIntervalAlarm);__VENEER_OS_CreateAlarm(wmspMPAckAlarm);}
