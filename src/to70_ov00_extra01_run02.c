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

void ov00_021EB56C(int n){OS_LockMutex(A(P(ov00_0221A6B8,0),0x11dc));W(P(ov00_0221A6B8,0),0x1000)=n;OS_UnlockMutex(A(P(ov00_0221A6B8,0),0x11dc));}

int ov00_021EB5B0(char*s){RTCDate date;RTCTime time;char buf[32];s64 now,then;int i;int n=strlen(s);if(n!=(int)strlen(ov00_02216AF0)||s[7]!=32||s[11]!=32||s[16]!=32||s[19]!=58||s[22]!=58||s[25]!=32)goto fail;if(RTC_GetDateTime(&date,&time)!=0)goto fail;now=RTC_ConvertDateTimeToSecond(&date,&time);if(now==-1)goto fail;strcpy(buf,s);buf[7]=0;buf[11]=0;buf[16]=0;buf[19]=0;buf[22]=0;buf[25]=0;errno=0;date.year=atol(buf+12);if(errno==34)goto fail;date.year-=2000;date.month=13;for(i=0;i<12;i++)if(!strcmp(((char**)ov00_02216AC0)[i],buf+8)){date.month=i+1;break;}if(date.month>12)goto fail;errno=0;date.day=atol(buf+5);if(errno==34)goto fail;errno=0;time.hour=atol(buf+17);if(errno==34)goto fail;errno=0;time.minute=atol(buf+20);if(errno==34)goto fail;errno=0;time.second=atol(buf+23);if(errno==34)goto fail;then=RTC_ConvertDateTimeToSecond(&date,&time);if(then==-1)goto fail;*(s64*)(ov00_0221A734+12)=now-then;*(s64*)(ov00_0221A734+4)=now;W(ov00_0221A734,0)=1;return 1;fail:return 0;}

int ov00_021EB7CC(Alloc a,Free b){if(P(ov00_0221A748,0)){ov00_021EC24C(9,-24100);return 0;}P(ov00_0221A748,0)=a(ov00_02216B10,0x1c20);if(!P(ov00_0221A748,0)){ov00_021EC24C(9,-24100);return 0;}MI_CpuFill8(P(ov00_0221A748,0),0,0x1c20);*(Alloc*)P(ov00_0221A748,0)=a;F(P(ov00_0221A748,0),4)=b;return 1;}

void ov00_021EB864(void){if(!P(ov00_0221A748,0))return;ov00_021E9754(A(P(ov00_0221A748,0),8));F(P(ov00_0221A748,0),4)(ov00_02216B20,P(ov00_0221A748,0),0);P(ov00_0221A748,0)=0;}
