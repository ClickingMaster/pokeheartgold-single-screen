#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u32 OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(u32);
extern void SPIi_ReturnResult(u16,u32);
extern int SPIi_SetEntry(u32,u32,u16,...);
typedef struct{u16 words[16];u32 state;u16 flags,mode;u32 address,index,length;u16 reload,control;u8 unk38[0x4];}W37001Work;
W37001Work micw;
extern int MicSetTimerValue(u32);
#define H(a) (*(volatile u16*)(a))
#define W(a) (*(volatile u32*)(a))
void MIC_Init(void);
void MIC_AnalyzeCommand(u32);
// @fn MIC_Init
void MIC_Init(void) {
    int i;
    micw.state = 0;
    for (i = 0; i < 16; i++) {
        micw.words[i] = 0;
    }
    H(0x0400010e) &= ~0x80;
}
// @fn MIC_AnalyzeCommand
void MIC_AnalyzeCommand(u32 packet){
u16 cmd;
if(packet&0x2000000){int i;for(i=0;i<16;i++)micw.words[i]=0;}
micw.words[(packet&0xf0000)>>16]=(u16)packet;
if(!(packet&0x1000000))return;
cmd=(micw.words[0]&0xff00)>>8;
switch(cmd){
case 0x40:
if(!SPIi_SetEntry(2,cmd,1,micw.words[0]&255))SPIi_ReturnResult(cmd,4);
H(0x027fff94)=0;W(0x027fff90)=0;break;
case 0x41:{u32 address,length;
if(micw.state){SPIi_ReturnResult(cmd,3);break;}
micw.flags=micw.words[0]&255;
address=((u32)micw.words[1]<<16)|micw.words[2];
if(address<0x02000000||address>=0x02400000){SPIi_ReturnResult(cmd,2);break;}
micw.address=address;
length=((u32)micw.words[3]<<16)|micw.words[4];
if(address+length>0x02400000){SPIi_ReturnResult(cmd,2);break;}
micw.length=length;
if(!MicSetTimerValue(((u32)micw.words[5]<<16)|micw.words[6])){SPIi_ReturnResult(cmd,2);break;}
micw.index=0;micw.mode=micw.flags&7;
if(!SPIi_SetEntry(2,cmd,0)){SPIi_ReturnResult(cmd,4);break;}
H(0x027fff94)=0;W(0x027fff90)=0;micw.state=1;break;}
case 0x42:
if(micw.state!=2){SPIi_ReturnResult(cmd,3);break;}
if(!SPIi_SetEntry(2,cmd,0)){SPIi_ReturnResult(cmd,4);break;}
micw.state=3;H(0x0400010e)&=~0x80;break;
case 0x43:{u32 irq;
if(micw.state!=2){SPIi_ReturnResult(cmd,3);break;}
if(!MicSetTimerValue(((u32)micw.words[1]<<16)|micw.words[2])){SPIi_ReturnResult(cmd,2);break;}
irq=OS_DisableInterrupts();H(0x0400010e)&=~0x80;H(0x0400010c)=micw.reload;H(0x0400010e)=micw.control|0xc0;OS_RestoreInterrupts(irq);SPIi_ReturnResult(cmd,0);break;}
default:SPIi_ReturnResult(cmd,1);break;
}}
