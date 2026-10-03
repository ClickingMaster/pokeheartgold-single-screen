#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 p[0x50d];u8 active;u8 q[0x18];u16 ending;u8 r[0xbc];u32 zero;} Work;
extern struct {u32 p[2];Work *work;} ov13_0224CF40;
extern void MBi_EndCommon(int);
extern int MBi_IsTaskAvailable(void),MBi_CallReset(void);
extern void MBi_OnReset(void);
extern void MBi_EndTaskThread(void (*)(void));
int MBi_CommEnd(void);
int MBi_CommEnd(void){int result=1;OSIntrMode mask=OS_DisableInterrupts();
 if(!ov13_0224CF40.work->active){MBi_EndCommon(0);goto done;}
 if(ov13_0224CF40.work->ending)goto done;
 ov13_0224CF40.work->zero=0;ov13_0224CF40.work->ending=1;
 if(MBi_IsTaskAvailable()){MBi_EndTaskThread(MBi_OnReset);result=0;}else result=MBi_CallReset();
 done:OS_RestoreInterrupts(mask);return result;
}
