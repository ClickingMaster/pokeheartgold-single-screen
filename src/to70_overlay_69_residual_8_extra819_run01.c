#pragma force_active on
#include "global.h"
#include "camera.h"
extern const VecFx32 ov69_021E767C;extern int ov69_021E737C(void *);
void ov69_021E6FE8(void *);
void ov69_021E6FE8(void *w){VecFx32 target={0,0,0};VecFx32 pos=ov69_021E767C;Camera_Init_FromTargetAndPos(&target,&pos,0x5c1,0,0,*(Camera**)((u8*)w+0xc2dc));Camera_SetPerspectiveClippingPlane(0,0x64000,*(Camera**)((u8*)w+0xc2dc));Camera_ApplyPerspectiveType(0,*(Camera**)((u8*)w+0xc2dc));Camera_SetStaticPtr(*(Camera**)((u8*)w+0xc2dc));if(*(u16*)((u8*)w+0xc304)==0)*(u16*)((u8*)w+0xc2e8)=1;else *(u16*)((u8*)w+0xc2e8)=0;while(ov69_021E737C(w)!=1){}}
