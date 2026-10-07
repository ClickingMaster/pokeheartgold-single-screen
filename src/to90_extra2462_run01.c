#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u8 pad[8];u8 status;u8 pad9[0x67];volatile s16 flags;s8 mode,kind;} Sock;
extern int SOCL_SocketIsInvalid(Sock *),SOCLi_GetReadBufferOccpiedSize(Sock *),SOCLi_GetWriteBufferFreeSize(Sock *);
int SOCL_GetStatus(Sock *);
int SOCL_GetStatus(Sock *s) {
 int result=0;
 OSIntrMode irq;
 if(SOCL_SocketIsInvalid(s)) { result|=128; }
 else {
  int tcp;
  if(s->flags&64)result|=32;
  if(s->kind==1 || (s->flags&4)) {
   irq=OS_DisableInterrupts();
   if(SOCLi_GetReadBufferOccpiedSize(s)>0)result|=1;
   if(SOCLi_GetWriteBufferFreeSize(s)>0)result|=8;
   OS_RestoreInterrupts(irq);
  }
  tcp=1;if(s->kind!=0&&s->kind!=4)tcp=0;
  if(tcp) {
   if((s->flags&4)&&s->status!=4) {if(!(result&1))s->flags&=~6;}
   if(!(s->flags&2)){if(!(s->flags&4))result|=64;}
  }
 }
 return result;
}
