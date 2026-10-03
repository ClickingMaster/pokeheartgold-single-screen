#pragma force_active on
#include "global.h"
#define W(p,o) (*(u32*)((p)+(o)))
#define P(p,o) (*(void**)((p)+(o)))
extern void ov70_0223BC7C(u8*),ov70_0223B7CC(void*),ov70_0223B8E0(u8*),ov70_0223BAE0(u8*),ov70_0223B9C8(u8*),ov70_0223CB1C(void*,void*,void*),ov70_0223CC04(void*,void*,void*,u16),ov70_0223F1D8(void*,void*,s16,int,int,int),ov70_0223F2BC(void*,void*,s8,int,int,int,int),ov70_0223F370(void*,void*,int,int,int,int,int),ov70_0223F244(void*,void*,void*,u32,int,int,int),ov70_0223CCA4(void),BeginNormalPaletteFade(int,int,int,int,int,int,int);
extern void*ov70_02242014(u32*,int,int);extern int ov70_0223F864(s8,s8,int);
int ov70_0223B55C(u8*);
int ov70_0223B55C(u8*w){u32 p[12];ov70_0223BC7C(w);ov70_0223B7CC(P(w,4));ov70_0223B8E0(w);ov70_0223BAE0(w);ov70_0223B9C8(w);p[0]=W(w,4);p[1]=(u32)(w+0xf58);p[2]=(u32)(w+0x1168);p[3]=W(w,0xdd0);p[4]=W(w,0xf04);p[5]=W(w,0xf08);p[6]=W(w,0xdcc);p[7]=W(w,0xba0);p[8]=W(w,0xba4);p[9]=W(w,0xbb0);p[10]=W((u8*)P(w,0),0x10);p[11]=W((u8*)P(w,0x11c4),0x14);P(w,0x11a8)=ov70_02242014(p,2,1);ov70_0223CB1C(w+0x1058,w+0x1178,P(w,0xba0));ov70_0223CC04(P(w,4),w+0x10d8,P(w,0xba0),*(u16*)(w+0x11de));ov70_0223F1D8(w+0x1068,P(w,0xba4),*(s16*)(w+0xb8a),0,0,0x10200);ov70_0223F2BC(w+0x1088,P(w,0xba0),*(s8*)(w+0xb8c),1,0,0,0x10200);ov70_0223F370(w+0x10a8,P(w,0xba0),ov70_0223F864(*(s8*)(w+0xb8d),*(s8*)(w+0xb8e),1),0,0,0x10200,1);ov70_0223F244(w+0x1188,P(w,0xbb0),P(w,0xba0),W(w,0x12cc),0,0,0x10200);*(void(**)(void))(w+0x1208)=ov70_0223CCA4;if(W(w,0x24)==13)BeginNormalPaletteFade(3,1,1,0,6,1,61);W(w,0x2c)=0;return 2;}
