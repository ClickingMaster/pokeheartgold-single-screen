#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define PTR(p,o) ((void*)W(p,o))
void SpriteSystem_TransferOam(void*);void GF_RunVramTransferTasks(void);void DoScheduledBgGpuUpdates(void*);
void ov113_021E62B0(void*p);
void ov113_021E62B0(void*p){volatile u32*base=(volatile u32*)0x027e0000;if(PTR(p,172))SpriteSystem_TransferOam(PTR(p,172));GF_RunVramTransferTasks();DoScheduledBgGpuUpdates(PTR(p,64));base[0x3ff8/4]|=1;}
