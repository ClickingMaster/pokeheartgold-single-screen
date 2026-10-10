#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u8 wmspMPIntervalAlarm[];
extern u8 wmspMPAckAlarm[];
extern void __VENEER_OS_CreateAlarm(void *);
void WMSP_InitAlarm(void);

void WMSP_InitAlarm(void) {
    __VENEER_OS_CreateAlarm(wmspMPIntervalAlarm);
    __VENEER_OS_CreateAlarm(wmspMPAckAlarm);
}
