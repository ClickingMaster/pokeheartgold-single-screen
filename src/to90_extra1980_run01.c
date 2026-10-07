#pragma force_active on
#include "global.h"
#include "to90_vct_ssp_private.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
extern struct {u32 prefix;u8*data;} _0221A684;extern u32 _0221A688;
void ov00_021E77CC(void*,void*,u32,int);void ov00_021E733C(void);void ov00_021E73E8(void);void ov00_021E7544(u32,u32,Session*,void*);void ov00_021E756C(u32,u32,Session*,void*);void ov00_021E79B4(void);void sub_0203993C(void);int sub_02034084(void);int ov00_021EE490(void);void VCT_SetCodec(int);void VCT_EnableEchoCancel(int);
void NNS_SndStrmAllocChannel(void*,int,void*);void NNS_SndStrmSetVolume(void*,int);void NNS_SndStrmSetup(void*,int,void*,u32,int,int,void(*)(void),void*);
void ov00_021E77F4(int heap,int codec,int count);
void ov00_021E77F4(int heap,int codec,int count){
 u8 channels[3]={0,0,0};u32 allocation;InitArgs config;
 if(!_0221A684.data){allocation=0;ov00_021E77CC(&allocation,&_0221A688,0x1ab0,heap);U(_0221A684.data,0x880)=allocation;ov00_021E77CC(_0221A684.data+0x888,_0221A684.data+0x884,count*0x23c0+32,heap);U(_0221A684.data,0x19f4)=heap;U(_0221A684.data,0x198c)=0;ov00_021E733C();}
 U(_0221A684.data,0x1a5c)=3;P(_0221A684.data,0x1a60)=_0221A684.data;U(_0221A684.data,0x1a64)=0x880;U(_0221A684.data,0x1a68)=0x1040;U(_0221A684.data,0x1a6c)=1;U(_0221A684.data,0x1a70)=0;U(_0221A684.data,0x1a74)=0;B(_0221A684.data,0x1a59)=1;
 NNS_SndStrmAllocChannel(_0221A684.data+0x19f8,1,channels);NNS_SndStrmSetVolume(_0221A684.data+0x19f8,0);NNS_SndStrmSetup(_0221A684.data+0x19f8,1,_0221A684.data+0x88c,0x880,65,2,ov00_021E73E8,_0221A684.data);
 U(_0221A684.data,0x19ec)=0;U(_0221A684.data,0x1a54)=0;sub_0203993C();if(!sub_02034084())config.mode=1;else config.mode=3;U(_0221A684.data,0x19e8)=config.mode;
 config.data=(Session*)(_0221A684.data+0x1990);config.count=count;config.id=(u8)ov00_021EE490();if(config.id==-1)GF_ASSERT(0);
 if(U(_0221A684.data,0x19e8)==3)config.callback=ov00_021E7544;else config.callback=ov00_021E756C;
 config.arg=NULL;config.buffer=P(_0221A684.data,0x884);config.size=count*0x23c0+32;VCT_Init(&config);U(_0221A684.data,0x19f0)=0;VCT_SetCodec(codec);ov00_021E79B4();VCT_EnableEchoCancel(1);
}
