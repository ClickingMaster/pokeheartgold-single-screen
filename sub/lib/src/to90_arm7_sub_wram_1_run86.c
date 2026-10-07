#pragma force_active on
#pragma dollar_identifiers on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#define HW16(a) (*(volatile u16*)(a))
#define HW32(a) (*(volatile u32*)(a))
#define HW8(a) (*(volatile u8*)(a))
#define U32(p,o) (*(u32*)((u8*)(p)+(o)))
#define U8(p,o) (*((u8*)(p)+(o)))
typedef struct W35261Param{u32 count;const u8 *source;u8 *destination;u32 verified;} W35261Param;
typedef struct W35261Status{u32 checked;W35261Param param;} W35261Status;
extern W35261Status status_checked$3825;
extern W35261Param cardi_param;
typedef struct W35261Context {u32 *result;u8 pad4[0x38];void(*callback)(void*);void *argument;u8 pad44[0xb0];u32 queue[2];u32 flags;} W35261Context;
extern W35261Context cardi_common;
extern u8 arg$3983[],buf$3759[],arg$3806[];
void OS_Sleep(u32);u64 OS_GetTick(void);u32 OS_DisableInterrupts(void);void OS_RestoreInterrupts(u32);void OS_WakeupThread(void*);void OS_WakeupThreadDirect(void*);
u8 CARDi_CommandReadStatus(void);void CARDi_CommandEnd(s32,s32);int CARDi_WaitPrevCommand(void);void CARDi_WriteEnable(void);void CARDi_SendSpiAddressingCommand(u32,u32);void CARDi_SetWriteProtectCore(u8);
void CARDi_CommReadCore(W35261Param*);void CARDi_CommWriteCore(W35261Param*);void CARDi_CommVerifyCore(W35261Param*);
void CARDi_CommArray(const void*,void*,u32,void(*)(W35261Param*));void CARDi_SetRomOp(u32,u32);u32 CARDi_GetRomFlag(u32);

void CARDi_ReadEnd(void);
#pragma opt_propagation off
// @fn CARDi_ReadEnd
void CARDi_ReadEnd(void){W35261Context*c=&cardi_common;u32 irq;void*argument;void(*callback)(void*);*c->result=0;callback=c->callback;argument=c->argument;irq=OS_DisableInterrupts();U32(c,0xfc)&=~0x4c;OS_WakeupThread((u8*)c+0xf4);if(U32(c,0xfc)&0x10)OS_WakeupThreadDirect((u8*)c+0x48);OS_RestoreInterrupts(irq);if(callback)callback(argument);}
