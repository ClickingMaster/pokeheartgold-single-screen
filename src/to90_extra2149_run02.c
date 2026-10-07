#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern volatile u16 ctrdgi_flash_lock_id[];extern void StartFlashTimer(u16),CheckFlashTimer(void);extern u8 CTRDGi_ReadSrFunc(const u8*);extern BOOL CTRDG_IsExisting(void);
u16 CTRDGi_PollingSR1MCOMMON(u16,const u8*,u8);
u16 CTRDGi_PollingSR1MCOMMON(u16 phase,const u8*address,u8 value){u16 result=0;StartFlashTimer(phase);for(;;){u8 read;CheckFlashTimer();read=CTRDGi_ReadSrFunc(address);if(read==value)break;if(read&32){if(CTRDGi_ReadSrFunc(address)==value)break;result=phase|0xa000;*(volatile u8*)0x0a005555=0xf0;break;}if(ctrdgi_flash_lock_id[1]){if(CTRDGi_ReadSrFunc(address)==value)break;result=phase|0xc000;*(volatile u8*)0x0a005555=0xf0;break;}}if(!CTRDG_IsExisting())result=0x1000;return result;}
