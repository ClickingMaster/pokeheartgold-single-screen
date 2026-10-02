#pragma force_active on
#include "global.h"
#pragma force_active on
extern u8*ov74_0223E2FC;extern void CRYPTO_RC4Init(void*,const void*,u32),CRYPTO_RC4Encrypt(void*,const void*,u32,void*);
u16 ov74_022368A4(const u16*,u32);int ov74_022368D4(void);
u16 ov74_022368A4(const u16*p,u32 size){u16 sum=0;u16 i=0;for(;i<size/2;i++){sum+=p[i];sum=(sum<<1)|(sum>>15);}return sum;}
int ov74_022368D4(void){u16 key[4];u8*p=ov74_0223E2FC;u16*source=(u16*)(p+0x1170);key[0]=source[0];key[3]=source[1];key[2]=source[2];key[1]=*(u16*)(p+0x115a);int i=0;u32 prev=0x3fa2;for(;i<4;i++){key[i]^=prev;prev=key[i];}CRYPTO_RC4Init(ov74_0223E2FC+0x104c,key,8);p=ov74_0223E2FC;CRYPTO_RC4Encrypt(p+0x104c,*(void**)(p+0x1178),*(u32*)(p+0x1160),*(void**)(p+0x1178));MIi_CpuClear32(0,(u32*)(ov74_0223E2FC+0x104c),0x104);u16 sum=ov74_022368A4(*(void**)(ov74_0223E2FC+0x1178),*(u32*)(ov74_0223E2FC+0x1160));if(*(u16*)(ov74_0223E2FC+0x115a)==sum)return 1;return 0;}
