#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{u8 pad[0x81c];u32 counter;} Work;extern char ov45_022565C0[],ov45_022565D0[],ov45_02255E60[];void __msl_assertion_failed(const char*,const char*,const char*,int);u32 ov45_0224A5E4(Work*);
u32 ov45_0224A5E4(Work*w){if(!w)__msl_assertion_failed(ov45_022565C0,ov45_022565D0,ov45_02255E60,813);u32 old=w->counter;u32 value;if(old==0x7fffffff)value=1;else value=old+1;w->counter=value;return old;}
