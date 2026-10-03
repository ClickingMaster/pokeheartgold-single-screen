#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include <string.h>
extern u8 *ov00_0221A784;
extern u8 ov00_0221B430[],ov00_0221B530[];
extern void ov00_021EC210(void *),ov00_021F2BBC(void),ov00_021EEE34(void),ov00_021EEE40(void),ov00_021EF1A8(void),ov00_021EEA74(void);
extern void ov00_021EE6FC(void);
extern void ov00_021EF1F0(void *,void *,void *,int,u32,void *,void (*)(void),int);
extern void ov00_021EFF48(void *,void *,void *,int,int);
extern void ov00_021F1620(void *,void *,void *,void *,void *,void *,int,int);
extern void ov00_021F99D4(void *);
void ov00_021ED730(u8 *,u8 *,int,const char *,const char *,int,int,int,int);
#define W(x) (*(u32 *)(ov00_0221A784+(x)))
#define B(x) (*(u8 *)(ov00_0221A784+(x)))
void ov00_021ED730(u8 *work,u8 *ctx,int c,const char *name,const char *other,int a,int b,int d,int e){
 u32 size;
 ov00_0221A784=work;ov00_021EC210(work);
 W(0)=0;W(4)=(u32)ov00_021F2BBC;W(8)=(u32)ov00_021EEE34;W(12)=(u32)ov00_021EEE40;W(16)=(u32)ov00_021EF1A8;
 W(20)=a?a:0x2000;W(24)=b?b:0x2000;
 W(28)=0;W(32)=(u32)ctx;W(36)=0;W(40)=0;B(44)=0;B(45)=0;W(100)=0;W(104)=(u32)ov00_0221B430;W(108)=(u32)ov00_0221B530;
 W(112)=0;
 W(116)=0;
 W(120)=0;
 W(124)=0;
 W(128)=0;
 W(132)=0;
 W(136)=0;
 W(140)=0;
 W(144)=0;
 W(148)=0;
 ov00_021EE6FC();
 ov00_021EF1F0(ov00_0221A784+0x98,ctx,ov00_0221A784+0x1c,c,*(u32 *)(ctx+0x24),ov00_0221A784+0x2e,ov00_021EEA74,0);
 ov00_021EFF48(ov00_0221A784+0x318,ov00_0221A784+0x1c,ov00_0221A784+0x2e,d,e);
 ov00_021F1620(ov00_0221A784+0x374,ov00_0221A784+0x1c,ov00_0221A784,ov00_0221A784+4,ov00_0221B430,ov00_0221B530,d,e);
 ov00_021F99D4(ov00_0221A784+0x7f8);
 if(strlen(name)>=256)size=255;else size=strlen(name);
 MI_CpuCopy8(name,ov00_0221B430,size);ov00_0221B430[size]=0;
 if(strlen(other)>=256)size=255;else size=strlen(other);
 MI_CpuCopy8(other,ov00_0221B530,size);ov00_0221B530[size]=0;
}
