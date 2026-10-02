#pragma force_active on
#include "global.h"
#pragma force_active on
extern const u8 ov70_022464F0[],ov70_022464F1[],ov70_022464FE[],ov70_02246500[],ov70_02246502[];
int ov70_0223C2EC(void *);
void ov70_02238F9C(void *,int,int);
void Sprite_SetAnimCtrlSeq(void *,int);
void ov70_0223C420(u8 *,int);
#define P(p,o) (*(void **)((p)+(o)))
#define W(p,o) (*(int *)((p)+(o)))
void ov70_0223C420(u8 *w,int i){
 int a,b;
 if(ov70_022464F0[i*2]==0){W((u8 *)P(w,0x11c4),36)=0;W((u8 *)P(w,0x11c4),40)=ov70_022464F1[i*2];}
 else{W((u8 *)P(w,0x11c4),36)=1;W((u8 *)P(w,0x11c4),44)=ov70_022464F1[i*2];}
 a=ov70_0223C2EC(w);b=ov70_0223C2EC(w);
 ov70_02238F9C(P(w,0xdcc),*(const u16 *)(ov70_022464FE+a*6),*(const u16 *)(ov70_02246500+b*6));
 b=ov70_0223C2EC(w);
 Sprite_SetAnimCtrlSeq(P(w,0xdcc),*(const u16 *)(ov70_02246502+b*6));
}
