#pragma force_active on
#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{u32 cleared[78];u32 zero;u8 pad[4];u8 flag;} Global;extern Global _0222AB84;extern u8 ov39_0222AD84[];extern void ov39_02229408(void),ov39_0222993C(void*),ov39_02229970(void);void ov39_02229310(void);
void ov39_02229310(void){ov39_02229408();memset(_0222AB84.cleared,0,sizeof(_0222AB84.cleared));_0222AB84.zero=0;_0222AB84.flag=0;*(u16*)(ov39_0222AD84+0x40)=0;ov39_0222993C(ov39_0222AD84);ov39_02229970();}
