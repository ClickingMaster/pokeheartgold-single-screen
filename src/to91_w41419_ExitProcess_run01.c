// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern void sys_exit(void);
void _ExitProcess(void);
void _ExitProcess(void){sys_exit();}
