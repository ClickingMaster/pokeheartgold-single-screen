#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
typedef struct{u8 pad[0x3ff8];u32 flags;}Irq;extern void DoScheduledBgGpuUpdates(void*),SpriteSystem_TransferOam(void);void ov103_021EC9B4(void*);
static inline void irq(u8*base,u32 mask){*(volatile u32*)(base+0x3ff8)|=mask;}
void ov103_021EC9B4(void*w){DoScheduledBgGpuUpdates(P(P(w,12),0));SpriteSystem_TransferOam();irq((u8*)0x027e0000,1);}
