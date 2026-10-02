#pragma force_active on
#include "global.h"
#include "camera.h"
#include "sprite_system.h"
#pragma force_active on
typedef struct W{u8 p[0x11c];SpriteSystem*sys;SpriteManager*mgr;ManagedSprite*sprites[12];}W;extern const ManagedSpriteTemplate ov68_021E7E74[];extern const VecFx32 ov68_021E7BEC;extern void ov68_021E6DDC(W*),ov68_021E6EB8(W*,int);void ov68_021E7178(W*,int),ov68_021E71C4(Camera**,enum HeapID);
void ov68_021E71C4(Camera**out,enum HeapID heap){VecFx32 pos=ov68_021E7BEC;CameraAngle angle={0,0,0,0};*out=Camera_New(heap);Camera_Init_FromPosDistanceAndAngle(&pos,0x10000,&angle,0x5c1,1,*out);Camera_SetPerspectiveClippingPlane(0,0x64000,*out);Camera_ClearFixedTarget(*out);Camera_SetStaticPtr(*out);}
