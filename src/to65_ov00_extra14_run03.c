#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include <string.h>
#define W(p,o) (*(int *)((char *)(p)+(o)))
#define P(p,o) (*(void **)((char *)(p)+(o)))
#define B(p,o) (*(u8 *)((char *)(p)+(o)))
#define H(p,o) (*(u16 *)((char *)(p)+(o)))
#define A(p,o) ((void *)((char *)(p)+(o)))
typedef void *(*Alloc)(char*,int);
typedef void (*Free)(char*,void*,int);
extern char ov00_02216420[],ov00_0221A694[],ov00_02216474[],ov00_022166F8[],ov00_02216710[],ov00_022167B8[],ov00_022167CC[],ov00_022167E0[];
extern void ov00_021EA2B8(void*,void*,int);
extern int CPS_Resolve(void*);
extern void CPS_SocRegister(void*);
extern void CPS_SslAddRandomSeed(void*,int);
extern void ov00_021E8048(void*),ov00_021E9438(void*);
extern char ov00_0221A6B4[],ov00_0221A69C[];
typedef struct {char *base,*cursor,*end;int size;} Buf;
extern void ov00_021E7B60(void *x);
extern void ov00_021E7C94(void);
extern void ov00_021E7D48(void);
extern void ov00_021E7DD4(void);
extern void ov00_021E7E2C(void);
extern int ov00_021E7E5C(void);
extern void ov00_021E7EA8(void *out);
extern void ov00_021E8940(int n);
extern void ov00_021E9154(void *p,int n);
extern void ov00_021E9210(void *p);
extern int ov00_021E926C(void *p);
extern void ov00_021E92E4(void *p);
extern int ov00_021E933C(void *p);
extern void ov00_021E9754(void *p);
extern int ov00_021E9B38(int n);
extern int ov00_021E9B44(void *p,Buf *b,int n);
extern void ov00_021E9B9C(void *p,void *b);
extern int ov00_021E9BDC(void *p,void *b,int n);

extern char ov00_02216464[],ov00_0221A6A0[],ov00_02216680[],ov00_02216698[],ov00_022166B0[],ov00_022166BC[],ov00_022166C4[],ov00_022166CC[],ov00_022166D0[],ov00_022166E0[],ov00_022166F4[],ov00_02216728[],ov00_0221677C[],ov00_0221679C[],ov00_022167B4[];
extern void DWCi_BM_GetWiFiInfo(void*);
extern int ov00_021E89AC(void*,void*);
extern int ov00_021E9C84(void*,void*);
extern long atol(const char*);
typedef struct {u32 v[18];} Copy72;
typedef struct {u32 v[7];} Copy28;
extern int ov00_021E7F38(int);
extern int ov00_021E7B70(void *settings,void *state);
extern int ov00_021E8984(void *out);
extern int ov00_021E8FA0(void *p,void *settings);
extern int ov00_021E90BC(void *p);
extern int ov00_021E9350(void *p);
extern int ov00_021E9808(void *p);
extern int ov00_021E98C8(void *p,char *name,char *value);
extern int ov00_021E9AB0(void *p,char *s);
void ov00_021E8940(int n){OS_LockMutex((OSMutex*)A(P(ov00_0221A694,8),0x13d8));W(P(ov00_0221A694,8),0x1004)=n;OS_UnlockMutex((OSMutex*)A(P(ov00_0221A694,8),0x13d8));}

int ov00_021E8984(void *out){char tmp[20];DWCi_BM_GetWiFiInfo(tmp);return ov00_021E89AC(out,tmp);}
