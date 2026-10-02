#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
#pragma force_active on
typedef struct {u8 pad[0x18];void *(*alloc)(u32);void (*free)(void *);} Config;
extern Config *_02110628;
extern void CPS_SocRegister(void *),CPS_SocUnRegister(void);
extern u32 CPS_Resolve(const char *);
u32 SOCL_Resolve(const char *);
u32 SOCL_Resolve(const char *name){u32 socket[25];void *buf;u32 result;if(!name)return 0;buf=_02110628->alloc(0xfd0);if(!buf)return 0;MI_CpuFill8(socket,0,100);socket[16]=(u32)buf;socket[15]=0xb68;socket[19]=(u32)buf+0xb68;socket[18]=0x466;CPS_SocRegister(socket);result=CPS_Resolve(name);CPS_SocUnRegister();_02110628->free(buf);return result;}
