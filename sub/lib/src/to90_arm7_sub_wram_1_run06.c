#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int sMasterPan;
extern int sSurroundDecay;
extern u8 sOrgPan[];
extern u8 sOrgVolume[];
extern int CalcSurroundDecay(int, int);
#define R8(c, o)  (*(volatile u8 *)(0x04000000u + ((u32)(c) << 4) + (o)))
#define R16(c, o) (*(volatile u16 *)(0x04000000u + ((u32)(c) << 4) + (o)))
#define R32(c, o) (*(volatile u32 *)(0x04000000u + ((u32)(c) << 4) + (o)))
extern u8 sndAlarm[];
u64 OS_GetTick(void);
void SndAlarmCallback(void *);
void OS_SetPeriodicAlarm(void *, u64, u64, void (*)(void *), void *);
void SND_StartIntervalTimer(void);

void SND_StartIntervalTimer(void) {
    OS_SetPeriodicAlarm(sndAlarm, OS_GetTick() + 65536, 2728, SndAlarmCallback, 0);
}
