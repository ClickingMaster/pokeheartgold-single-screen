#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#pragma force_active on
struct Buf{char*start,*pos,*end;int x;};struct C{u8 gap[0x19f8];struct Buf buf;};int strlen(const char*);int OS_SNPrintf(char*,int,const char*,...);int ov00_021E9BDC(struct C*,struct Buf*,int);extern char ov00_022167B4[];int ov00_021E9AB0(struct C*,char*);
struct View{u8 gap[0x1008];int flag;u8 gap2[0x1124-0x100c];char*s1,*s2;};extern char ov00_02216728[],ov00_0221677C[];int ov00_021E9B44(struct C*,struct Buf*,int);int ov00_021E9808(struct C*);
struct Clean{u8 g0[0x1014];void(*release)(char*,void*,int);u8 g1[0x19cc-0x1018];void*a,*b;};void ov00_021EA2B8(struct C*,void*,int);void ov00_021E9B9C(struct C*,void*);void MI_CpuFill8(void*,int,u32);extern char ov00_022166F8[],ov00_02216710[];void ov00_021E9754(struct C*);
int ov00_021E9AB0(struct C*c,char*s){int n;struct Buf*b=&c->buf;n=strlen(s);int avail=b->end-b->pos;int result;if(n>avail){if(!ov00_021E9BDC(c,b,n-avail+1))return 1;avail=b->end-b->pos;}result=OS_SNPrintf(b->pos,avail,ov00_022167B4,s);if(result!=n)return 1;b->pos+=result;return 0;}
