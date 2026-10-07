#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u8 pad[0x48];int capacity;} SenderState;
typedef struct {u8 pad[0x10c];SenderState *state;} Sender;
typedef struct{u8 p[0x68];Sender*sender;u8 q[4];s16 flags;s8 block,mode;}Socket;
extern int SOCLi_AllocWriteBuffer(Socket*,int,int,int*,int),SOCLi_ExecWriteCommand(Socket*,const void*,int,int,u16,u32,int);
int SOCLi_WriteBuffer(Socket*,const void*,int,u16,u32,int);
int SOCLi_WriteBuffer(Socket*s,const void*data,int len,u16 port,u32 addr,int block){int buffer;int maximum,count;u32 done;Sender *sender=s->sender;s8 mode=s->mode;SenderState *senderState=sender->state;int capacity=senderState->capacity;done=0;if(mode==1){if(len>(s32)((u32)capacity-42U))return -35;maximum=len;}else{maximum=(s32)((u32)capacity-54U);if(len<=maximum)maximum=len;}while(len>0){count=SOCLi_AllocWriteBuffer(s,len,maximum,&buffer,block);if(count>0){if(SOCLi_ExecWriteCommand(s,data,count,buffer,port,addr,block)<=0)return -6;data=(const u8*)data+count;len=(s32)((u32)len-(u32)count);done+=(u32)count;}if(!(block&1)){if(count>0)break;return -6;}}return (s32)done;}
