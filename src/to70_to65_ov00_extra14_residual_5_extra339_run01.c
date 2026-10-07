#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#pragma force_active on
struct Buf{char*start,*pos,*end;int x;};struct C{u8 gap[0x19f8];struct Buf buf;};int strlen(const char*);int OS_SNPrintf(char*,int,const char*,...);int ov00_021E9BDC(struct C*,struct Buf*,int);extern char ov00_022167B4[];int ov00_021E9AB0(struct C*,char*);
struct View{u8 gap[0x1008];int flag;u8 gap2[0x1124-0x100c];char*s1,*s2;};extern char ov00_02216728[],ov00_0221677C[];int ov00_021E9B44(struct C*,struct Buf*,int);int ov00_021E9808(struct C*);
struct Clean{u8 g0[0x1014];void(*release)(char*,void*,int);u8 g1[0x19cc-0x1018];void*a,*b;};void ov00_021EA2B8(struct C*,void*,int);void ov00_021E9B9C(struct C*,void*);void MI_CpuFill8(void*,int,u32);extern char ov00_022166F8[],ov00_02216710[];void ov00_021E9754(void*);
void ov00_021E9754(void*parent){struct C*c=parent;void(*release)(char*,void*,int)=((struct Clean*)c)->release;if(c==0)return;ov00_021EA2B8(c,(u8*)c+0x1a38,32);ov00_021E9B9C(c,(u8*)c+0x1a08);ov00_021E9B9C(c,&c->buf);if(((struct Clean*)c)->a){release(ov00_022166F8,((struct Clean*)c)->a,0);((struct Clean*)c)->a=0;}if(((struct Clean*)c)->b){release(ov00_02216710,((struct Clean*)c)->b,0);((struct Clean*)c)->b=0;}MI_CpuFill8(c,0,0x1c14);}
