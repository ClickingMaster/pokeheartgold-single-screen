#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define W(p,o) (*(u32*)((p)+(o)))
#define P(p,o) (*(u8**)((p)+(o)))
extern void __msl_assertion_failed(const char*,const char*,const char*,int);
extern int ov00_021EC3D8();
extern int ov00_021FF368();
extern int ov45_0224A14C();
extern int ov45_0224A708();
extern char ov45_02255E60[];
extern char ov45_02255EC4[];
extern char ov45_02255FC4[];
extern char ov45_02255FD4[];
extern char ov45_022560B0[];
extern char ov45_022565C0[];
extern char ov45_022565D0[];
extern char ov45_02256644[];
extern char ov45_022566A0[];
void ov45_0224966C(char*p,int n,char*key);
void ov45_02249904(u8*p);
void ov45_0224A578(u8*p);
void ov45_0224A5E4(u8*p);
int ov45_0224A630(u8*p,int x);
void ov45_0224A6B4(u8*p);
void ov45_0224A9F8(u8*p,void*x,void*y,void*z);
void ov45_02249904(u8*p) {if(!p)__msl_assertion_failed(ov45_02256644,ov45_022565D0,ov45_02255FC4,403);ov00_021EC3D8(P(p,28));ov00_021EC3D8(P(p,8));ov00_021EC3D8(P(p,12));ov00_021EC3D8(p);}
