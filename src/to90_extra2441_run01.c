#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))

#include "camera.h"
extern const VecFx32 ov96_0221C0F4;void ov96_021F6398(void*);
void ov96_021F6398(void*s){VecFx32 up;CameraAngle angle;VecFx32 delta=ov96_0221C0F4;W(s,0x194)=0;W(s,0x198)=0;W(s,0x19c)=0;angle.x=0;angle.y=0;angle.z=0;Camera_Init_FromTargetDistanceAndAngle((VecFx32*)((u8*)s+0x194),409600,&angle,164,1,1,P(s,0x190));Camera_OffsetLookAtPosAndTarget(&delta,P(s,0x190));Camera_SetPerspectiveClippingPlane(16384,1048576,P(s,0x190));up.x=0;up.y=4096;up.z=0;Camera_SetLookAtCamUp(&up,P(s,0x190));Camera_SetStaticPtr(P(s,0x190));}
