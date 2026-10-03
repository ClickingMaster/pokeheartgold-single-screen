#pragma force_active on
#include "global.h"
#pragma force_active on
extern int TextPrinterCheckActive(u8);
int ov112_021EBE4C(u8*w);
int ov112_021EBE4C(u8*w){if(TextPrinterCheckActive((u8)*(u32*)(w+0x1e524))==0){return 9;}else{return 8;}}
