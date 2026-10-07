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
typedef struct W35254Param{u32 count;const u8 *source;u8 *destination;u32 verified;} W35254Param;
typedef struct W35254Status{u32 checked;W35254Param param;} W35254Status;
extern W35254Status status_checked$3825;
extern W35254Param cardi_param;
extern u8 cardi_common[0x200];
extern u8 arg$3983[],buf$3759[],arg$3806[];
void OS_Sleep(u32);u64 OS_GetTick(void);u32 OS_DisableInterrupts(void);void OS_RestoreInterrupts(u32);void OS_WakeupThread(void*);void OS_WakeupThreadDirect(void*);
u8 CARDi_CommandReadStatus(void);void CARDi_CommandEnd(s32,s32);int CARDi_WaitPrevCommand(void);void CARDi_WriteEnable(void);void CARDi_SendSpiAddressingCommand(u32,u32);void CARDi_SetWriteProtectCore(u8);
void CARDi_CommReadCore(W35254Param*);void CARDi_CommWriteCore(W35254Param*);void CARDi_CommVerifyCore(W35254Param*);
void CARDi_CommArray(const void*,void*,u32,void(*)(W35254Param*));void CARDi_SetRomOp(u32,u32);u32 CARDi_GetRomFlag(u32);

void CARDi_WriteBackupCore(u32 address,const void*src,u32 size);
// @fn CARDi_WriteBackupCore
void CARDi_WriteBackupCore(u32 address,const void*src,u32 size){if(CARDi_WaitPrevCommand()){u32 mask;void*c=(void*)U32(cardi_common,0);u32 page=U32(c,0x24);mask=page-1;while(size){u32 chunk=page-(address&mask);if(chunk>size)chunk=size;CARDi_WriteEnable();status_checked$3825.param.count=U32(c,0x28)+1+chunk;CARDi_SendSpiAddressingCommand(address,10);CARDi_CommArray(src,0,chunk,CARDi_CommWriteCore);CARDi_CommandEnd((s32)U32(c,48),(s32)U32(c,52));if(U32(c,0))break;src=(const u8*)src+chunk;address+=chunk;size-=chunk;}}}
