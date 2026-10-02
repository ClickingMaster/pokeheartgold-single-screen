#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#pragma warn_uninitializedvar off
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
extern u8 ctrdgi_flash_lock_id[];
extern u8 AgbFlash[];
typedef struct {u32 x[8];u16 sector,pad;} FlashTask;
void CTRDGi_SetFlashBankMx(u16);
u8 CTRDGi_ReadSrFunc(const u8*);
u32 CTRDGi_VerifyFlashSectorCore(const u8*,const u8*,u32);
u32 CTRDGi_ReadAgbFlashCore(FlashTask*);
u32 CTRDGi_VerifyAgbFlashCore(FlashTask*);
u32 CTRDGi_WriteAndVerifyAgbFlashSectorCore(FlashTask*);
void CTRDGi_SetTask(FlashTask*,u32(*)(FlashTask*),void*);
BOOL CTRDG_IsExisting(void);
u16 CTRDGi_PollingSR512kCOMMON(u16,const u8*,u8);
void StartFlashTimer(u16);
void CheckFlashTimer(void);
void CTRDG_ReadAgbFlash(u16,u32,void*,u32);
u32 CTRDG_WriteAgbFlashSector(u16,const void*);
u32 CTRDG_VerifyAgbFlash(u16,const void*,u32);
void CTRDG_WriteAndVerifyAgbFlashAsync(u16,const void*,u32,void*);
void CTRDGi_SetFlashBankMx(u16 bank){*(vu8*)0x0a005555=0xaa;*(vu8*)0x0a002aaa=0x55;*(vu8*)0x0a005555=0xb0;*(vu8*)0x0a000000=bank;}
