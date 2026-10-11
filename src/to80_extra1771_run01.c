#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
extern int PCBox_UpdatePartyPanelSlide(void*);extern void PCBox_UpdateActionMenuSlide(void*);int ov14_021E952C(void*);
int ov14_021E952C(void*w){if(!PCBox_UpdatePartyPanelSlide(w))return 0;PCBox_UpdateActionMenuSlide(P(P(w,0x34),0x2f0));return 1;}
