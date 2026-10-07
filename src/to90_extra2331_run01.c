#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct To90W30509Work{u8 p[0x10];void*net;u8 p14[0x2bc];u8 peers[1];}To90W30509Work;typedef struct To90W30509Header{u8 zero,kind;u16 port;u32 address;}To90W30509Header;extern To90W30509Work*ov00_021F955C(void);extern const u8 ov00_02219D28[];extern int memcmp(const void*,const void*,u32);extern void ov00_02211A88(void*,u8*,u32,To90W30509Header*),ov00_0221075C(u8*,u32,To90W30509Header*);int ov00_021F2880(void*,u32,int,u8*,u32);
#define WORD(w,o) (*(u32*)((u8*)(w)+(o)))
#define BYTE(w,o) (*(u8*)((u8*)(w)+(o)))
#define HALF(w,o) (*(u16*)((u8*)(w)+(o)))
typedef struct To90W30509Peer{u8 id,value;u16 zero;u32 extra;}To90W30509Peer;extern const u8 ov00_02217A74[],ov00_02217A80[],ov00_02217A8C[];extern int ov00_021EE6D0(void),ov00_0220D000(void*,u32);extern void*ov00_021EE72C(int);extern To90W30509Peer*ov00_021EE7A4(int);extern u32 strtoul(const char*,char**,int);extern void ov00_0220D00C(void*,const void*,int),ov00_021F2EE4(int,int),ov00_021F2EC4(void),ov00_0220D2A4(void*,To90W30509Peer*,int),ov00_021F64C0(int);void ov00_021F2974(void*,void*,u32,int,int,u8*);
#pragma opt_optimizecontrolflow off
int ov00_021F2880(void*unused,u32 address,int port,u8*data,u32 length){To90W30509Header h;u8 tag;if(length==0||data==0)return 0;MI_CpuClear32(&h,sizeof(h));h.kind=2;h.port=((port>>8)&255)|((port<<8)&0xff00);h.address=address;tag=data[0];if((tag==254&&data[1]==253)||tag==92){if(ov00_021F955C()->net)ov00_02211A88(ov00_021F955C()->net,data,length,&h);}else if(memcmp(data,ov00_02219D28,6)==0)ov00_0221075C(data,length,&h);else{if(tag==254)return 0;else return 0;}return 1;}
