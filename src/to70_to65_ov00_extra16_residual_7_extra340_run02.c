#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#pragma force_active on
int OS_SNPrintf(char*,int,const char*,...);int strlen(const char*);void MI_CpuCopy8(const void*,void*,u32);int ov00_02200880(void*,void*,char*);extern char ov00_02217B6C[],ov00_02217B5C[],ov00_02217B64[];int ov00_021F45E0(void*,int,void*,char*);
struct Inner{void*obj;};struct Net{int a;struct Inner*inner;u8 gap[0xe4-8];void*sock;};struct Request{u8 flag,pad;u16 value;void*arg,*data;};struct Net*ov00_021F955C(void);void*ov00_0220F724(void*,int,int);void ov00_022138C4(void*,void*,int,void*);int ov00_021F82EC(void);void*ov00_0220D294(void*);int ov00_022100D4(void*,void*,int,void*,void*,void*);extern u8 ov00_021F9104[],ov00_021F9100[];int ov00_021F4248(struct Request*);
int ov00_021F45E0(void*a,int c,void*b,char*s){char buf[512];int n=OS_SNPrintf(buf,512,ov00_02217B5C,ov00_02217B64,3,ov00_02217B6C);char*end;buf[n]=c;(buf+1)[n]=0;end=buf+1+n;if(s){u32 size=strlen(s);if(n+2+size>512)size=511-(n+1);MI_CpuCopy8(s,end,size);end[size]=0;}return ov00_02200880(a,b,buf);}
