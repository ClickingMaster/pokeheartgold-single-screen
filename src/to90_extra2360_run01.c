#pragma force_active on
#include "global.h"
#pragma force_active on
extern void *Heap_Alloc(int,u32),*NARC_New(int,int);
extern void HeapExp_FndInitAllocator(void*,int,int),ov108_021E9230(void*,int),GfGfx_EngineATogglePlanes(int,int);
extern void GF_3DVramMan_InitLinkedListTexVramManager(u32,u32,void*,u32,BOOL),GF_3DVramMan_InitLinkedListPlttVramManager(u32,void*,u32,BOOL);
#define H(a) (*(volatile u16*)(a))
#define W(a) (*(volatile u32*)(a))
void ov108_021E8CD4(u8*,int,int);
void ov108_021E8CD4(u8*w,int flag,int heap){u32 size;MI_CpuFill8(w,0,244);NNS_G3dInit();G3X_Init();G3X_InitMtxStack();H(0x04000060)&=~0x3002;H(0x04000060)&=0xcffb;H(0x04000060)=(H(0x04000060)&~0x3000)|8;H(0x04000060)=(H(0x04000060)&~0x3000)|16;H(0x04000060)=(H(0x04000060)&~0x3000)|32;G3X_SetFog(0,(GXFogBlend)0,(GXFogSlope)0,0);G3X_SetClearColor(0,0,0x7fff,63,0);W(0x04000540)=2;W(0x04000580)=0xbfff0000;*(int*)(w+20)=heap;w[241]=flag;size=NNS_GfdGetLnkTexVramManagerWorkSize(128);*(void**)(w+236)=Heap_Alloc(*(int*)(w+20),size);GF_3DVramMan_InitLinkedListTexVramManager(0x20000,0,*(void**)(w+236),size,1);size=NNS_GfdGetLnkPlttVramManagerWorkSize(256);*(void**)(w+232)=Heap_Alloc(*(int*)(w+20),size);GF_3DVramMan_InitLinkedListPlttVramManager(0x4000,*(void**)(w+232),size,1);HeapExp_FndInitAllocator(w+4,heap,32);*(void**)(w+24)=NARC_New(40,heap);*(void**)(w+28)=NARC_New(107,heap);*(void**)(w+32)=NARC_New(106,heap);ov108_021E9230(w,heap);GfGfx_EngineATogglePlanes(1,1);H(0x04000008)=(H(0x04000008)&~3)|1;}
