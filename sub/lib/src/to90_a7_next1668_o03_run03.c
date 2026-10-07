#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
void SND_SetMasterVolume(s32 volume);
void SND_SetOutputSelector(u32,u32,u32,u32);
// @fn SND_SetMasterVolume
void SND_SetMasterVolume(s32 volume)
{
    *(volatile u8 *)0x04000500 = (u8)volume;
}
// @fn SND_SetOutputSelector
void SND_SetOutputSelector(u32 left,u32 right,u32 channel1,u32 channel3){u32 enabled=(*(volatile u8 *)0x04000501 & 128)?1:0;*(volatile u8 *)0x04000501=(u8)(left|((enabled<<7)|(channel3<<5)|(channel1<<4)|(right<<2)));}
