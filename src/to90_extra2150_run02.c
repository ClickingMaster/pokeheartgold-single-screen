#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u32 x[8];u16 sector,pad;} FlashTask;
u32 CTRDG_WriteAgbFlashSector(u16,const void*);u32 CTRDG_VerifyAgbFlash(u16,const void*,u32);u32 CTRDGi_WriteAndVerifyAgbFlashSectorCore(FlashTask*);
u32 CTRDGi_WriteAndVerifyAgbFlashSectorCore(FlashTask*arg){FlashTask *local=arg;FlashTask t=*local;u16 i=0,sec=t.sector;const void*src=(void*)t.x[3];u32 n=t.x[6],ret;do{ret=CTRDG_WriteAgbFlashSector(sec,src);if(!ret){ret=CTRDG_VerifyAgbFlash(sec,src,n);if(!ret)return ret;}i++;}while(i<3);return ret;}
