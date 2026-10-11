#pragma force_active on
#include "global.h"
#pragma force_active on
extern void Main_SetVBlankIntrCB(void*,void*),HBlankInterruptDisable(void),GfGfx_EngineASetPlanes(int),GfGfx_EngineBSetPlanes(int),GF_TouchpadResetSampling(void),GF_TouchpadStartSampling(int),Heap_Create(int,int,u32);
extern void *Heap_Alloc(int,u32),*NARC_New(int,int),*SysTask_CreateOnVBlankQueue(void(*)(void*,void*),void*,u32);
extern void ov14_021E5A60(void),ov14_021E59AC(void*,void*),ov14_021E5EAC(int),ov14_021E783C(void*,int,int);
extern int ov14_021E7930(void*,u8);
extern void ov14_021E5A70(void*);
extern void ov14_021E5E74(void*);
extern void ov14_021E5C54(void*);
extern void ov14_021E5D78(void*);
extern void ov14_021E5DE0(void*);
extern void ov14_021F4ED0(void*);
extern void ov14_021F297C(void*);
extern void ov14_021F2F20(void*);
extern void ov14_021F2F3C(void*);
extern void ov14_021E7BA4(void*);
extern void ov14_021E81FC(void*);
extern void ov14_021E825C(void*);
extern void ov14_021E82BC(void*);
extern void ov14_021E5ED0(void*);
extern void ov14_021F5620(void*);
extern void ov14_021F566C(void*);
extern void ov14_021F49C8(void*);
extern void ov14_021F6A44(void*);
void *ov14_021EAFAC(u8*);
void *ov14_021EAFAC(u8*w){
Main_SetVBlankIntrCB(0,0);HBlankInterruptDisable();GfGfx_EngineASetPlanes(0);GfGfx_EngineBSetPlanes(0);*(volatile u16*)0x04000050=0;*(volatile u16*)0x04001050=0;GF_TouchpadResetSampling();GF_TouchpadStartSampling(4);*(volatile u16*)0x04000304 &= ~0x8000;Heap_Create(3,10,0x80000);*(void**)(w+0x34)=Heap_Alloc(10,0x88e0);MI_CpuFill8(*(void**)(w+0x34),0,0x88e0);*(void**)((u8*)*(void**)(w+0x34)+0x450)=NARC_New(2,10);*(void**)((u8*)*(void**)(w+0x34)+0x454)=NARC_New(20,10);ov14_021E5A60();
ov14_021E5A70(w);ov14_021E5E74(w);ov14_021E5C54(w);ov14_021E5D78(w);ov14_021E5DE0(w);ov14_021F4ED0(w);ov14_021F297C(w);ov14_021F2F20(w);ov14_021F2F3C(w);ov14_021E783C(w,ov14_021E7930(w,w[0x1f]),2);ov14_021E7BA4(w);if(*(u32*)((u8*)*(void**)w+8)!=1&&*(u32*)((u8*)*(void**)w+8)!=0){ov14_021E81FC(w);ov14_021E825C(w);}
ov14_021E82BC(w);ov14_021E5ED0(w);ov14_021F5620(w);ov14_021F566C(w);ov14_021F49C8(w);ov14_021F6A44(w);*(void**)*(void**)(w+0x34)=SysTask_CreateOnVBlankQueue(ov14_021E59AC,w,0);ov14_021E5EAC(1);return *(void**)(w+0x30);}
