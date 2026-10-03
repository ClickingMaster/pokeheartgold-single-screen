#pragma force_active on
#include "global.h"
#pragma force_active on
extern struct{u8 pad[0x48];u32 keys;}gSystem;
void ov43_0222A320(u32*p);
void ov43_0222A320(u32*p){if(gSystem.keys&0xcf3)*p=0;}
