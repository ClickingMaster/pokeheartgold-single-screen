#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
void sys_writec(const char*);int __write_console(int,const char*,u32*,void*);
int __write_console(int handle,const char*buffer,u32*count,void*ref){u32 i;u32 size=*count;for(i=0;i<size;i++)sys_writec(buffer+i);return 0;}
