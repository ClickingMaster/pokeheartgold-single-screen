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
void ov45_02249390(u8 *c,char **a){u8 *p;if(!c)__msl_assertion_failed(ov45_02255938,ov45_02255918,ov45_02255874,0x41a);if(!a)__msl_assertion_failed(ov45_02255A30,ov45_02255918,ov45_02255874,0x41b);if(!W(c,0x134))__msl_assertion_failed(ov45_02255B7C,ov45_02255918,ov45_02255874,0x424);p=ov00_021FF0C0(W(c,0x134),a[0]);if(!p)return;strncpy((char*)p+0x80,a[1],0x18);p[0x97]=0;strncpy((char*)p+0x98,a[2],0x40);p[0xd7]=0;W(p,0xd8)=1;}
