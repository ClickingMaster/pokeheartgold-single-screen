#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct Work Work;
struct Work { u8 *pc;void *obj;u32 flags;u8 pad0[0x28];void (*callback)(Work *);u8 pad1[0x5e];u8 phase;u8 pad2[0x49];u32 scale,invscale;};
extern void NNS_G3dGeSendDL(const void *,u32);
extern void NNS_G3dGeBufferOP_N(u32,const u32 *,u32);
void NNSi_G3dFuncSbc_CALLDL(Work *,u32);
void NNSi_G3dFuncSbc_POSSCALE(Work *,u32);
void NNSi_G3dFuncSbc_CALLDL(Work *w,u32 opt) {
 int phase=w->callback?w->phase:0;
 u32 skip;
 if(phase==1) {w->flags&=~0x40;w->callback(w);phase=w->callback?w->phase:0;skip=w->flags&0x40;}else skip=0;
 if(!(w->flags&0x100) && !skip) {
  u8 *p=w->pc;
  u32 off=p[1]|p[2]<<8|p[3]<<16|p[4]<<24;
  u32 len=p[5]|p[6]<<8|p[7]<<16|p[8]<<24;
  NNS_G3dGeSendDL(p+off,len);
 }
 if(phase==3) {w->flags&=~0x40;w->callback(w);}
 w->pc+=9;
}
void NNSi_G3dFuncSbc_POSSCALE(Work *w,u32 opt) {
 if(!(w->flags&0x100) && !(w->flags&0x200)) {
  u32 values[3];
  if(opt==0) {values[0]=values[1]=values[2]=w->scale;}
  else {values[0]=values[1]=values[2]=w->invscale;}
  NNS_G3dGeBufferOP_N(0x1b,values,3);
 }
 w->pc++;
}
