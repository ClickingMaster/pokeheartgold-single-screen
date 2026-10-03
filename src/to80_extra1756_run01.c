#pragma force_active on
// clang-format off
#include "global.h"
#pragma force_active on
#include <nitro/code32.h>
// clang-format on
typedef struct{int fd;s16 events,revents;} Poll;
typedef struct{u8 pad[0x70];s16 flags;u8 pad2;s8 state;} Socket;
extern int SOCL_GetStatus(int),SOCL_SocketIsInvalid(Socket*),SOCLi_ExecEnableSslCommand(Socket*,void*);extern void OS_Sleep(u32);
int SOC_Poll(Poll*,u32,s64);int SOCL_EnableSsl(Socket*,void*);
int SOC_Poll(Poll *p,u32 n,s64 timeout){int finite=timeout!=-1;Poll*q;u32 i;int hits;for(;;){hits=0;q=p;for(i=0;i<n;){int mask=q->events|0xe0;int status=SOCL_GetStatus(q->fd)&mask;q->revents=status;i++;q++;if(status)hits++;}if(hits>0||(finite&&timeout<=0))break;OS_Sleep(1);timeout-=523;}return hits;}
