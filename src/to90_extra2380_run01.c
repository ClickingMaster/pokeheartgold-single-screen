#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))

void ov68_021E7A18(void*,int),ov68_021E7898(void*,int),ov68_021E68D4(void*,int),ov68_021E67E0(void*),ov68_021E7910(void*),ov68_021E797C(void*,int),GridInputHandler_SetNextInput(void*,u8),ov68_021E73A4(void*,u8,int);int ov68_021E6BEC(void*);void ov68_021E773C(void*,int);
void PlaySE(int);
void ov68_021E773C(void*s,int value){if(value<=3){H(P(s,0),0x14)=value;if(H(P(s,0),0x16)+H(P(s,0),0x14)<B(s,0x1b8)){ov68_021E7A18(s,H(P(s,0),0x16));ov68_021E7898(s,1);ov68_021E68D4(s,ov68_021E6BEC(s));}else{ov68_021E7A18(s,5);ov68_021E7898(s,0);ov68_021E68D4(s,-2);}}else if(value==4){PlaySE(1501);void*p=P(s,0);value=H(p,0x14);H(p,0x16)++;ov68_021E68D4(s,ov68_021E6BEC(s));ov68_021E67E0(s);ov68_021E7A18(s,5);ov68_021E7898(s,0);ov68_021E7910(s);ov68_021E797C(s,1);GridInputHandler_SetNextInput(P(s,0x1c8),(u8)value);}else if(value==5){PlaySE(1501);void*p=P(s,0);value=H(p,0x14);H(p,0x16)--;ov68_021E68D4(s,ov68_021E6BEC(s));ov68_021E67E0(s);ov68_021E7A18(s,5);ov68_021E7898(s,0);ov68_021E7910(s);ov68_021E797C(s,-1);GridInputHandler_SetNextInput(P(s,0x1c8),(u8)value);}else if(value==6){ov68_021E7A18(s,5);ov68_021E7898(s,0);if(H(s,0x1cc)!=6){value=H(P(s,0),0x14);GridInputHandler_SetNextInput(P(s,0x1c8),(u8)value);}}else if(value==7){value=H(P(s,0),0x14);GridInputHandler_SetNextInput(P(s,0x1c8),(u8)value);}ov68_021E73A4(s,(u8)value,3);}
