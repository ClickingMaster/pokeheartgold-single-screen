#pragma force_active on
#include "global.h"
#pragma force_active on
#define I(p,o) (*(s32*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
extern int ov37_021E75C4(void),sub_02033250(void),sub_02037454(void);extern void sub_02038C1C(int),sub_0205A904(int),ov37_021E78A4(void*),Sprite_SetAnimCtrlSeq(void*,int),ov37_021E76D0(void*,int);
int ov37_021E76F0(void*);
#pragma opt_common_subs off
static inline BOOL W31712_Valid(u32 bits){BOOL result=FALSE;u32 key=bits-2;if(key<=14){if(0x4045u&(1u<<key))result=TRUE;}return result;}
int ov37_021E76F0(void*w){int value=ov37_021E75C4();switch(value){case 1:if(I(w,0x304)<=14){if(sub_02033250()!=1)I(w,0x9400)=1;I(w,0x308)=19;sub_02038C1C(1);if(I(w,0x93f8)){ov37_021E78A4(w);Sprite_SetAnimCtrlSeq(P(w,0x268),21);}return 2;}break;case 2:case 3:case 4:sub_0205A904(1);if(value<I(w,0x314)){if(I(w,0x93f4)==2)ov37_021E76D0(w,0);else ov37_021E76D0(w,1);}break;case 5:sub_0205A904(8);break;default:break;}if(value<I(w,0x314)){I(w,0x318)=sub_02037454();I(w,0x31c)=sub_02033250();if(I(w,0x9404)&&!(U(w,0x31c)&U(w,0x9408))){ov37_021E76D0(w,1);I(w,0x93f4)=1;I(w,0x9404)=0;I(w,0x9408)=0;}}I(w,0x314)=ov37_021E75C4();if(I(w,0x318)<sub_02037454()){I(w,0x93f4)=2;I(w,0x9404)=1;U(w,0x9408)=sub_02033250()^U(w,0x31c);GF_ASSERT(W31712_Valid(U(w,0x9408)));}return 1;}
