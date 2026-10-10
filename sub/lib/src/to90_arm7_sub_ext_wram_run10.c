#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u8 wmspMPIntervalAlarm[];
extern u8 wmspMPAckAlarm[];
extern void __VENEER_OS_CancelAlarm(void *);
void WMSP_CancelAllAlarms(void);

void WMSP_CancelAllAlarms(void) {
    __VENEER_OS_CancelAlarm(wmspMPIntervalAlarm);
    __VENEER_OS_CancelAlarm(wmspMPAckAlarm);
}
