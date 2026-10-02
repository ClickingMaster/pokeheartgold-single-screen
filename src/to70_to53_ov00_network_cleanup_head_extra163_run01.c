#pragma force_active on
#include "global.h"
#pragma force_active on
extern u8 *_0221A680;
void *Heap_Alloc(int,u32);
void *sub_0202C6F4(void *),*sub_0202C08C(void *),*sub_0202C23C(void *,int);
void ov00_021E700C(void);
int DWC_CheckHasProfile(void *),DWC_CheckValidConsole(void *);
int ov00_021E5900(void *,int,u32,void *);
#define W(o) (*(int *)(_0221A680+(o)))
#define P(o) (*(void **)(_0221A680+(o)))
#define B(o) (*(_0221A680+(o)))
#define H(o) (*(u16 *)(_0221A680+(o)))
int ov00_021E5900(void *save,int heap,u32 size,void *config){
 u8 *raw;
 int i;
 GF_ASSERT(_0221A680==NULL);
 raw=Heap_Alloc(heap,0x1108);MI_CpuFill8(raw,0,0x1108);
 _0221A680=(u8 *)(((u32)raw+31)&~31);
 P(0xf7c)=raw;P(0xf78)=save;W(0xfa4)=0;W(0xfa8)=0;W(0xfc0)=0;W(0x1070)=0;
 W(0x1084)=heap;W(0x1088)=heap;W(0x108c)=size;
 P(0xf90)=Heap_Alloc(heap,size+0x3020);W(0xf98)=0;
 P(0xf94)=NNS_FndCreateExpHeapEx((void *)(((u32)P(0xf90)+31)&~31),size,0);
 W(0xf9c)=0;W(0x10a4)=0;W(0x1094)=-1;W(0x106c)=0;P(0x107c)=config;
 W(0x1080)=0;W(0x10d8)=0;W(0x1098)=-1;W(0x109c)=1;
 B(0x10de)=0;W(0x10cc)=0;B(0x10dc)=0;B(0x10dd)=0;H(0x10d4)=1;W(0x10d0)=1;H(0x10d6)=1;
 if(save!=NULL){P(0xf10)=sub_0202C08C(sub_0202C6F4(P(0xf78)));P(0x100)=sub_0202C23C(sub_0202C6F4(P(0xf78)),0);}
 B(0x10e5)=1;W(0x1064)=0;
 for(i=0;i<32;i++)*(_0221A680+i+0x1044)=0;
 ov00_021E700C();
 if(!DWC_CheckHasProfile(P(0xf10)))return 1;
 if(DWC_CheckValidConsole(P(0xf10)))return 0;
 return 2;
}
