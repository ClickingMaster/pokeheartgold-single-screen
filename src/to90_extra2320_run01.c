#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
int ov38_02220100(char*,const char*,int);
#pragma force_active on
#pragma opt_lifetimes on
#pragma opt_common_subs on
int ov38_02220100(char*out,const char*in,int size){u32 i;int count=0;i=count;for(;(s32)i<size;i++){if(*in!=0){char c=*in;count++;in++;*out++=c;}else *out++=0;}return count;}
