#pragma force_active on
#include "global.h"
typedef struct {void*bg;u8 pad[0xea0];void(*callback)(void*);} Work;
extern Work ov74_0223D454;extern u32 ov74_0223E2D4[];void GF_RunVramTransferTasks(void);void OamManager_ApplyAndResetBuffers(void);void DoScheduledBgGpuUpdates(void*);void ov74_02235A74(void);
static inline void irq(u32 flag){volatile u32 *base=(volatile u32*)0x027E0000; base[0xffe]|=flag;}
void ov74_02235A74(void){Work*w=&ov74_0223D454;if(ov74_0223E2D4[9]){w->callback(w);w->callback=0;}GF_RunVramTransferTasks();OamManager_ApplyAndResetBuffers();if(w->bg)DoScheduledBgGpuUpdates(w->bg); irq(1);}
