#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
void NVRAM_ReadSiliconId(u8*);
// @fn NVRAM_ReadSiliconId
void NVRAM_ReadSiliconId(u8*out){volatile u16*c=(volatile u16*)0x040001C0,*d;while(*c&128){}d=(volatile u16*)0x040001C2;*c=0x8900;*d=159;c=d-1;while(*c&128){}d=(volatile u16*)0x040001C2;*d=0;c=d-1;while(*c&128){}d=(volatile u16*)0x040001C2;out[0]=(u8)*d;*c=0x8100;*d=0;c=d-1;while(*c&128){}out[1]=(u8)*(volatile u16*)0x040001C2;}
