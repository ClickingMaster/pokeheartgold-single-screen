#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
u16 GetMatColAnmuAlphaValue_(const void*,u32,u32);
u16 GetMatColAnmuAlphaValue_(const void*resource,u32 flags,u32 frame){const u8*p;u32 last,first;if(flags&0x20000000)return(u16)flags;p=(const u8*)resource+(u16)flags;if(!(flags&0xc0000000))return p[frame];last=(flags&0x1fff0000)>>16;if(flags&0x40000000){if(frame&1){if(frame>last)return p[(last>>1)+1];return(u16)((p[frame>>1]+p[(frame>>1)+1])>>1);}return p[frame>>1];}if(frame&3){if(frame>last)return p[(last>>2)+(frame&3)];if(frame&1){if(frame&2){frame>>=2;first=frame+1;}else{first=frame>>2;frame=first+1;}{u32 a=p[first],b=p[frame];a*=3;return(u16)((a+b)>>2);}}return(u16)((p[frame>>2]+p[(frame>>2)+1])>>1);}return p[frame>>2];}
