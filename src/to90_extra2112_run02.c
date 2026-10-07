#pragma force_active on
// clang-format off
#include "global.h"
int sprintf(char*,const char*,...);
#pragma force_active on
#include <nitro/code32.h>
// clang-format on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
extern u8 ov00_02219D74[];
extern u32 ov00_0221B878;
extern char ov00_0221B890[],ov00_02219E84[];
void *ov00_021EC3A8(u32);u32 ov00_021FF368(void);void ov00_02210E58(void);int ov00_02210ECC(const char*,u32,void*,int);
int ov00_022108BC(void **out,u32 kind,u32 value,const char*name,const char*other,u32 enabled,u32 p6,u32 p7,u32 p8,u32 p9,u32 p10,u32 p11,u32 p12,u32 p13);
#pragma opt_strength_reduction off
int ov00_022108BC(void **out,u32 kind,u32 value,const char*name,const char*other,u32 enabled,u32 p6,u32 p7,u32 p8,u32 p9,u32 p10,u32 p11,u32 p12,u32 p13){
 int i,status;u8*w;char buffer[64];
 if(out){w=*out=ov00_021EC3A8(0x110);}else w=ov00_02219D74;
 srand(ov00_021FF368());strcpy((char*)w+4,name);strcpy((char*)w+0x44,other);
 U(w,0xc0)=value;U(w,0xac)=0;U(w,0xb0)=0;U(w,0)=kind;U(w,0xb8)=1;
 U(w,0x10c)=p13;U(w,0x88)=p7;U(w,0x8c)=p8;U(w,0x90)=p9;U(w,0x94)=p10;U(w,0x98)=p11;U(w,0x9c)=p12;
 U(w,0xa0)=0;U(w,0xa4)=0;U(w,0xd4)=0;U(w,0xbc)=enabled;U(w,0xc4)=0;U(w,0xc8)=p6;U(w,0x104)=0;H(w,0x108)=0;U(w,0xa8)=0;U(w,0xb4)=0;
 for(i=0;i<4;i++)B(w,0x84+i)=rand()%255;
 for(i=0;i<10;i++)U(w+4*i,0xd8)=-1;
 U(w,0x100)=0;if(!ov00_0221B878)ov00_02210E58();
 if(enabled){s8 first=ov00_0221B890[0];if(!first)sprintf(buffer,ov00_02219E84,name);status=ov00_02210ECC(first?ov00_0221B890:buffer,0x6cfc,w+0xcc,0);}else status=1;
 if(status==0)return 3;else return 0;
}
