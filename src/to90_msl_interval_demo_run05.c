#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
char *_Z22__DecodeUnsignedNumberPcPm(char*,u32*);
char *_Z22__DecodeUnsignedNumberPcPm(char *p,u32 *out){u32 a=*(const u8*)p;u32 b,c,d;if(!(a&1)){*out=a>>1;return p+1;}b=(u8)p[1];if(!(a&2)){*out=((a>>2)<<8)|b;return p+2;}c=(u8)p[2];if(!(a&4)){*out=((a>>3)<<16)|(b<<8)|c;return p+3;}d=(u8)p[3];*out=((a>>3)<<24)|(b<<16)|(c<<8)|d;return p+4;}
