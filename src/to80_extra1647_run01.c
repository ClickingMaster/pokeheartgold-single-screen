#pragma force_active on
#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
extern char ov00_02218450[],ov00_02218478[],ov00_0221849C[],ov00_02218410[],ov00_02218394[],ov00_022184C0[];
extern int ov00_021FF970(int,void*,int,int),ov00_021FFA60(int),ArrayLength(void*),ov00_02201F48(void*,void*,int);extern void ov00_0220A86C(void*,int,char*),ov00_02202990(void*,int,int),ov00_0220A3BC(void*,char*,char*),__msl_assertion_failed(char*,char*,char*,int);
int ov00_02202154(void*,int,void*,int,int*,int*,char*),ov00_02202244(void*,u8*,char,...);
int ov00_02202244(void*p,u8*q,char arg,...){u8*ctx=q;int n,close,res;if(I(ctx,0x28)==0)__msl_assertion_failed(ov00_0221849C,ov00_02218410,ov00_02218394,222);if(I(ctx,0x30)-I(ctx,0x34)==0&&ArrayLength(V(ctx,0x38))==0){res=ov00_02202154(p,I(ctx,8),&arg,1,&n,&close,ov00_022184C0);if(res)return res;if(close)return 0;}return ov00_02201F48(p,ctx+0x28,*(s8*)(&arg));}
