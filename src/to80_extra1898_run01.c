#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x180];void *bg;u8 pad2[0x7fc-0x184];u8 data[2][3200];u16 fields[2][8];int queued;} Work;
extern void DoScheduledBgGpuUpdates(void *);extern void GXS_LoadOBJ(const void *,u32,u32);extern void GfGfxLoader_GXLoadPal(u16,u16,int,u32,u32,int);extern void GF_RunVramTransferTasks(void);extern void OamManager_ApplyAndResetBuffers(void);
void ov65_0221CE1C(Work *);
static inline void setIrq(u32*base){base[0xFFE]|=1;}
void ov65_0221CE1C(Work *w){DoScheduledBgGpuUpdates(w->bg);if(w->queued){int n=w->queued-1;GXS_LoadOBJ(w->data[n],n*3200,3200);GfGfxLoader_GXLoadPal(w->fields[n][0],w->fields[n][2],5,(n+2)*32,32,26);w->queued=0;}GF_RunVramTransferTasks();OamManager_ApplyAndResetBuffers();setIrq((u32*)0x027e0000);}
