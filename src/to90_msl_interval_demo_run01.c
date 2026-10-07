#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern int sys_readc(void);
int __read_console(int,u8*,u32*,void*);
int __read_console(int handle,u8 *buf,u32 *length,void *data){u32 i,size=*length;for(i=0;i<size;i++){if((buf[i]=sys_readc())==13||buf[i]==10){*length=i+1;break;}}return 0;}
