#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 pad[0xf8];s32 size;u8*buffer;u16 write,read;OSThreadQueue queue;void*pipe;}Send;
typedef struct{u8 pad[0x48];s32 len;u8*buffer;u8 pad2[0x18];Send*send;u8 pad3[4];s16 flags;s8 unused,type;u16 local,remote;u32 ip;}Socket;
typedef struct{u32 unused;Socket*socket;u8 pad[5];u8 mode;u8 pad2[2];u8*first;int firstLen;u8*second;int secondLen;u16 write,pad3,local,remote;u32 ip;}Packet;
int SOCLi_GetWriteBufferFreeSize(Socket*);int SOCLi_ExecWriteCommand(Socket*,void*,int,int,u16,u32,int);int SOCLi_WriteCallBack(Packet*);
extern Packet*SOCLi_CreateCommandPacket(int(*)(Packet*),void*,int);extern int SOCLi_ExecCommandPacketInSendPipe(void*,Packet*);extern u16 CPS_SocGetEport(void);extern void CPS_SocBind(u16,u16,u32);extern int SOCLi_GetOptimumSendBufLen(int),SOCLi_MemCpy(void*,int,Packet*),CPS_SocWrite(void*,int);extern void CPS_TcpClose(void);
int SOCLi_WriteCallBack(Packet*p){u8*buffer;Socket*s=p->socket;Send*w;int total=0,header,limit,result;w=s->send;if(((s->type==0||s->type==4)?1:0)&&!(s->flags&4)){goto error;}if(p->ip)CPS_SocBind(p->local,p->remote,p->ip);buffer=s->buffer;header=(s->type==0||s->type==4)?54:42;if(s->type!=0&&s->type!=4)limit=s->len-header;else limit=SOCLi_GetOptimumSendBufLen(s->len-header);for(;;){result=SOCLi_MemCpy(buffer+header,limit,p);if(result<=0)break;result=CPS_SocWrite(buffer+header,result);if(result<=0){if(((s->type==0||s->type==4)?1:0)){s->flags&=~14;s->flags|=128;CPS_TcpClose();}total=-76;break;}total+=result;}goto done;error:total=-76;done:w->read=p->write;OS_WakeupThread(&w->queue);return total;}
