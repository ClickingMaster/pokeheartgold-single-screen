#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
void SND_Enable(void);
void SND_StopChannel(int,int);void SND_Shutdown(void);
void __VENEER_SVC_ResetSoundBias(u32);void OS_SpinWait(u32);void PMi_ResetControl(u32);void SND_BeginSleep(void);
// @fn SND_Enable
void SND_Enable(void){*(volatile u8*)0x04000501|=0x80;}
// @fn SND_Shutdown
void SND_Shutdown(void){int channel=0;*(volatile u8*)0x04000501&=~0x80;for(;channel<16;channel++)SND_StopChannel(channel,1);*(volatile u8*)0x04000508=0;*(volatile u8*)0x04000509=0;}
// @fn SND_BeginSleep
void SND_BeginSleep(void){*(volatile u8*)0x04000501&=~0x80;__VENEER_SVC_ResetSoundBias(128);OS_SpinWait(0x40000);PMi_ResetControl(1);*(volatile u16*)0x04000304&=~1;}
