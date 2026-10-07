#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct {u16 prefix[8],action,channel;} W40817Request;
typedef struct {u16 command,length;} W40817Reply;
typedef struct {u16 prefix[4],state,unused,mode;} W40817Station;
extern int WSetChannel(u32,u32),WSetForcePowerState(u32);extern void WStart(void),WStop(void);
int DEV_TestRxReqCmd(W40817Request*,W40817Reply*);
int DEV_TestRxReqCmd(W40817Request *a,W40817Reply *out)
{
 W40817Station *p=(W40817Station *)(*(u8 *volatile *)0x0380fff4+0x344);
 u32 state;
 out->length=1;
 state=p->state;
 if((state&0xf0)!=16) return 1;
 if(a->action!=0) {
  if(a->action==1) {
   if(state!=16) return 1;
   WSetChannel(a->channel,1);
   p->mode=0;
   WStart();
   WSetForcePowerState(0x8000);
   p->state=17;
  }
 } else {
  if(state==17) {
   WSetForcePowerState(0);
   WStop();
  } else {
   return 1;
  }
  p->state=16;
 }
 return 0;
}
