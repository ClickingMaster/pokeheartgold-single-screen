#pragma force_active on
#include <nitro/types.h>
#include <nitro/os/message.h>
#include <stdarg.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct {u32 module,command,args[4];} SpiCommand;
typedef struct {u32 delay,priority;OSThread thread;u8 stack[512];OSMessageQueue queue;OSMessage messages[16];SpiCommand commands[16];u32 next,lockOwner,lockCount,reserved;} SpiWork;
typedef char CheckThread[(sizeof(OSThread)==0xa4)?1:-1];
typedef char CheckQueue[(sizeof(OSMessageQueue)==0x20)?1:-1];
typedef char CheckWork[(sizeof(SpiWork)==0x49c)?1:-1];
extern SpiWork spiWork;
int SPIi_SetEntry(u32,u32,u16,...);
#define va_arg(ap,type) (*(type*)((ap += sizeof(type))-sizeof(type)))
int SPIi_SetEntry(u32 module,u32 command,u16 count,...){va_list args;OSIntrMode irq;u32 index;int i;if(count>4)return 0;irq=OS_DisableInterrupts();spiWork.commands[spiWork.next].module=module;spiWork.commands[spiWork.next].command=command;va_start(args,count);for(i=0;i<count;i++)spiWork.commands[spiWork.next].args[i]=va_arg(args,u32);index=spiWork.next;spiWork.next=(index+1)&15;OS_RestoreInterrupts(irq);va_end(args);return OS_SendMessage(&spiWork.queue,&spiWork.commands[index],0);}
