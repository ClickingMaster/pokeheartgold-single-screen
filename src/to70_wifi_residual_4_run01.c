#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct Receive {u8 pad[0xf8];s32 consumed;u16 threshold;} Receive;
typedef struct Socket {u8 pad[8];u8 state8;u8 pad9[0x5b];Receive *receive;u8 pad68[8];s16 flags;s8 blocking,mode;} Socket;
typedef struct Command {u32 pad;Socket *socket;u32 pad8[2];void *buffer;u32 length;void *port,*address;} Command;
void *CPS_SocRead(u32*);void OS_Sleep(u32);void MI_CpuCopy8(const void*,void*,u32);void CPS_SocConsume(u32);int SOCLi_CopyCPSBuffer(Socket*,void*,u32,void*,void*);void SOCLi_ConsumeCPSBuffer(Socket*);int SOCLi_ReadCallBack(Command*);
int SOCLi_ReadCallBack(Command *command){Socket *socket=command->socket;void *buffer=command->buffer;Receive *receive=socket->receive;u32 consumed=receive->consumed;void *port=command->port;u32 length=command->length;void *address=command->address;u32 available;void *data;for(;;){data=CPS_SocRead(&available);if(data==0)break;if((s32)(available-consumed)>0)break;int stream=1;if(socket->mode!=0&&socket->mode!=4)stream=0;if(stream&&socket->state8!=4){data=0;break;}OS_Sleep(10);}if(socket->flags&0x80)return -15;if(socket->mode==4){if(data==0)return 0;if(length>available)length=available;MI_CpuCopy8(data,buffer,length);CPS_SocConsume(length);return length;}int result;if(data==0)result=0;else result=SOCLi_CopyCPSBuffer(socket,buffer,length,port,address);if(result<=0)return result;if(receive->consumed>=receive->threshold)SOCLi_ConsumeCPSBuffer(socket);return result;}
