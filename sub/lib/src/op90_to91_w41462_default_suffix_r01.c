#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define BASE (*(u8 **)0x0380fff4)
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define W(p,o) (*(u32 *)((u8*)(p)+(o)))
#define HW(a) (*(volatile u16 *)(a))
void FLASH_DirectRead(u32,u32,void*);void FLASH_Read(u32,u32,void*);int RF_Write(u32);int BBP_Write(u32,u32);
int WSetChannel(u16 channel,int direct);
// @fn WSetChannel
int WSetChannel(u16 channel,int direct){
 void(*read)(u32,u32,void*)=direct?FLASH_DirectRead:FLASH_Read;
 u16 saved;u32 state,value,address;
 if(!(H(BASE,0x32c)&(1u<<channel)))return 5;
 saved=HW(0x04808040);HW(0x04808040)=0x8001;
 {int busy,power;do{power=HW(0x0480803c)>>8;busy=HW(0x04808214);}while(power!=2||(busy!=0&&busy!=9));}
 H(BASE,0x3be)=channel;state=H(BASE,0x5f8);
 switch(state){case 2:case 5:{u32 index=(u32)channel-1u;u32 offset=index*6;value=0;read(offset+0xf2,3,&value);RF_Write(value);read(offset+0xf5,3,&value);RF_Write(value);value=0;if(W(BASE,0x604)&0x10000){if(!(W(BASE,0x604)&0x8000)){read(index+0x154,1,&value);value=W(BASE,0x604)|((value<<27)>>17);RF_Write(value);}}else{read(index+0x146,1,&value);BBP_Write(30,value);}break;}
 case 3:{u32 offset=H(BASE,0x5fc)+0xcf;u32 i;for(i=0;i<H(BASE,0x600);i++){value=0;address=0;read(offset,1,&address);read(offset+channel,1,&value);BBP_Write(address,value);offset+=15;}for(i=0;i<H(BASE,0x5fe);i++){value=0;read(offset,1,&value);value<<=8;read(offset+channel,1,&value);value|=0x50000;RF_Write(value);offset+=15;}break;}}
 HW(0x04808040)=saved;HW(0x04808048)=3;return 0;
}
