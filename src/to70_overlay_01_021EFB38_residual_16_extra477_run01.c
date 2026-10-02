#pragma force_active on
#include "global.h"
typedef struct{int a,b;void*scene,*buffer;} Context;extern Context *ov01_02209B64;
extern void*Heap_Alloc(int,int);extern void ov01_021F1224(void),ov01_021F1250(void);extern void*sub_02014DB4(void(*)(void),void(*)(void),void*,int,int,int);extern void sub_02015528(void*,int);extern void*sub_02015524(void*);extern void Camera_SetPerspectiveClippingPlane(int,int,void*);extern void*GfGfxLoader_LoadFromOpenNarc(void*,int,int,int,int);extern void sub_0201526C(void*,void*,int,int);

void ov01_021F0F08(void*narc,int member);

void ov01_021F0F08(void*narc,int member){if(ov01_02209B64==0)GF_AssertFail();if(ov01_02209B64->scene!=0)GF_AssertFail();ov01_02209B64->buffer=Heap_Alloc(4,0x4800);ov01_02209B64->scene=sub_02014DB4(ov01_021F1224,ov01_021F1250,ov01_02209B64->buffer,0x4800,1,4);if(ov01_02209B64->scene==0)GF_AssertFail();sub_02015528(ov01_02209B64->scene,1);Camera_SetPerspectiveClippingPlane(0x1000,0x384000,sub_02015524(ov01_02209B64->scene));{void*d=GfGfxLoader_LoadFromOpenNarc(narc,member,0,4,0);sub_0201526C(ov01_02209B64->scene,d,0,0);}}
