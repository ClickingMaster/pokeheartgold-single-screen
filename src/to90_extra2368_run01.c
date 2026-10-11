#pragma force_active on
#include "global.h"
#pragma force_active on
extern void Main_SetVBlankIntrCB(void*,void*),HBlankInterruptDisable(void),GfGfx_EngineASetPlanes(int),GfGfx_EngineBSetPlanes(int),GF_TouchpadResetSampling(void),GF_TouchpadStartSampling(int),Heap_Create(int,int,u32);
extern void *Heap_Alloc(int,u32),*NARC_New(int,int),*SysTask_CreateOnVBlankQueue(void(*)(void*,void*),void*,u32);
extern void PCBox_SetGfxBanks(void),PCBox_VBlankCallback(void*,void*),PCBox_SetBlendEnabled(int),PCBox_LoadScrolledWallpaper(void*,int,int);
extern int PCBox_GetWallpaperGfxIndex(void*,u8);
extern void PCBox_InitBgLayers(void*);
extern void PCBox_CreatePaletteData(void*);
extern void PCBox_LoadBgGraphics(void*);
extern void PCBox_CreateMsgResources(void*);
extern void PCBox_LoadWindowGraphics(void*);
extern void PCBox_AddWindows(void*);
extern void ov14_021F297C(void*);
extern void PCBox_LoadBoxIcons(void*);
extern void PCBox_LoadPartyIcons(void*);
extern void PCBox_InitPanels(void*);
extern void PCBox_ShowPartyButton(void*);
extern void ov14_021E825C(void*);
extern void ov14_021E82BC(void*);
extern void PCBox_CreateYesNoPrompt(void*);
extern void ov14_021F5620(void*);
extern void PCBox_PrintInfoLabels(void*);
extern void PCBox_DrawAllBoxThumbnails(void*);
extern void PCBox_CreateModeGridInput(void*);
void *PCBox_State_Init(u8*);
void *PCBox_State_Init(u8*w){
Main_SetVBlankIntrCB(0,0);HBlankInterruptDisable();GfGfx_EngineASetPlanes(0);GfGfx_EngineBSetPlanes(0);*(volatile u16*)0x04000050=0;*(volatile u16*)0x04001050=0;GF_TouchpadResetSampling();GF_TouchpadStartSampling(4);*(volatile u16*)0x04000304 &= ~0x8000;Heap_Create(3,10,0x80000);*(void**)(w+0x34)=Heap_Alloc(10,0x88e0);MI_CpuFill8(*(void**)(w+0x34),0,0x88e0);*(void**)((u8*)*(void**)(w+0x34)+0x450)=NARC_New(2,10);*(void**)((u8*)*(void**)(w+0x34)+0x454)=NARC_New(20,10);PCBox_SetGfxBanks();
PCBox_InitBgLayers(w);PCBox_CreatePaletteData(w);PCBox_LoadBgGraphics(w);PCBox_CreateMsgResources(w);PCBox_LoadWindowGraphics(w);PCBox_AddWindows(w);ov14_021F297C(w);PCBox_LoadBoxIcons(w);PCBox_LoadPartyIcons(w);PCBox_LoadScrolledWallpaper(w,PCBox_GetWallpaperGfxIndex(w,w[0x1f]),2);PCBox_InitPanels(w);if(*(u32*)((u8*)*(void**)w+8)!=1&&*(u32*)((u8*)*(void**)w+8)!=0){PCBox_ShowPartyButton(w);ov14_021E825C(w);}
ov14_021E82BC(w);PCBox_CreateYesNoPrompt(w);ov14_021F5620(w);PCBox_PrintInfoLabels(w);PCBox_DrawAllBoxThumbnails(w);PCBox_CreateModeGridInput(w);*(void**)*(void**)(w+0x34)=SysTask_CreateOnVBlankQueue(PCBox_VBlankCallback,w,0);PCBox_SetBlendEnabled(1);return *(void**)(w+0x30);}
