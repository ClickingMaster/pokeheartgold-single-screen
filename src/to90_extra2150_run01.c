#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u32 x[8];u16 sector,pad;}FlashTask;
typedef struct{u32 size,pad;u16 shift;}Flash;
extern u16 ctrdgi_flash_lock_id;extern Flash *AgbFlash;
extern void CTRDGi_SetFlashBankMx(u16);extern u32 CTRDGi_VerifyFlashSectorCore(const void*,const void*,u32);
u32 CTRDGi_VerifyAgbFlashCore(FlashTask*);
u32 CTRDGi_VerifyAgbFlashCore(FlashTask*arg){FlashTask t=*arg;u32 old;u16 sector=t.sector;const void*src=(void*)t.x[3];u32 size=t.x[6],result;OS_LockCartridge(ctrdgi_flash_lock_id);old=*(vu16*)0x04000204&3;*(vu16*)0x04000204=(*(vu16*)0x04000204&~3)|3;if(AgbFlash->size==0x20000){CTRDGi_SetFlashBankMx(sector>>4);sector&=15;}result=CTRDGi_VerifyFlashSectorCore(src,(const void*)(0x0a000000+(sector<<AgbFlash->shift)),size);*(vu16*)0x04000204=(*(vu16*)0x04000204&~3)|old;OS_UnlockCartridge(ctrdgi_flash_lock_id);return result;}
