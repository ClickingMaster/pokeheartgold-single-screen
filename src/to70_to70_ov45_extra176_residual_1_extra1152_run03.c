#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include <string.h>
void ov45_02249554(u8 *,int,u8 *);
void ov45_022495DC(u8 *,int,u8 *);
void ov45_0224966C(s8 *,int,s8 *);
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
extern char ov45_02255938[],ov45_02255918[],ov45_02255874[],ov45_02255A30[],ov45_02255B7C[];
extern char ov45_022565C0[],ov45_022565D0[],ov45_0225609C[],ov45_02256630[],ov45_02256638[];
extern void __msl_assertion_failed(char*,char*,char*,int);
extern u8 *ov00_021FF0C0(u32,char*);
extern int strnicmp(char*,char*);
void ov45_02249390(u8*,char**);
u8 *ov45_0224983C(u8*,char*);
u8 *ov45_0224983C(u8 *c,char *name){u8 *p;if(!c)__msl_assertion_failed(ov45_022565C0,ov45_022565D0,ov45_0225609C,0x17f);if(!name)__msl_assertion_failed(ov45_02256630,ov45_022565D0,ov45_0225609C,0x181);if(!*name)__msl_assertion_failed(ov45_02256638,ov45_022565D0,ov45_0225609C,0x182);p=(u8*)W(c,0x814);while(p){if(W(p,0)==12&&strnicmp((char*)W((void*)W(p,0x1c),8),name)==0)return p;p=(u8*)W(p,0x24);}return 0;}
