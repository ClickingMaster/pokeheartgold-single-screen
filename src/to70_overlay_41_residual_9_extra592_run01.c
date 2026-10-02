#pragma force_active on
#include "global.h"
void ov41_022463FC(void);void ov41_0224642C(void);
void ov41_022463FC(void){*(volatile u32*)0x04000000 &= 0xffff1fff;*(volatile u16*)0x04000008=(*(volatile u16*)0x04000008&~3)|1;*(volatile u16*)0x0400000a &= ~3;}
