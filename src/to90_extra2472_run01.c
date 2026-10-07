#pragma force_active on
#include "global.h"
#pragma force_active on
#define I(p,o) (*(s32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
extern u8 *_0221A680;
void ov00_021EDB1C(void),ov00_021E6CE8(void),ov00_021E7AE0(void),ov00_021E7ACC(void),ov00_021E765C(void),ov00_021E62D8(void);
s32 ov00_021EE4FC(void),ov00_021EE490(void),ov00_021E7AF4(s32,s32),ov00_021E6690(void),ov00_021E67E0(s32);
s32 ov00_021E6850(void);
s32 ov00_021E6850(void){ov00_021EDB1C();ov00_021E6CE8();if(I(_0221A680,0x1090)){if(H(_0221A680,0x10d4)==1&&I(_0221A680,0x10d0)==1&&H(_0221A680,0x10d6)==1)ov00_021E7AE0();else ov00_021E7ACC();ov00_021E765C();u8*p=_0221A680;if(I(p,0x1080)!=ov00_021EE4FC()&&!B(p,0x10df)&&I(p,0x109c)){s32 a=ov00_021EE4FC();s32 b=ov00_021EE490();if(ov00_021E7AF4(a,b))I(_0221A680,0x1080)=ov00_021EE4FC();}}s32 state=I(_0221A680,0x1070);if(state==18){s32 r=ov00_021E6690();if(!r)return 23;return r;}if(state==17)return 24;if((u32)state-7<=1){s32 i;for(i=0;i<I(_0221A680,0x107c);i++){u8 *counter=_0221A680+i*4;u32 next=*(u32*)(counter+0x10ac);s32 old=*(volatile s32*)(counter+0x10ac);*(u32*)(counter+0x10ac)=next+1;if(old>=120&&!I(_0221A680,0x1078)&&ov00_021E67E0(i)){ov00_021E62D8();break;}}}return ov00_021E6690();}
