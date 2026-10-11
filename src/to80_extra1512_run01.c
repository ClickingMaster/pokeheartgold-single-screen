#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define PTR(p,o) ((void*)W(p,o))
void PCBox_DrawMarkingMenuEntry(void*,int);void BgCellManager_DrawCellAt(void*,int,int,int);
void PCBox_ToggleMarking(void*p,int bit);
void PCBox_ToggleMarking(void*p,int bit){void*q=PTR(p,52);W(q,0x40c0)^=1<<bit;PCBox_DrawMarkingMenuEntry(p,bit);BgCellManager_DrawCellAt(PTR(PTR(p,52),752),2,21,6);}
