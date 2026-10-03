#pragma force_active on
#include "global.h"
void ov41_0224642C(void);
void ov41_0224642C(void){*(volatile u32*)0x4000000=(*(volatile u32*)0x4000000&0xffff1fff)|0x2000;*(volatile u16*)0x4000048=(*(volatile u16*)0x4000048&~63)|31;*(volatile u16*)0x400004a=(*(volatile u16*)0x400004a&~63)|18;*(volatile u16*)0x4000040=0xaf6;*(volatile u16*)0x4000044=0x128f;*(volatile u16*)0x4000008=*(volatile u16*)0x4000008&~3;*(volatile u16*)0x400000a=(*(volatile u16*)0x400000a&~3)|1;}
