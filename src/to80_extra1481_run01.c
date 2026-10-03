#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#pragma opt_strength_reduction off
void vct_mix_audio(s16*,s16*,s16*,u32);
static inline s32 clamp(s32 x){if(x>32767)x=32767;else if(x<-32768)x=-32768;return x;}
void vct_mix_audio(s16*a,s16*b,s16*out,u32 bytes){u32 n;u32 i=0;s32 x;n=bytes>>1;for(;i<n;i+=4){x=a[i];x+=b[i];if(x>32767)x=32767;else if(x<-32768)x=-32768;out[i]=x;x=a[i+1];x+=b[i+1];if(x>32767)x=32767;else if(x<-32768)x=-32768;out[i+1]=x;x=a[i+2];x+=b[i+2];if(x>32767)x=32767;else if(x<-32768)x=-32768;out[i+2]=x;x=a[i+3];x+=b[i+3];if(x>32767)x=32767;else if(x<-32768)x=-32768;out[i+3]=x;}}
