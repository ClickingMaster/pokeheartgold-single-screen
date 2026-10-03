#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u8 p[0x68];void*sender;u8 q[4];volatile s16 flags;s8 block,mode;}Socket;
static inline int openSocket(Socket*s){int result=0;if(s&&(s->flags&1))result=1;return result;}
extern int SOCL_SocketIsInvalid(Socket*);
extern int SOCLi_AllocWriteBuffer(Socket*,int,int,void**,int),SOCLi_ExecWriteCommand(Socket*,void*,int,void*,int,int,int);
int SOCL_WriteTo(Socket*,void*,int,int,int,int);int SOCLi_WriteBuffer(Socket*,void*,int,int,int,int);
int SOCL_WriteTo(Socket*s,void*data,int len,int port,int addr,int flags){void*sender;int block,result,ok;if(SOCL_SocketIsInvalid(s))return -28;ok=0;if(s)ok=(s->flags&1)!=0;if(!ok)return -39;ok=1;if(s->mode!=0&&s->mode!=4)ok=0;if(ok){if(!(s->flags&4)||(s->flags&8))return -56;}sender=s->sender;if((flags&4)||s->block==0){if(!OS_TryLockMutex((OSMutex*)((u8*)sender+0xe0)))return -6;block=0;}else{OS_LockMutex((OSMutex*)((u8*)sender+0xe0));block=1;}result=SOCLi_WriteBuffer(s,data,len,port,addr,block);OS_UnlockMutex((OSMutex*)((u8*)sender+0xe0));return result;}
