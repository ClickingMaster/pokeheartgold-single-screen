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
typedef struct W35251Param{u32 count;const u8 *source;u8 *destination;u32 verified;} W35251Param;
typedef struct W35251Status{u32 checked;W35251Param param;} W35251Status;
extern W35251Status status_checked$3825;
extern W35251Param cardi_param;
extern u8 cardi_common[0x200];
extern u8 arg$3983[],buf$3759[],arg$3806[];
void OS_Sleep(u32);u64 OS_GetTick(void);u32 OS_DisableInterrupts(void);void OS_RestoreInterrupts(u32);void OS_WakeupThread(void*);void OS_WakeupThreadDirect(void*);
u8 CARDi_CommandReadStatus(void);void CARDi_CommandEnd(s32,s32);int CARDi_WaitPrevCommand(void);void CARDi_WriteEnable(void);void CARDi_SendSpiAddressingCommand(u32,u32);void CARDi_SetWriteProtectCore(u8);
void CARDi_CommReadCore(W35251Param*);void CARDi_CommWriteCore(W35251Param*);void CARDi_CommVerifyCore(W35251Param*);
void CARDi_CommArray(const void*,void*,u32,void(*)(W35251Param*));void CARDi_SetRomOp(u32,u32);u32 CARDi_GetRomFlag(u32);

void CARDi_InitStatusRegister(void);
// @fn CARDi_InitStatusRegister
void CARDi_InitStatusRegister(void){u8 protection=U8((void*)U32(cardi_common,0),0x54);if(protection!=255&&!status_checked$3825.checked){if(protection!=CARDi_CommandReadStatus())CARDi_SetWriteProtectCore(protection);status_checked$3825.checked=1;}}
