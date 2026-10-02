#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include <string.h>
#define W(p,o) (*(int *)((char *)(p)+(o)))
#define U(p,o) (*(u32 *)((char *)(p)+(o)))
#define P(p,o) (*(void **)((char *)(p)+(o)))
#define B(p,o) (*(char *)((char *)(p)+(o)))
#define A(p,o) ((void *)((char *)(p)+(o)))
extern void __msl_assertion_failed(char*,char*,char*,int);
extern void *ov00_021EC3BC(void*,int),*ov00_021EC3A8(int);
extern void ov00_021EC3D8(void*),ov00_021EC2EC(int,void*,int),ov00_021FA0D8(void),ov00_021EC24C(int,int);
extern int ov00_021EC230(void);
extern int ov45_0225478C(int,int,int,int,int,int);
extern int ov45_02253528(void*,char*);
extern void ov45_02251DB4(void*,int);
extern char ov45_02257148[],ov45_02257154[],ov45_02257164[],ov45_02257184[],ov45_022571A4[],ov45_022571B4[],ov45_022571C4[],ov45_02257220[],ov45_02257248[],ov45_02257258[],ov45_02257334[],ov45_02257344[],ov45_022573C4[],ov45_022573E4[],ov45_02257408[],ov45_02257438[],ov45_0225745C[],ov45_0225749C[],ov45_022574BC[],ov45_022574E0[],ov45_02257514[],ov45_02257538[],ov45_022575D8[],ov45_022575FC[],ov45_02257600[],ov45_02257610[],ov45_02257620[],ov45_02257624[],ov45_02257628[],ov45_0225762C[],ov45_0225763C[],ov45_0225765C[],ov45_022576A0[],ov45_022576CC[],ov45_022576D4[],ov45_022576DC[],ov45_02257704[],ov45_02257728[],ov45_02257750[],ov45_02257758[],ov45_0225775C[],ov45_02257764[],ov45_02257780[],ov45_022577AC[],ov45_022577F8[],ov45_022587F8[];
extern int ov45_02253968(void*);
extern int ov45_02253374(char *s,void *p);
extern int ov45_022533E4(char *s,void *p);
extern u32 ov45_02253D50(u32 n);
extern void ov45_02253D88(u32 n);
extern int ov45_02253D9C(void);
extern int ov45_02253DDC(char *a,char *b,u32 c,u32 d,u32 e,u32 f);
extern void ov45_022545BC(void);
extern void ov45_022546F8(void);
extern int ov45_02254774(int *p);
extern int ov45_022549E0(int a,int b,int c,int d,int e);
extern int ov45_02254A04(int a,int b,int c,int d,int e);
extern void *ov45_02253B5C(void *p);
extern void ov45_02254088(void);
extern void *ov00_021EC2A8(int,int);
extern void ov00_021FFC18(void*,void*,int,int),ov00_021FA0B4(int);
extern unsigned long strtoul(const char*,char**,int);
extern int ov45_022542F4(u8 *out,u8 *a,int na,u8 *b,int nb);
extern void ov45_0225443C(int mode,char *s);
extern int ov45_02253968(void *p);
extern void ov45_0225408C(void);
extern int ov00_021FA488(void*,int,int,void(*)(void),void(*)(void),int),ov00_021FA118(void);
extern int ov45_022545F0(void);
int ov45_02253374(char *s,void *p){void *arr;char *dup;int n;arr=ov00_021EC3BC(P(p,32),(W(p,36)+1)*4);if(!arr)return 0;P(p,32)=arr;dup=ov00_021EC3A8(strlen(s)+1);if(!dup)return 0;strcpy(dup,s);n=W(p,36)++;((char**)P(p,32))[n]=dup;return 1;}
