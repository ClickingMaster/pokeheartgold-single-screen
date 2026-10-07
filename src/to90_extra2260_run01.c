#pragma force_active on
#include "global.h"
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define C(p,o) (*(s8*)((u8*)(p)+(o)))
extern void ov112_021F2204(void*,int,int),ov112_021F22B0(void*,u8,int),ov112_021F1624(void*,int,int);
void ov112_021F298C(void*);
typedef struct {unsigned pad:8,a:1,b:1,rest:22;} Flags;
#pragma force_active on
void ov112_021F298C(void*w){void*mon=P((u8*)w+B(w,0x13d)*4,0xc0);int score=0,part;int left,right;u32 total;
 ov112_021F2204(w,0,1);ov112_021F22B0(P(w,0x84),(u8)W(w,12),3);
 part=U(mon,0x7c)/100;if(part>655)part=655;score+=part;
 part=10*B(w,0x142);if(part>40)part=40;score+=part;
 part=10*B(w,0x141);if(part>40)part=40;score+=part;
 part=15*B(w,0x143);if(part>150)part=150;
 left=((Flags*)((u8*)w+0x144))->a;right=((Flags*)((u8*)w+0x144))->b;
 score+=part;total=score;total+=left*250;total+=right*250;
 if(total>=550)right=72;else if(total>=300)right=73;else if(total>=200)right=74;else if(total>=150)right=75;else if(total>=100)right=76;else if(total>=30)right=77;else right=78;
 if(W(w,0x130)<12)left=71;else left=90;ov112_021F1624(w,left,right);
}
