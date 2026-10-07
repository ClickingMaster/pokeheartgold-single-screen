#pragma force_active on
#include "global.h"
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define C(p,o) (*(s8*)((u8*)(p)+(o)))
extern const u8 ov08_02225B68[];extern u8 gSystem[];
extern u32 ov08_02223368(void*,const void*);
extern int ov08_02224B8C(void*),ov08_02224B88(void*),ov08_02224C94(void*),ov08_02223CD4(void*,int);
extern void ov08_0222417C(void*),ov08_02224938(void*,u8,int),PlaySE(int);
int ov08_02222918(void*);
#pragma force_active on
int ov08_02222918(void *w){u32 selection=ov08_02223368(w,ov08_02225B68);
 if(selection==~0u){int moved=ov08_02224B8C(P(w,0x34));int old=ov08_02224B88(P(w,0x34));selection=ov08_02224C94(P(w,0x34));int current=ov08_02224B88(P(w,0x34));if(selection==~1u)selection=6;else if(moved==1&&old==current){u32 buttons=U(gSystem,0x4c);if((buttons&32)&&(current==0||current==2||current==4))selection=7;if((buttons&16)&&(current==1||current==3||current==5))selection=8;}}
 else ov08_0222417C(w);
 switch(selection){case 0:case 1:case 2:case 3:case 4:case 5:if(ov08_02223CD4(w,selection)){PlaySE(1501);B((u8*)P(w,0)+B(w,0x114d),0x27)=selection;B(w,0x114b)=6;ov08_02224938(w,(u8)(selection+6),0);return 11;}break;
 case 6:PlaySE(1501);B(w,0x114b)=4;ov08_02224938(w,14,0);return 11;
 case 7:if(B((u8*)w+B(w,0x114d),0x1154)){PlaySE(1501);B(w,0x114b)=7;C(w,0x114e)=-1;ov08_02224938(w,12,0);return 11;}break;
 case 8:if(B((u8*)w+B(w,0x114d),0x1154)){PlaySE(1501);B(w,0x114b)=7;B(w,0x114e)=1;ov08_02224938(w,13,0);return 11;}break;
 }return 2;
}
