#pragma force_active on
#include "global.h"
#include "field_overlay_particle_storage_private.h"
#include <string.h>
#pragma force_active on
extern void *Heap_Alloc(int,int);
extern void GfGfx_EngineATogglePlanes(int,int);
extern void ov01_021F1094(void *,void *);
extern void ov01_021F10C8(void *,void *);
extern void *SysTask_CreateOnVWaitQueue(void (*)(void *,void *),void *,int);
extern void *SysTask_CreateOnMainQueue(void (*)(void *,void *),void *,int);

static inline void clearBytes(void *arg,u32 n){u8 *p=arg;do{*p++=0;}while(--n);}
void ov01_021F0DDC(void *arg);
void ov01_021F0DDC(void *arg){if(sEncounterEffectState.unk0)GF_AssertFail();sEncounterEffectState.unk0=Heap_Alloc(4,24);memset(sEncounterEffectState.unk0,0,24);((FieldOverlayParticleWork*)sEncounterEffectState.unk0)->state=0;((FieldOverlayParticleWork*)sEncounterEffectState.unk0)->flag=0;((FieldOverlayParticleWork*)sEncounterEffectState.unk0)->arg=arg;GfGfx_EngineATogglePlanes(2,0);GfGfx_EngineATogglePlanes(4,0);GfGfx_EngineATogglePlanes(8,0);GX_ResetBankForBG();MIi_CpuClearFast(0,(void*)0x06840000,0x20000);*(vu32*)0x04000064=0xC0320C04;SysTask_CreateOnVWaitQueue(ov01_021F1094,sEncounterEffectState.unk0,0x400);SysTask_CreateOnMainQueue(ov01_021F10C8,sEncounterEffectState.unk0,0x400);}
