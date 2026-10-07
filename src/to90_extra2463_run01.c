#pragma force_active on
#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u8 pad[0x68];u8 *pipe;u32 pad6c;volatile s16 flags;s8 mode,kind;} Sock;
extern int SOCL_SocketIsInvalid(Sock *),SOCLi_ShutdownCallBack(void *);
extern void *SOCLi_CreateCommandPacket(int (*)(void *),void *,s8);
extern int SOCLi_ExecCommandPacketInSendPipe(void *,void *);
#pragma opt_rebuildlogicals off
static inline int Active(Sock *s){int v=0;if(s){if(s->flags&1)v=1;}return v;}
int SOCL_Shutdown(Sock *);
int SOCL_Shutdown(Sock *s){int active;u8 *pipe;void *packet;if(SOCL_SocketIsInvalid(s))return -28;active=Active(s);if(!active)return -39;if(!(s->flags&4)||(s->flags&8))return -56;s->flags|=8;pipe=s->pipe;if(pipe&&*(void **)(pipe+0x10c)){packet=SOCLi_CreateCommandPacket(SOCLi_ShutdownCallBack,*(void **)(pipe+0x10c),s->mode);if(!packet)return -33;return SOCLi_ExecCommandPacketInSendPipe(*(void **)(pipe+0x10c),packet);}return 0;}
