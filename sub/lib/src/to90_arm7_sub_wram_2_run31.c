#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
void SPI_DummyWait_03803F70(void);void NVRAM_ReadDataBytesAtHigherSpeed(u32,u32,u8*);
// @fn NVRAM_ReadDataBytesAtHigherSpeed
void NVRAM_ReadDataBytesAtHigherSpeed(u32 address,u32 length,u8*out){u16 a[3];int j;u32 k;if(length<1)return;a[0]=(address&0xff0000)>>16;a[1]=(address&0xff00)>>8;a[2]=address&255;while((*(volatile u16*)0x040001C0)&128){}(*(volatile u16*)0x040001C0)=0x8900;(*(volatile u16*)0x040001C2)=11;j=0;for(;j<3;j++){while((*(volatile u16*)0x040001C0)&128){}(*(volatile u16*)0x040001C2)=a[j]&255;}while((*(volatile u16*)0x040001C0)&128){}(*(volatile u16*)0x040001C2)=0;while((*(volatile u16*)0x040001C0)&128){}for(k=0;k<length-1;k++){(*(volatile u16*)0x040001C2)=0;while((*(volatile u16*)0x040001C0)&128){}out[k]=(u8)(*(volatile u16*)0x040001C2);}(*(volatile u16*)0x040001C0)=0x8100;SPI_DummyWait_03803F70();out[k]=(u8)*(volatile u16*)0x040001C2;}
