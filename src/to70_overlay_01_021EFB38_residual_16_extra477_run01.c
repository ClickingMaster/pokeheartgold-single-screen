#pragma force_active on
#include "global.h"
#include "heap.h"
#include "spl.h"
#include "field_overlay_particle_storage_private.h"

extern u32 ov01_021F1224(u32,BOOL),ov01_021F1250(u32,BOOL);extern SPLEmitter*ParticleSystem_New(u32(*)(u32,BOOL),u32(*)(u32,BOOL),void*,u32,BOOL,enum HeapID);extern void ParticleSystem_SetPerspectiveType(void*,int);extern void*ParticleSystem_GetCamera(void*);extern void Camera_SetPerspectiveClippingPlane(int,int,void*);extern void*GfGfxLoader_LoadFromOpenNarc(void*,int,int,int,int);extern void ParticleSystem_SetResource(void*,void*,int,int);

void ov01_021F0F08(void*narc,int member);

void ov01_021F0F08(void*narc,int member){if(ov01_02209B64.unk0==0)GF_AssertFail();if(((FieldOverlayParticleWork*)ov01_02209B64.unk0)->scene!=0)GF_AssertFail();((FieldOverlayParticleWork*)ov01_02209B64.unk0)->buffer=Heap_Alloc((enum HeapID)4,0x4800);((FieldOverlayParticleWork*)ov01_02209B64.unk0)->scene=ParticleSystem_New(ov01_021F1224,ov01_021F1250,((FieldOverlayParticleWork*)ov01_02209B64.unk0)->buffer,0x4800,1,(enum HeapID)4);if(((FieldOverlayParticleWork*)ov01_02209B64.unk0)->scene==0)GF_AssertFail();ParticleSystem_SetPerspectiveType(((FieldOverlayParticleWork*)ov01_02209B64.unk0)->scene,1);Camera_SetPerspectiveClippingPlane(0x1000,0x384000,ParticleSystem_GetCamera(((FieldOverlayParticleWork*)ov01_02209B64.unk0)->scene));{void*d=GfGfxLoader_LoadFromOpenNarc(narc,member,0,4,0);ParticleSystem_SetResource(((FieldOverlayParticleWork*)ov01_02209B64.unk0)->scene,d,0,0);}}
