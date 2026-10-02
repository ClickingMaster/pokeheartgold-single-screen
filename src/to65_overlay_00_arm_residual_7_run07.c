#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#define W(p,n) (*(int *)((u8 *)(p)+(n)))
#define P(p,n) (*(void **)((u8 *)(p)+(n)))
#define B(p,n) (((u8 *)(p))[n])
void __msl_assertion_failed(char*,char*,char*,int);
void *ov00_021EC3A8(int);void *ov00_021EC3BC(void*,int);void ov00_021EC3D8(void*);void ov00_021FEE8C(void*);
void ov00_02204284(void*,int);void ov00_0220A890(void*,char*);void ov00_0220A348(char*,char*,int);
void *ov00_022074D8(void*);int ov00_02207DD0(void*,int,void**);void *ov00_0220752C(void*,int,int);int ov00_022075AC(void*,void*);int ov00_02207610(void*,void*);int ov00_0220779C(void*,void*,int,char*);int ov00_02207FB8(void*);void ov00_02207E40(void*,void*);
int sprintf(char*,const char*,...);int ov00_02202324(void*,void*,char*,int);
extern char ov00_02217EE4[];
extern char ov00_02217FEC[];
extern char ov00_02218230[];
extern char ov00_0221823C[];
extern char ov00_02218248[];
extern char ov00_02218258[];
extern char ov00_02218260[];
extern char ov00_02218268[];
extern char ov00_022182BC[];
extern char ov00_022182F8[];
extern char ov00_022182FC[];
extern char ov00_022183A8[];
extern char ov00_022183DC[];
extern char ov00_022183F8[];
extern char ov00_02218410[];
extern char ov00_0221841C[];
extern char ov00_0221842C[];
extern char ov00_0221843C[];
extern char ov00_0221844C[];
void ov00_022066D0(void*,int);void ov00_02207F1C(void*,int(*)(void*,void*,int),int);
int ov00_021FF970(int,void*,int,int);int ov00_021FFA60(int);void ov00_0220A86C(void*,int,char*);int ov00_02202990(void*,int,int);void ov00_0220A3BC(void*,char*,...);
int ArrayLength(void*);int strncmp(const char*,const char*,size_t);int ov00_0220A4A0(char*,char*,char*,int);
int ov00_02207B34(void*);int ov00_02200A98(void*);void ov00_021FF3CC(void);unsigned ov00_021FF368(void);void srand(unsigned int);long atol(const char*);
extern char ov00_02217EE4[];
extern char ov00_02217FEC[];
extern char ov00_0221800C[];
extern char ov00_02218230[];
extern char ov00_0221823C[];
extern char ov00_02218248[];
extern char ov00_02218258[];
extern char ov00_02218260[];
extern char ov00_02218268[];
extern char ov00_022182BC[];
extern char ov00_022182F8[];
extern char ov00_022182FC[];
extern char ov00_022183A8[];
extern char ov00_022183C0[];
extern char ov00_022183DC[];
extern char ov00_022183F8[];
extern char ov00_02218410[];
extern char ov00_0221841C[];
extern char ov00_0221842C[];
extern char ov00_0221843C[];
extern char ov00_0221844C[];
extern char ov00_02218450[];
extern char ov00_02218478[];
extern char ov00_0221849C[];
extern char ov00_022184C0[];
extern char ov00_022185A0[];
extern char ov00_022185A8[];
extern char ov00_022185AC[];
extern char ov00_022185B4[];
extern char ov00_022185F0[];
extern char ov00_02218658[];
extern char ov00_02218678[];
char *strchr(const char*,int);
int ov00_02200880(void*c,int a,char*b);
void ov00_02200A3C(void*c);
int ov00_02200A7C(void*a,void*b,int n);
int ov00_02200A98(void*c);
int ov00_0220108C(void*c,void*t);
int ov00_02201AE8(void*c,int a,int b,char*text);
int ov00_02201BC4(void*c,int a,int b,char*d);
int ov00_02201CAC(void*c,int a);
int ov00_02201D78(void*a,void*b,int n);
int ov00_02201F48(void*c,void*b,int ch);
int ov00_02201FE0(void*c,void*b,char*s,int n);
int ov00_022020EC(void*c,void*b,char*s);
int ov00_0220211C(void*c,void*b,int n);
int ov00_02202424(void*c,void*b,char*s);
int ov00_02202938(void*a,void*b);
int ov00_022008F4(void*out,int a,int b);
int ov00_02202154(void*c,int fd,void*buf,int n,int*closed,int*sent,char*tag);
int ov00_02202324(void*c,void*b,char*s,int n);
int ov00_022027FC(void*c,void*b,char**out,int*num,int*len);
void ov00_02202B28(void*c,void*t);
int ov00_022027FC(void*c,void*b,char**out,int*num,int*len){char tmp[16],*end;int n;*out=0;if(W(b,8)<5)return 0;end=strchr(P(b,0),10);if(end){if(strncmp(end-5,ov00_022185A0,5))return 3;*end=0;if(!ov00_0220A4A0(P(b,0),ov00_022185A8,tmp,16))return 3;*num=atol(tmp);if(!ov00_0220A4A0(P(b,0),ov00_022185AC,tmp,16))return 3;n=atol(tmp);if(W(b,8)>(n+1)+(end-(char*)P(b,0))){if(end[n+1])return 3;*out=end+1;*len=n;W(b,12)=(n+1)+(end-(char*)P(b,0))+1;}else *end=10;}return 0;}

int ov00_02202938(void*a,void*b){if(!b||!P(b,0)||!W(b,12))return 0;W(b,8)-=W(b,12);if(W(b,8))memmove(P(b,0),(u8*)P(b,0)+W(b,12),W(b,8));B(P(b,0),W(b,8))=0;W(b,12)=0;return 0;}
