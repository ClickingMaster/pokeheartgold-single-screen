#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <string.h>
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define B(p,o) (*(u8 *)((u8 *)(p)+(o)))
extern u8 *ov00_0221A784;
extern void *ov00_0221A78C[32];
extern u32 ov00_0221A80C[32][2];
extern struct {u32 pad; u8 *state;} ov00_0221A90C;
extern struct {u32 pad[2]; u8 *state;} ov00_0221A918;
#define L (ov00_0221A90C.state)
#define F (ov00_0221A918.state)
extern u8 *ov00_0220D2E0(void *);

extern int ov00_021F35E4(u8 **);
extern void *ov00_021F3398(int);
extern void ov00_021EC24C(int,int);
extern void ov00_021EF6F4(void *,void *);
extern void ov00_021EF508(void *,void *);
extern void ov00_021EF8C0(void);
extern int ov00_021FFE20(void *,const char *,const char *,int,int,void *,void *);
extern int ov00_021EFCB0(void *,u32,u32,char *);
extern int ov00_021EBF90(void *,int,void *,int);
extern int ov00_021EBE08(void *,int,void *,int);
extern int ov00_021F04A0(int,void *,int);
extern void ov00_021F0EF8(void);
extern void ov00_021F10A8(void);
extern char ov00_02217A10[];
extern void ov00_02200020(void *,u32,int,int,void *,void *);
extern void ov00_02200400(void *,char *,void *);
extern int DWC_GetGsProfileId(void *,void *);
extern int ov00_021FFD9C(void *);
extern void ov00_021E7D48(void);
extern void ov00_021E7DD4(void);
extern void ov00_021EC2EC(int,void *,int);
extern void ov00_021F2EE4(int,int);
#pragma warn_uninitializedvar off
void *ov00_021EE64C(int id) ;
int ov00_021EE6B0(void *p) ;
int ov00_021EE6C0(void *p) ;
int ov00_021EE6D0(void) ;
void ov00_021EE6FC(void) ;
void **ov00_021EE72C(int i) ;
void **ov00_021EE73C(u32 id,int n) ;
u32 *ov00_021EE7A4(int i) ;
int ov00_021EE7B4(int id) ;
void ov00_021EE7FC(void) ;
void ov00_021EE800(int n) ;
void *ov00_021EE820(int id) ;
u32 ov00_021EE880(u8 *p,int n) ;
void ov00_021EEA74(int a,int b) ;
void ov00_021EEAD4(int a,int b) ;
void ov00_021EF1F0(void *p,void *a,void *b,void *c,void *d,void *e,void *f,void *g) ;
void ov00_021EF274(void) ;
void *ov00_021EF3A4(void) ;
void ov00_021EF3C0(int a,int b) ;
void ov00_021EF410(void) ;
void ov00_021EF46C(void) ;
int ov00_021EF494(u32 a) ;
void ov00_021EF640(const char *a,const char *b) ;
void ov00_021EF658(const char *a,const char *b,void *cb,int state) ;
int ov00_021EFC6C(void) ;
int ov00_021EFC98(void *p,char *s) ;
int ov00_021EFDCC(void *p,char *s,int *n) ;
int ov00_021EFDE8(void *p,u32 a,u32 b,char *s,int *n) ;
int ov00_021EFF04(void *cb,void *arg) ;
int ov00_021EFF30(void *p,int n,void *out,int size) ;
int ov00_021EFF3C(void *p,int n,void *out,int size) ;
void ov00_021F0238(int a,int b) ;
void ov00_021F0290(void *p,u32 *q) ;
int ov00_021F02DC(void *p,u32 *q) ;
int ov00_021F03BC(int i) ;
int ov00_021F0404(int id) ;
void ov00_021F0468(void) ;
void ov00_021F05D8(void) ;
void ov00_021F0678(void) ;
void ov00_021F0914(void) ;
void ov00_021F094C(u8 *p,int i,int value) ;

extern int ov00_021EE8B4(u32);
extern int ov00_021F9A10(int,int,int,int);
extern int ov00_021F9A4C(int,int,int,int);
void ov00_021EEC94(void *unused,u32 *p);
int ov00_021EEE34(int a,int b,int c,int e);
int ov00_021EF1A8(int a,int b,int c,int e);
int ov00_021F05EC(void);
int ov00_021EFE78(void *p,int n);
void ov00_021F0338(void *p,u32 *q);
void ov00_021F0550(int i);
#pragma warn_unusedvar off

extern int ov00_0220D294(void *);
extern int ov00_021FFA60(int);
int ov00_021EE8B4(u32 a);
int ov00_021EE9D0(u32 a);
void ov00_021EF1B4(void *p);

u32 ov00_021EE880(u8 *p,int n) { u32 mask=0; int i; for(i=0;i<n;i++)mask|=1<<p[i]; return mask; }
