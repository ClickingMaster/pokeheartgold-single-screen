#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern int sMasterPan,sSurroundDecay;
extern u8 sOrgPan[],sOrgVolume[];
extern int CalcSurroundDecay(int,int);
#define R8(c,o) (*(volatile u8 *)(0x04000000u+((u32)(c)<<4)+(o)))
#define R16(c,o) (*(volatile u16 *)(0x04000000u+((u32)(c)<<4)+(o)))
#define R32(c,o) (*(volatile u32 *)(0x04000000u+((u32)(c)<<4)+(o)))
extern u8 sndAlarm[];void OS_CancelAlarm(void*);
void SND_StopIntervalTimer(void);
// @fn SND_StopIntervalTimer
void SND_StopIntervalTimer(void){OS_CancelAlarm(sndAlarm);}
