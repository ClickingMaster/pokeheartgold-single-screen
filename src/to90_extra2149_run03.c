#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
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
u32 CTRDGi_ReadAgbFlashCore(FlashTask*arg){FlashTask *local=arg;FlashTask t=*local;u32 old;u16 sec=t.sector;u32 off=t.x[5];u8*d=(void*)t.x[7];u32 n=t.x[6];const u8*src;OS_LockCartridge(H(ctrdgi_flash_lock_id,0));old=*(vu16*)0x04000204&3;*(vu16*)0x04000204=(*(vu16*)0x04000204&~3)|3;if(W((void*)W(AgbFlash,0),0)==0x20000){CTRDGi_SetFlashBankMx(sec>>4);sec&=15;}src=(const u8*)(off+(0x0a000000+(sec<<H((void*)W(AgbFlash,0),8))));while(n--)*d++=*src++;*(vu16*)0x04000204=(*(vu16*)0x04000204&~3)|old;OS_UnlockCartridge(H(ctrdgi_flash_lock_id,0));return 0;}
