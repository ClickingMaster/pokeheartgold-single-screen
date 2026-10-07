#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#pragma opt_propagation off
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define V16(a) (*(volatile u16*)(a))
extern void *AgbFlash;extern u16 ctrdgi_flash_lock_id;
int CTRDGi_VerifyFlashErase(void*,int(*)(void*));
int CTRDGi_VerifyFlashErase(void *addr,int(*fn)(void*)){int result;void *flash;u32 old;OS_LockCartridge(ctrdgi_flash_lock_id);old=V16(0x04000204)&3;flash=AgbFlash;{u32 bus=V16(0x04000204),cycles=W(flash,16);V16(0x04000204)=(bus&~3u)|cycles;}result=0;if(fn(addr))result=0x8004;V16(0x04000204)=(V16(0x04000204)&~3)|old;OS_UnlockCartridge(ctrdgi_flash_lock_id);return result;}
