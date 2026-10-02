#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#pragma force_active on
int OS_SNPrintf(char*,int,const char*,...);int strlen(const char*);void MI_CpuCopy8(const void*,void*,u32);int ov00_02200880(void*,void*,char*);extern char ov00_02217B6C[],ov00_02217B5C[],ov00_02217B64[];int ov00_021F45E0(void*,int,void*,char*);
struct Inner{void*obj;};struct Net{int a;struct Inner*inner;u8 gap[0xe4-8];void*sock;};struct Request{u8 flag,pad;u16 value;void*arg,*data;};struct Net*ov00_021F955C(void);void*ov00_0220F724(void*,int,int);void ov00_022138C4(void*,void*,int,void*);int ov00_021F82EC(void);void*ov00_0220D294(void*);int ov00_022100D4(void*,void*,int,void*,void*,void*);extern u8 ov00_021F9104[],ov00_021F9100[];int ov00_021F4248(struct Request*);
int ov00_021F4248(struct Request*r){int i,result;if(r->flag==0){struct Net*n=ov00_021F955C();void*x=ov00_0220F724(r->arg,0,0);ov00_022138C4(n->sock,x,r->value,r->data);if(ov00_021F82EC()!=0)return 2;}for(i=0;i<5;i++){void*x=ov00_0220D294(ov00_021F955C()->inner->obj);result=ov00_022100D4(x,r->data,r->flag,ov00_021F9100,ov00_021F9104,r);if(result==0)return result;if(result!=3)return result;}return result;}
