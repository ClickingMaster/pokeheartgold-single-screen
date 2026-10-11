#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
void BattleBagMenu_SelectLastUsedItem(u8*w);
void BattleBagMenu_SelectLastUsedItem(u8*w){u8 index=B(w,0x114d);u8*args=P(w,0);u32 i=0;u8*entry=w+index*144;u16 value=H(args,0x20);
 for(;i<36;i++,entry+=4){if(value==H(entry,0x3c)){B(args+index,0x27)=i%6;B((u8*)P(w,0)+B(w,0x114d),0x2c)=i/6;return;}}
}
