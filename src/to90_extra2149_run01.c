#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
extern u8 ctrdgi_flash_lock_id[],AgbFlash[];
void StartFlashTimer(u16);u8 CTRDGi_ReadSrFunc(const u8*);BOOL CTRDG_IsExisting(void);u16 CTRDGi_PollingSR512kCOMMON(u16,const u8*,u8);
u16 CTRDGi_PollingSR512kCOMMON(u16 phase,const u8*p,u8 expected){u16 err=0;StartFlashTimer(phase);for(;;){u8 actual=CTRDGi_ReadSrFunc(p);if(actual==expected)break;if(!H(ctrdgi_flash_lock_id,2))continue;if(expected==CTRDGi_ReadSrFunc(p))break;if(H((void*)W(AgbFlash,0),24)==0xc2)*(vu8*)0x0a005555=0xf0;err=phase|0xc000;break;}if(!CTRDG_IsExisting())err=0x1000;return err;}
