#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include <string.h>
#define W(p,o) (*(int*)((char*)(p)+(o)))
#define P(p,o) (*(void**)((char*)(p)+(o)))
#define A(p,o) ((void*)((char*)(p)+(o)))
typedef void *(*Alloc)(char*,int);
typedef void (*Free)(char*,void*,int);
#define F(p,o) (*(Free*)((char*)(p)+(o)))
typedef struct {Alloc alloc;Free free;void *other;} Cbs;
typedef struct {int a,b;} Pair;
typedef struct {char *name,*value;} Entry;
typedef struct {char pad[0x1a38];Entry entries[32];} Dict;
extern void ov00_021E9754(void*),ov00_021E9210(void*),ov00_021E7DD4(void),ov00_021E7D48(void),ov00_021EA660(void*);
extern u32 ov00_021EBF90(char*,u32,char*,u32);
extern int ov00_021EBBEC(void);
extern int ov00_021E9F2C(void*,int),ov00_021EB5B0(char*);
extern char ov00_02216B64[],ov00_02216B6C[],ov00_02216B78[],ov00_02216B84[],ov00_02216B8C[],ov00_02216B9C[];
extern int errno;
extern RTCResult RTC_GetDateTime(RTCDate*,RTCTime*);
extern char ov00_02216AF0[],ov00_02216AC0[],ov00_0221A734[],ov00_02216BA8[];
extern u32 ov00_021EBE08(u8*,u32,char*,u32);
extern long atol(const char*);
extern char ov00_0221A6B8[],ov00_0221A748[],ov00_0221A74C[],ov00_022168A4[],ov00_022168C0[],ov00_02216904[],ov00_02216918[],ov00_02216928[],ov00_02216938[],ov00_02216954[],ov00_02216974[],ov00_02216B10[],ov00_02216B20[];
extern char *ov00_021EA1CC(void*p,char*s);
extern u32 ov00_021EA224(void*p,char*s,char*out,u32 n);
extern int ov00_021EA278(void*p,char*s,char*out,int n);
extern void ov00_021EA2B8(void*p,void*arr,int n);
extern int ov00_021EA340(Cbs *cb);
extern void ov00_021EA420(void);
extern void ov00_021EA518(void);
extern int ov00_021EA580(void);
extern int ov00_021EA5C0(void);
extern void ov00_021EA5D8(void);
extern void ov00_021EB56C(int n);
extern int ov00_021EB7CC(Alloc a,Free b);
extern void ov00_021EB864(void);
extern int ov00_021EBB18(void);
extern int ov00_021EC0FC(int*p);
extern int ov00_021EC11C(int*p,int*q);
extern void ov00_021EC210(void);
extern int ov00_021EC230(void);
extern void ov00_021EC24C(int a,int b);

u32 ov00_021EBF90(char *src,u32 n,char*out,u32 cap){int bits,bytes,count;u32 i;char *dst;char tmp[4];char *q;int j;if(n&3)return -1;bits=0;for(i=0;i<n;i++)if(src[i]!='*')bits+=6;bytes=bits/8;if(!out)return bytes;if(cap<(u32)bytes)return -1;if(!n){*out=0;return 0;}dst=out;do{for(j=0,q=tmp;j<4;j++,q++){int c=src[j];if(c>='A'&&c<='Z')*q=c-'A';else if(c>='a'&&c<='z')*q=c-'G';else if(c>='0'&&c<='9')*q=c+4;else if(c=='.')*q=62;else if(c=='-')*q=63;else *q=0;}dst[0]=(tmp[0]<<2)|(tmp[1]>>4);src+=4;count=dst+1-out;if(count>=bytes)break;dst[1]=(tmp[1]<<4)|(tmp[2]>>2);count=dst+2-out;if(count>=bytes)break;dst[2]=(tmp[2]<<6)|tmp[3];dst+=3;count=dst-out;}while(count<bytes);return count;}

int ov00_021EC0FC(int*p){if(p)*p=W(ov00_0221A74C,4);return W(ov00_0221A74C,0);}

int ov00_021EC11C(int*p,int*q){if(p)*p=W(ov00_0221A74C,4);if(q){switch((u32)W(ov00_0221A74C,0)){case 2:case 3:case 4:case 5:case 8:*q=6;break;case 6:*q=3;break;case 7:*q=4;break;case 10:case 11:case 12:case 13:*q=1;break;case 1:case 9:*q=7;break;case 14:*q=5;break;case 15:case 17:case 19:*q=6;break;case 16:case 18:*q=2;break;default:*q=0;break;}}return W(ov00_0221A74C,0);}

void ov00_021EC210(void){if(W(ov00_0221A74C,0)!=9){W(ov00_0221A74C,0)=0;W(ov00_0221A74C,4)=0;}}

int ov00_021EC230(void){return W(ov00_0221A74C,0)!=0;}

void ov00_021EC24C(int a,int b){if(W(ov00_0221A74C,0)!=9){((Pair*)ov00_0221A74C)->a=a;((Pair*)ov00_0221A74C)->b=b;}}
