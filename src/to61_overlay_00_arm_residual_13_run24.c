#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <string.h>
extern long atol(const char *);
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define B(p,o) (*(u8 *)((u8 *)(p)+(o)))
extern int strnicmp(const char *, const char *);
extern double atod(const char *);
extern u16 __msl_digit[];
extern u8 __lower_mapC[];
extern char ov00_0221A514[],ov00_0221A51C[],ov00_0221A5B8[],ov00_0221A5BC[],ov00_0221A5C8[],ov00_0221A530[];
extern char *ov00_022163F0[2];
extern char *ov00_0221B9D0;
extern u32 ov00_0221B9D4;
extern void *ov00_0221B9DC;
extern void ov00_02212CD0(void *,const char *,const char *);
extern char *ov00_02212D48(void *,const char *,char *);
extern void ov00_021EC3D8(void *);
extern void ov00_021FE678(void *);
extern int ArrayLength(void *);
extern void *ov00_021FE6EC(void *,int);
extern void *ov00_021FE5B4(int,int,void *);
extern void ov00_021FEC3C(void *);
extern void ov00_021FE968(void *,int);
extern void ov00_021FE754(void *,void *);
extern void ov00_021FF01C(void *,void *);
extern void ov00_021FEF74(void *,void *);
extern void *ov00_021FF0C0(void *,void *);
extern char *ov00_021FF3D4(const char *);
extern void *ov00_02212C08(void *);
extern void ov00_02212CAC(void *);
extern void ov00_02212B28(void *);
extern void ov00_02212660(void *);
extern void ov00_0221267C(void *);
extern void ov00_02215FE0(void *);
extern void ov00_02212C6C(void *);
extern int ov00_021FF818(int);
extern void __msl_assertion_failed(const char *,const char *,const char *,int);
typedef struct { char *v[2]; } Strings;
extern void ov00_02213BF4(void *,int,char *,u32);
#include <stdarg.h>
int ov00_02212DB4(void *p,const char *key,int def);
double ov00_02212E44(void *p,const char *key,double def);
u16 ov00_02212E80(void *p);
u16 ov00_02212ECC(void *p);
int ov00_02212EAC(void *p);
int ov00_02212F00(const char *s);
char *ov00_02212F60(char *s,int delim);
void ov00_02212FC4(void *p,char *s);
u32 ov00_0221321C(const char *s,u32 div);
u32 ov00_022141EC(const char *s,u32 div);
void ov00_02213274(void *p);
u32 ov00_02213298(char **p,u32 div);
int ov00_022132A8(char **p,char **q);
void ov00_02213378(void *p,u32 a,u16 b);
int ov00_0221339C(void *p);
void ov00_022136E4(void *p);
void ov00_0221390C(void *p,void *q);
void ov00_02213938(void *p);
void ov00_02213950(void *p);
void ov00_0221396C(void *p);
int ov00_02213984(void *p);
void * ov00_022139B8(void *p,int i);
int ov00_022139C8(void *p);
void ov00_022139D8(void *p,int a,char *b,u32 c);
int ov00_022139F0(void **p,void **q);
int ov00_02213B24(void **p,void **q);
int ov00_02213B8C(void **p,void **q);
void ov00_02213C6C(void *p,void *q,...);
int ov00_02213CA8(void *p,void *q);
int ov00_02213CFC(void *p,u32 a,u32 b);
void ov00_02213D6C(void *p,void *q);
void ov00_02213DA0(void *p,int i);
int ov00_02213DEC(void *p);
void *ov00_02213DFC(void *p,int i);
void ov00_02213E10(void *p);
void ov00_02213E68(void *p);
void ov00_02213EC0(void *p);
char *ov00_02213EE8(void *p,const char *s);
void ov00_02213F58(void *p,void *s);
int ov00_02213FD4(const char *p,int n);
void ov00_02214370(char **p,const char *s,int *n);
void ov00_022143C0(char **p,int v,int *n);
void ov00_022143E4(u8 **p,u32 v,int *n,...);
void ov00_02214438(char **p,const void *s,int len,int *n);
void ov00_02214860(void *p);
void ov00_022148A4(void *p);
void ov00_02214908(void *p);
void ov00_0221496C(void *p);
int ov00_02214A44(int flags);
int ov00_02214A70(char *p,int n);
u32 ov00_02212E78(void *p);
u16 ov00_02212EA4(void *p);
u32 ov00_02212EC4(void *p);
void ov00_02212EF0(void *p,u32 v);
u32 ov00_02212EF8(void *p);
u32 ov00_022132BC(void *p);
void ov00_02213370(void *p,u32 v);
void ov00_02213384(void *p,u32 v);
void ov00_0221338C(void *p,u32 v);
u8 ov00_02213394(void *p);
u32 ov00_022139E8(void *p);
void ov00_0221505C(void *p,u32 v);

extern void *ov00_021EC3A8(u32);
extern void *ov00_021FED4C(int,int,int,void *,void *,void *);
extern void ov00_02214004(void *,const char *,const char *,const char *,int,int,void *,void *);
extern void ov00_022125E4(void *,int,int,int,void *,void *);
extern int ov00_02213708(void *,int,int,const void *,int,int,int,int);
extern u32 ov00_021FFA44(const char *);
extern int ov00_02215CDC(void *,u32,u16,int,int);
extern int ov00_02215E04(void *,u32,u16,int);
extern void ov00_021FEA20(void *,void *);
extern void ov00_02212D10(void *,const char *,int);
extern void ov00_02212124(void *,void *,int);
extern char ov00_0221A550[],ov00_0221A638[];
extern int ov00_0221B0D0;
void *ov00_022132C4(void *unused,u32 a,u16 b);
void ov00_02213570(void *unused,int event,void *q,void *p);
void *ov00_02213610(const char *a,const char *b,const char *c,int d,int e,int f,int g,void *cb,void *arg);
extern void ov00_022133B8(void *,int,void *,void *);
int ov00_02213838(void *p,int a,int b,const void *c,int d,int e,int f);
int ov00_0221386C(void *p,const char *addr,int port,int a,int b);
int ov00_022138C4(void *p,const char *addr,int port,int a);
int ov00_02213A4C(void **p,void **q);
void ov00_02213BF4(void *p,int dir,char *key,u32 type);
void ov00_022149A4(char *p,char *s,int n);
int ov00_02214AF4(void *p,u8 *s,int n);
void ov00_022143E4(u8 **p,u32 v,int *n,...);

int ov00_02214A44(int flags) { int n=5; if(flags&2)n+=4; if(flags&8)n+=4; if(flags&16)n+=2; if(flags&32)n+=2; return n; }

int ov00_02214A70(char *p,int n) { int k; while(n>0 && *p) { k=ov00_02213FD4(p,n); if(k<0)return 0; n-=k; p+=k; k=ov00_02213FD4(p,n); if(k<0)return 0; p+=k; n-=k; } if(n==0)return 0; return *p==0; }

int ov00_02214AF4(void *p,u8 *s,int n) { int len=ArrayLength((void *)W(p,8)); int i; for(i=0;i<len;i++){void *q=ov00_021FE6EC((void *)W(p,8),i); switch(W(q,4)){case 1:s++;n--;break;case 2:s+=2;n-=2;break;case 0:if(n<1)return 0; n--; if(*s++==255){int k=ov00_02213FD4((char *)s,n); if(k==-1)return 0;s+=k;n-=k;}break;default:__msl_assertion_failed(ov00_0221A638,ov00_0221A5C8,ov00_0221A550,0x317);return 0;}if(n<0)return 0;}return 1; }
