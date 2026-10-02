#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include <string.h>
#define W(p,o) (*(int *)((char *)(p)+(o)))
#define P(p,o) (*(void **)((char *)(p)+(o)))
#define B(p,o) (*(char *)((char *)(p)+(o)))
#define A(p,o) ((void *)((char *)(p)+(o)))
extern void __msl_assertion_failed(char*,char*,char*,int);
extern char *strchr(const char*,int);
extern int sprintf(char*,const char*,...);
extern void *ov00_021EC3A8(int);
extern void ov00_021EC3D8(void*),ov00_021FF3D0(void),ov00_021FF3C0(int);
extern int ov45_0224F770(void*,int),ov45_0224F660(void*,int),ov45_0224A7F0(void*,void*,int,int),ov45_02248238(void*,char*),ov45_022487BC(void*,char*);
extern void ov45_0224A6B4(void*),ov45_022479F0(void*),ov45_02244BCC(void*),ov45_02252CB4(void*,char*),ov45_02252AA0(void*),ov45_022522B4(void*),ov45_02247D84(void*,char*);
extern int ov45_02252F74(void*,char*,...);
extern void ov00_021FF59C(void*,int*,int*,int*);
extern char ov45_022569C0[],ov45_022569D8[],ov45_022569E8[],ov45_022569F8[],ov45_02256A18[],ov45_02256A2C[],ov45_02256A40[],ov45_02256A68[],ov45_02256A7C[],ov45_02256A90[],ov45_02256AA4[],ov45_02256AB8[],ov45_02256ACC[],ov45_02256AE0[],ov45_02256AF4[],ov45_02256B0C[],ov45_02256B3C[],ov45_02256B54[],ov45_02256B6C[],ov45_02256B94[],ov45_02256BA0[],ov45_02256BC0[],ov45_02256C2C[],ov45_02256C48[],ov45_02256DD0[],ov45_02256DDC[],ov45_02256DF4[],ov45_02256E04[],ov45_02256E64[],ov45_02256E94[],ov45_02256E9C[],ov45_02256EAC[],ov45_02256EC0[],ov45_02256ECC[],ov45_02256ED8[],ov45_02256F50[],ov45_02256F60[],ov45_02256F78[],ov45_02256F88[],ov45_02256F94[],ov45_02256FA0[],ov45_02256FB4[],ov45_02256FC4[],ov45_02256FD8[],ov45_02256FDC[],ov45_02256FE8[],ov45_02256FF0[],ov45_02256FFC[],ov45_0225700C[],ov45_02257018[],ov45_02257028[],ov45_02257038[],ov45_02257050[],ov45_0225705C[],ov45_0225706C[],ov45_0225707C[],ov45_02257094[],ov45_0225709C[],ov45_022570A4[],ov45_022570B8[],ov45_022570C4[],ov45_022570D4[],ov45_022570E4[],ov45_022570FC[],ov45_0225710C[],ov45_02257124[],ov45_0225713C[],ov45_02257140[],ov45_02257174[],ov45_02257194[],ov45_022571D4[],ov45_022571E4[],ov45_022571F8[],ov45_0225720C[],ov45_02257234[],ov45_02257248[],ov45_02257258[],ov45_02257268[],ov45_0225727C[],ov45_02257290[],ov45_022572B4[],ov45_022572CC[],ov45_022572F0[],ov45_022572FC[],ov45_0225730C[],ov45_02257324[],ov45_02257334[],ov45_02257344[],ov45_022573C4[],ov45_022573E4[],ov45_02257408[],ov45_02257438[],ov45_0225745C[],ov45_0225749C[],ov45_022574BC[],ov45_022574E0[],ov45_02257514[],ov45_02257538[],ov45_02257578[],ov45_02257590[],ov45_0225759C[],ov45_022575B0[],ov45_022575D8[],ov45_022577F0[],ov45_022577F4[];
extern void ov45_02251B0C(void*,int,char*,int,int);
extern void ov45_0225035C(void *p,char *name);
extern void ov45_02250424(void *p,int x,char *name,char *other);
extern void ov45_022504E0(void *p);
extern int ov45_02250590(void *p);
extern void ov45_022505A0(void *p,char *name,void *x,int y,int z,int wait);
extern void ov45_022507D8(void *p,char *name,char *other);
extern void ov45_02250E6C(void *p,char *name,int n);
extern int ov45_02250F5C(void *p,char *name);
extern int ov45_02251024(void *p,char *name);
extern char *ov45_022514F4(void);
extern int ov45_02251A8C(char *name);
extern int ov45_02251BE0(void *p);
extern void ov45_02251C40(void *p);
extern int ov45_02251EF0(void *p);
extern void ov45_02252530(void *p,int *a,int *b,int *c);
extern int ov45_0224FABC(int,void*,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int);
extern void ov45_02244D30(void*,int,void*,void*,void*,int,int,int);
extern int ov45_0224A874(void*,char*,int,int,int,char*);
extern void ov45_02247A44(void*,char*);
extern int ov45_022502D0(void *p,int a,int b,int c,int d,int e,int f,int g,int h,int i,int j,int k);
extern void ov45_02250688(void *p,char *name,char *other,int x,int y,int z,int wait);
extern void ov45_02251554(void *p,char *a,char *b,int n,char **keys,char **values);
extern void *ov00_021EC3BC(void*,int);
extern int ov45_02251C50(void *p,int n);
extern void ov45_02251DB4(void *p,int n);
extern void ov45_02251350(void *p,char *a,char *b,u32 mode);
typedef struct {void *fn;void *arg;} Callback;
extern int ov45_022510D4(void*,char*,int,int,int,Callback*);
void ov45_02252530(void *p,int *a,int *b,int *c){int x=0,y=0,z=0;ov00_021FF59C(p,&x,&y,&z);if(a)*a=x;if(b)*b=y;if(c)*c=z;}
