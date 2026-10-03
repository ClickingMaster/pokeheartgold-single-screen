#pragma force_active on
#include "global.h"
static inline void setIrq(u32*base){base[0xFFE]|=1;}
void ov67_021E6A00(void*);void DoScheduledBgGpuUpdates(void*);void ov67_021E5BE0(u32*);
void ov67_021E5BE0(u32*p){ov67_021E6A00(p);DoScheduledBgGpuUpdates((void*)p[4]);setIrq((u32*)0x027E0000);}
