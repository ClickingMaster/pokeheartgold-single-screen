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
void CTRDG_ReadAgbFlash(u16 sec,u32 off,void*d,u32 n){FlashTask t;t.sector=sec;t.x[5]=off;t.x[7]=(u32)d;t.x[6]=n;CTRDGi_ReadAgbFlashCore(&t);}
u32 CTRDG_WriteAgbFlashSector(u16 sec,const void*s){return ((u32(*)(u16,const void*))W(ctrdgi_flash_lock_id,4))(sec,s);}
u32 CTRDG_VerifyAgbFlash(u16 sec,const void*s,u32 n){FlashTask t;t.sector=sec;t.x[3]=(u32)s;t.x[6]=n;return CTRDGi_VerifyAgbFlashCore(&t);}
void CTRDG_WriteAndVerifyAgbFlashAsync(u16 sec,const void*s,u32 n,void*cb){FlashTask t;t.x[3]=(u32)s;t.sector=sec;t.x[6]=n;CTRDGi_SetTask(&t,CTRDGi_WriteAndVerifyAgbFlashSectorCore,cb);}
