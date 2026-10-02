#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct IrqFlags {u8 pad[0x3ff8];volatile u32 flags;} IrqFlags;
typedef struct Banks {u32 words[10];} Banks;extern Banks ov12_0226C0D0,ov12_0226C0F8;typedef struct State{u8 pad[9215];u8 a:1,b:1,c:1,padbits:5;} State;
void GfGfx_SetBanks(Banks *),PokepicManager_HandleLoadImgAndOrPltt(void *),GF_RunVramTransferTasks(void),SpriteSystem_TransferOam(void),PaletteData_PushTransparentBuffers(void *),DoScheduledBgGpuUpdates(void *),ov12_02239730(u8 *),ov12_022397E4(u8 *);
static inline void irq(u8 *base,u32 mask){*(volatile u32 *)(base+0x3ff8)|=mask;}
void ov12_02239730(u8 *s){Banks a,b;if(((State *)s)->a){((State *)s)->a=0;*(volatile u16 *)0x04000050=0;*(volatile u16 *)0x04001050=0;}if(((State *)s)->b){((State *)s)->b=0;a=ov12_0226C0D0;GfGfx_SetBanks(&a);}if(((State *)s)->c){((State *)s)->c=0;b=ov12_0226C0F8;GfGfx_SetBanks(&b);}PokepicManager_HandleLoadImgAndOrPltt(*(void **)(s+136));GF_RunVramTransferTasks();SpriteSystem_TransferOam();PaletteData_PushTransparentBuffers(*(void **)(s+40));DoScheduledBgGpuUpdates(*(void **)(s+4));irq((u8 *)0x027e0000,1);}
void ov12_022397E4(u8 *s){PaletteData_PushTransparentBuffers(*(void **)(s+12));GF_RunVramTransferTasks();DoScheduledBgGpuUpdates(*(void **)(s+4));irq((u8 *)0x027e0000,1);}
