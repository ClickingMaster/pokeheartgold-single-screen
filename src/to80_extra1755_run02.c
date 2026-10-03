#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
s32 GetTexSRTAnmVectorVal_(const u8*,u32,u32,u32);
s32 GetTexSRTAnmVectorVal_(const u8*base,u32 flags,u32 data,u32 frame){u32 last;if(flags&0x20000000)return(s32)data;base+=data;if(!(flags&0xc0000000))goto direct;last=(u16)flags;if(flags&0x40000000){if(frame&1){if(frame>last){frame=(last>>1)+1;goto direct;}frame>>=1;goto average;}frame>>=1;goto direct;}else{u32 part=frame&3;if(part){if(frame>last){frame=(last>>2)+part;goto direct;}if(frame&1){u32 weighted;s32 a,b;if(frame&2){frame>>=2;weighted=frame+1;}else{weighted=frame>>2;frame=weighted+1;}if(!(flags&0x10000000)){a=((const s32*)base)[weighted];b=((const s32*)base)[frame];}else{a=((const s16*)base)[weighted];b=((const s16*)base)[frame];}a*=3;a+=b;return a>>2;}frame>>=2;goto average;}frame>>=2;}
direct:if(flags&0x10000000)return((const s16*)base)[frame];return((const s32*)base)[frame];
average:{s32 a,b;if(flags&0x10000000){a=((const s16*)base)[frame];b=((const s16*)base)[frame+1];}else{a=((const s32*)base)[frame];b=((const s32*)base)[frame+1];}return(a+b)>>1;}}
