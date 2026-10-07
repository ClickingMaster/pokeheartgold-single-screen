#pragma force_active on
#include "global.h"
#pragma force_active on
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#include <string.h>
extern u8*_0221A680;extern const u32 _0221602C[2];extern char _02216414[],_02216034[];int sprintf(char*,const char*,...);void ov00_021E7300(void);int ov00_021F14D4(int,const void*,u32),ov00_021F13A4(int,const char*,const char*);void ov00_021E6424(void),ov00_021E63CC(void),ov00_021E6428(void),ov00_021E6484(void),ov00_021E6554(void),ov00_021E6240(void);void ov00_021EDF1C(u8,void*,void(*)(void),int,void(*)(void),int),ov00_021F989C(void(*)(void)),ov00_021F98BC(void(*)(void)),ov00_021EE24C(void(*)(void),int),ov00_021F98DC(void(*)(void));int ov00_021E5CEC(const char*,int,int,int);
typedef struct{u32 a,b;} Pair;
int ov00_021E5CEC(const char*name,int channel,int local,int interval){Pair params;GF_ASSERT(_0221A680);if(U(_0221A680,0x1070)!=4)return 0;ov00_021E7300();params=*(const Pair*)_0221602C;params.b=1000*interval;GF_ASSERT(ov00_021F14D4(0,&params,8)==0);GF_ASSERT(ov00_021F13A4(0,_02216414,name)!=0);MI_CpuFill8(_0221A680+0xfc4,0,128);sprintf((char*)_0221A680+0xfc4,_02216034,_02216414,name);GF_ASSERT(strlen((char*)_0221A680+0xfc4)<128);if(local)ov00_021F13A4(1,(char*)_0221A680+0xfc4,(char*)_0221A680+0xfc4);U(_0221A680,0x1070)=5;U(_0221A680,0x107c)=channel;ov00_021EDF1C((u8)channel,_0221A680+0xfc4,ov00_021E63CC,0,ov00_021E6424,0);U(_0221A680,0x1074)=0;ov00_021F989C(ov00_021E6428);ov00_021F98BC(ov00_021E6484);ov00_021EE24C(ov00_021E6554,0);ov00_021F98DC(ov00_021E6240);U(_0221A680,0x1078)=0;B(_0221A680,0x10e5)=1;return 1;}
