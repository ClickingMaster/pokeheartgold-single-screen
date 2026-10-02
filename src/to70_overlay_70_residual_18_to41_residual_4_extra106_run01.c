#pragma force_active on
#include "global.h"
#pragma force_active on
extern int (*const ov70_02246528[])(u8 *);
extern const u8 ov70_022464FE[],ov70_02246500[];
void ov70_02238F9C(void *,int,int);
int ov70_0223C2EC(u8 *);
int ov70_0223B6EC(u8 *);
#define W(p,o) (*(int*)((p)+(o)))
#define P(p,o) (*(void**)((p)+(o)))
#define H(p,o) (*(s16*)((p)+(o)))
int ov70_0223B6EC(u8 *w){
 int ret=ov70_02246528[W(w,0x2c)](w);
 int i,a,b;
 u8 *p;
 for(i=0,p=w;i<8;i++,p+=4){
  ov70_02238F9C(P(p,0xee4),H(p,0x120c),W(w,0xf14)+H(p,0x120e));
  ov70_02238F9C(P(w,0xf10),55,W(w,0xf14)+424);
 }
 a=ov70_0223C2EC(w);b=ov70_0223C2EC(w);
 ov70_02238F9C(P(w,0xdcc),*(const u16 *)(ov70_022464FE+a*6),*(const u16 *)(ov70_02246500+b*6)-W(w,0xf14));
 return ret;
}
