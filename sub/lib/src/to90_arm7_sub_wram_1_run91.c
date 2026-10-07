#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma dollar_identifiers on
typedef struct {u32 status,pad,id,source,destination,length;u8 p[0x58-0x18];u32 supported;} Request;
typedef struct Common {Request *request;u32 command;volatile u32 received;u8 p[0x3c-12];void (*callback)(void*);void *argument;void (*task)(struct Common*);u8 thread[0xec-0x48];void *wake;u32 pad;u32 queue[2];volatile u32 flags;} Common;
extern Common cardi_common;
extern u32 OS_DisableInterrupts(void);extern void OS_RestoreInterrupts(u32),OS_SleepThread(void*),OS_WakeupThread(void*),OS_WakeupThreadDirect(void*);
extern u32 CARDi_ReadRomIDCore(void);extern void CARDi_InitStatusRegister(void),CARDi_ReadBackupCore(u32,u32,u32),CARDi_WriteBackupCore(u32,u32,u32),CARDi_ProgramBackupCore(u32,u32,u32),CARDi_VerifyBackupCore(u32,u32,u32),CARDi_EraseBackupSectorCore(u32,u32),CARDi_EraseBackupSubSectorCore(u32,u32),CARDi_EraseChipCore(void),CARDi_SetWriteProtectCore(u32);extern u32 CARDi_CommandReadStatus(void);extern int PXI_SendWordByFifo(u32,u32,int);
void CARDi_TaskThread(void*arg);
/* Remote requests are dispatched only for commands 0 and 2..15. */
// @fn CARDi_TaskThread
void CARDi_TaskThread(void*arg){Common*c=&cardi_common;(void)arg;for(;;){int remote=0;u32 irq=OS_DisableInterrupts();for(;;){if(!(c->flags&4)){if(c->flags&16){remote=1;c->flags|=4;c->flags&=~16u;break;}}else if(c->flags&8)break;c->wake=c->thread;OS_SleepThread(0);}OS_RestoreInterrupts(irq);if(remote){c->request->status=0;{Request*r=c->request;u32 cmd=c->command;if(!(r->supported&(1u<<cmd)))r->status=3;else switch(cmd){case 0:case 1:break;case 2:CARDi_InitStatusRegister();break;case 3:c->request->id=CARDi_ReadRomIDCore();break;case 4:r->status=3;break;case 6:CARDi_ReadBackupCore(r->source,r->destination,r->length);break;case 7:CARDi_WriteBackupCore(r->destination,r->source,r->length);break;case 8:CARDi_ProgramBackupCore(r->destination,r->source,r->length);break;case 9:CARDi_VerifyBackupCore(r->destination,r->source,r->length);break;case 11:CARDi_EraseBackupSectorCore(r->destination,r->length);break;case 15:CARDi_EraseBackupSubSectorCore(r->destination,r->length);break;case 12:CARDi_EraseChipCore();break;case 13:*(u8*)c->request->destination=(u8)CARDi_CommandReadStatus();break;case 14:CARDi_SetWriteProtectCore(*(u8*)r->source);break;default:r->status=3;break;}}while(PXI_SendWordByFifo(11,1,1)<0){}irq=OS_DisableInterrupts();c->flags&=~0x4cu;OS_WakeupThread(c->queue);if(c->flags&16)OS_WakeupThreadDirect(c->thread);OS_RestoreInterrupts(irq);}else c->task(c);}}
