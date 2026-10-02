#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
s64 FX_SinFx64c_internal(s64);s64 FX_CosFx64c_internal(s64);s64 FX_SinFx64c(s32);s64 FX_CosFx64c(s32);
static inline s64 Mul(s64 x,s64 y){return (s64)(((u64)x*(u64)y)>>32);}
s64 FX_SinFx64c(s32 x){if(x<0)return -FX_SinFx64c(-x);s64 angle=(s64)x*0x145f306ddLL;s32 sector=(s32)(angle>>44);u64 reduced=(u64)(angle>>12)&0xffffffffULL;if(sector&1)reduced=0x100000000ULL-reduced;s64 result;if((sector+1)&2)result=FX_CosFx64c_internal(reduced);else result=FX_SinFx64c_internal(reduced);if((sector&7)>3)return -result;return result;}
s64 FX_CosFx64c(s32 x){if(x<0)return FX_CosFx64c(-x);s64 angle=(s64)x*0x145f306ddLL;s32 sector=(s32)(angle>>44);u64 reduced=(u64)(angle>>12)&0xffffffffULL;if(sector&1)reduced=0x100000000ULL-reduced;s64 result;if((sector+1)&2)result=FX_SinFx64c_internal(reduced);else result=FX_CosFx64c_internal(reduced);if(((sector+2)&7)>3)return -result;return result;}
