// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef void (*PMCallback)(u32,void*);
typedef struct { u8 pad[0x20]; PMCallback callback; void *arg; } PMWork;
typedef struct { u16 done; u16 pad; u16 *buffer; } PMReg;
extern PMReg _021E3750[];
extern PMWork _021E370C;
extern int PMi_Lock(void);
extern void PMi_SendPxiData(u32);
int PMi_ReadRegisterAsync(u32,u16*,PMCallback,void*);
// @fn PMi_ReadRegisterAsync
int PMi_ReadRegisterAsync(u32 reg,u16 *buf,PMCallback cb,void *arg){
 if(!PMi_Lock())return 1;
 _021E370C.callback=cb;
 _021E370C.arg=arg;
 _021E3750[reg].done=0;
 _021E3750[reg].buffer=buf;
 PMi_SendPxiData(0x03006500|(reg&0xff));
 return 0;
}
