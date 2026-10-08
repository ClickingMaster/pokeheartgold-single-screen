// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Channel{u16 unused0;u8 type;struct{u8 active:1,other:7;}state;u8 unused4[31];u8 pan;u16 volume;u8 unused26[46];}Channel;
typedef struct Sound{Channel channels[16];u8 unused540[0xc40];u32 controls[16];u32 unused11c0,mask;}Sound;
#include <nitro/snd/common/channel.h>
#include <nitro/snd/common/exchannel.h>
#include <nitro/snd/common/work.h>
int SND_ReadChannelInfo(const Sound*,int,SNDChannelInfo*);
#define SW(amount) do{switch(amount){case 0:break;case 1:out->volume>>=1;break;case 2:out->volume>>=2;break;case 3:out->volume>>=4;break;}}while(0)
#pragma opt_propagation off
// @fn SND_ReadChannelInfo
int SND_ReadChannelInfo(const Sound*s,int index,SNDChannelInfo*out)
{
    if (index < 0 || index > 15)
    {
        return 0;
    }
    out->lockFlag = (s->mask & (1u << index)) ? 1 : 0;
    if (out->lockFlag)
    {
        u32 control = s->controls[index];
        out->activeFlag = (control & 0x80000000) ? 1 : 0;
        {
            int divider = (control & 0x300) >> 8;
            out->volume = (u8)(control & 127);
            out->volume <<= 4;
            SW(divider);
        }
        u32 pan = (control & 0x7f0000) >> 16;
        out->pan = pan;
    }
    else
    {
        const Channel * channel = &s->channels[index];
        int encoded, divider;
        out->activeFlag = channel->state.active;
        out->envStatus = (SNDEnvStatus)channel->type;
        encoded = channel->volume;
        divider = encoded >> 8;
        out->volume = encoded & 255;
        out->volume <<= 4;
        SW(divider);
        out->pan = channel->pan;
    }
    return 1;
}
