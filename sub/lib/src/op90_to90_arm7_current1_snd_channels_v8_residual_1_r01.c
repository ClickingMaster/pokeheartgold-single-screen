#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
extern int sMasterPan, sSurroundDecay;
extern u8 sOrgPan[], sOrgVolume[];
extern int CalcSurroundDecay(int, int);
void SND_SetupChannelPsg(int c, int duty, int volume, int shift, int timer, int pan);
// @fn SND_SetupChannelPsg
void SND_SetupChannelPsg(int c, int duty, int volume, int shift, int timer, int pan)
{
    u32 channelOffset = (u32)c << 4;
    {
        int master = sMasterPan;
        sOrgPan[c] = pan;
        if (master >= 0)
            pan = master;
    }
    sOrgVolume[c] = volume;
    if (sSurroundDecay > 0 && (0xfff5u & (1u << c)))
        volume = CalcSurroundDecay(volume, pan);
    {
        u32 control = ((u32)duty << 24) | 0x60000000u | ((u32)pan << 16) | ((u32)shift << 8);
        *(volatile u32 *)(0x04000400u + channelOffset) = (u32)volume | control;
    }
    *(volatile u16 *)(0x04000408u + channelOffset) = 0x10000u - (u32)timer;
}
