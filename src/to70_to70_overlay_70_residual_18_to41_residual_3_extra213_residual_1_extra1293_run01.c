#pragma force_active on
#include "global.h"
#include "system.h"
extern u8 ov70_022454A4[],ov70_022454A5[],ov70_02245498[];
extern void ov70_0223B364(u32 *,int,int,int,int),Gts_SetState(u32 *,int,int),PlaySE(int),BeginNormalPaletteFade(int,int,int,int,int,int,int),ov70_0223AFA8(u32 *,int);
extern int ov70_02241164(u32),TouchscreenHitbox_FindRectAtTouchNew(void *);
int ov70_0223AFFC(u32 *);
int ov70_0223AFFC(u32 *w){if(gSystem.newKeys&1){ov70_0223B364(w,16,1,0,0xf0f);Gts_SetState(w,3,4);PlaySE(1500);}else if(gSystem.newKeys&2){BeginNormalPaletteFade(0,0,0,0,16,1,61);w[11]=8;PlaySE(1500);}else if(gSystem.newKeys&32){int n=w[0x4b];int next=ov70_022454A4[2*n];if(n!=next&&(int)w[0x4a]>=next+1)ov70_0223AFA8(w,next);}else if(gSystem.newKeys&16){int n=w[0x4b];int next=ov70_022454A5[2*n];if(n!=next&&(int)w[0x4a]>=next+1)ov70_0223AFA8(w,next);}else{int next=ov70_02241164(w[0x4a]);int hit;if(next!=-1)ov70_0223AFA8(w,next);hit=TouchscreenHitbox_FindRectAtTouchNew(ov70_02245498);if(hit==0){ov70_0223B364(w,16,1,0,0xf0f);Gts_SetState(w,3,4);PlaySE(1500);}else if(hit==1){BeginNormalPaletteFade(0,0,0,0,16,1,61);w[11]=8;PlaySE(1500);}}return 3;}
