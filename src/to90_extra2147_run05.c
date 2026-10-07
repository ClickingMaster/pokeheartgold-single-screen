#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u32 flags;u16 min,max;u8 rest[8];}Parameters;extern int WMi_StartMP(void*,void*,u32,void*,u16,Parameters*);int WM_StartMP(void*,void*,u32,void*,u16,u16);
int WM_StartMP(void*callback,void*recv,u32 recvSize,void*send,u16 sendSize,u16 frequency){Parameters p;MI_CpuClear32(&p,sizeof(p));p.flags=3;p.min=frequency;p.max=frequency;return WMi_StartMP(callback,recv,recvSize,send,sendSize,&p);}
