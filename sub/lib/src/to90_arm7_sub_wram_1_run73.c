#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#define HW16(a) (*(volatile u16*)(a))
#define HW32(a) (*(volatile u32*)(a))
#define HW8(a) (*(volatile u8*)(a))
#define U32(p,o) (*(u32*)((u8*)(p)+(o)))
#define U8(p,o) (*((u8*)(p)+(o)))
typedef struct W35244Param{u32 count;const u8 *source;u8 *destination;u32 verified;} W35244Param;
typedef struct W35244Status{u32 checked;W35244Param param;} W35244Status;
#define status_checked (*(W35244Status*)((u8*)&cardi_param-4))
extern W35244Param cardi_param;
extern u8 cardi_common[0x200];
extern u8 arm7_arg3983[],arm7_buf3759[],arm7_arg3806[];
void OS_Sleep(u32);u64 OS_GetTick(void);u32 OS_DisableInterrupts(void);void OS_RestoreInterrupts(u32);void OS_WakeupThread(void*);void OS_WakeupThreadDirect(void*);
u8 CARDi_CommandReadStatus(void);void CARDi_CommandEnd(s32,s32);int CARDi_WaitPrevCommand(void);void CARDi_WriteEnable(void);void CARDi_SendSpiAddressingCommand(u32,u32);void CARDi_SetWriteProtectCore(u8);
void CARDi_CommReadCore(W35244Param*);void CARDi_CommWriteCore(W35244Param*);void CARDi_CommVerifyCore(W35244Param*);
void CARDi_CommArray(const void*,void*,u32,void(*)(W35244Param*));void CARDi_SetRomOp(u32,u32);u32 CARDi_GetRomFlag(u32);

int CARDi_WaitPrevCommand(void);
// @fn CARDi_WaitPrevCommand
int CARDi_WaitPrevCommand(void){void*p;CARDi_CommandEnd(0,50);p=(void*)U32(cardi_common,0);if(U32(p,0)==4){U32(p,0)=6;return 0;}return 1;}
