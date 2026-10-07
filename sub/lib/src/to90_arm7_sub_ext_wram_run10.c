#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u8 wmspMPIntervalAlarm[],wmspMPAckAlarm[];
extern void __VENEER_OS_CancelAlarm(void*);
void WMSP_CancelAllAlarms(void);
// @fn WMSP_CancelAllAlarms
void WMSP_CancelAllAlarms(void){__VENEER_OS_CancelAlarm(wmspMPIntervalAlarm);__VENEER_OS_CancelAlarm(wmspMPAckAlarm);}
