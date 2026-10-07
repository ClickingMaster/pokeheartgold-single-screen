#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
void SPI_SendWait_03803D98(u32);void NVRAM_PageProgram(u32,u16,const u8*);
// @fn NVRAM_PageProgram
void NVRAM_PageProgram(u32 address,u16 length,const u8*in){u16 a[3];int j,limit;if(length<1)return;if(((address+length-1)>>8)>(address>>8))length=(u16)(256-(address&255));a[0]=(address&0xff0000)>>16;a[1]=(address&0xff00)>>8;a[2]=address&255;while((*(volatile u16*)0x040001C0)&128){}(*(volatile u16*)0x040001C0)=0x8900;(*(volatile u16*)0x040001C2)=2;j=0;for(;j<3;j++){while((*(volatile u16*)0x040001C0)&128){}(*(volatile u16*)0x040001C2)=a[j]&255;}limit=length-1;for(j=0;j<limit;j++){while((*(volatile u16*)0x040001C0)&128){}(*(volatile u16*)0x040001C2)=in[j];}while((*(volatile u16*)0x040001C0)&128){}(*(volatile u16*)0x040001C0)=0x8100;SPI_SendWait_03803D98(in[j]);}
