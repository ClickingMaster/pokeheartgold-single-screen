#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct Receive {u8 pad[0xe0];u8 mutex[24];} Receive;
typedef struct Socket {u8 pad[0x64];Receive *receive;u8 pad68[8];volatile s16 state;s8 blocking,mode;} Socket;
int SOCL_SocketIsInvalid(Socket*);u32 OS_GetProcMode(void);int OS_TryLockMutex(void*);void OS_LockMutex(void*);void OS_UnlockMutex(void*);int SOCLi_ReadAndConsumeBuffer(Socket*,void*,int,void*,void*,int,int);int SOCL_ReadFrom(Socket*,void*,int,void*,void*,int);
typedef enum Flag {FLAG_NO,FLAG_YES} Flag;
static inline Flag IsCreated(Socket *s){Flag result=FLAG_NO;if(s){if(s->state&1)result=FLAG_YES;}return result;}
int SOCL_ReadFrom(Socket *socket,void *buffer,int length,void *port,void *address,int flags){Receive *receive;int block;if(SOCL_SocketIsInvalid(socket))return -28;if((flags&4)||!socket->blocking){if(socket->mode==4)return -28;block=0;}else{if(OS_GetProcMode()==0x12)return -28;block=1;}if(IsCreated(socket)==0)return -39;int stream=1;if(socket->mode!=0&&socket->mode!=4)stream=0;if(stream){if(!(socket->state&4)||(socket->state&8))return -56;}receive=socket->receive;if(!(block&1)){if(!OS_TryLockMutex(receive->mutex))return -6;}else OS_LockMutex(receive->mutex);int result=SOCLi_ReadAndConsumeBuffer(socket,buffer,length,port,address,block,flags);OS_UnlockMutex(receive->mutex);return result;}
