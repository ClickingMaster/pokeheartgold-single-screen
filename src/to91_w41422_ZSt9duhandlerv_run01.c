// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern void _ZSt9terminatev(void);
void _ZSt9duhandlerv(void);
void _ZSt9duhandlerv(void){_ZSt9terminatev();}
