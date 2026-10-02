#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#define W(p,n) (*(int*)((u8*)(p)+(n)))
#define U(p,n) (*(unsigned*)((u8*)(p)+(n)))
#define P(p,n) (*(void**)((u8*)(p)+(n)))
typedef union{struct{u32 flag:1;u32 pad:31;u32 len;char *data;}b;struct{char head;char inlinebuf[11];}s;} Str;
extern void *ov45_022577DC;
extern char ov45_022552DC[],ov45_022552E4[],ov45_022552EC[],ov45_022552F0[],ov45_022552FC[];
void ov45_02237B58(Str*,char*);void ov45_02237BC8(Str*);void ov45_02237A24(void*,void*);void ov45_022376BC(void*);
void ov45_02232D8C(void*,int*,int);void ov45_02242858(void*);void ov00_021EC2EC(int,void*,int);
void ov45_022436E0(void*,Str*,Str*);void ov45_0224335C(void*,Str*);void ov45_0223DAD4(Str*,char*);void *ov45_02242C3C(void*,Str*);void ov45_022428D8(Str*,void*,Str*);void ov45_022428C8(Str*,void*,Str*);int ov45_02251024(void*,char*);
void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);void *ov45_02242394(unsigned);
void *ov45_0223EA34(void*,unsigned,char*);void ov45_022421E4(void*,unsigned,void*);
char *strncpy(char*,const char*,size_t);
int ov45_022435F8(void*,Str*);
void *ov45_0223EC64(void*p);
void ov45_0223EC80(void*p,unsigned n);
void *ov45_0223ECB0(void*p);
void ov45_0223ED00(void);
void ov45_0223F1C8(void);
void ov45_0223F8F4(void);
void ov45_0223F910(void);
void ov45_0223ED1C(void);
void ov45_0223F2D8(void*a,char*s);
void ov45_0223F6F4(void*a,void*b,char*c,char*d);
void ov45_0223F768(void*a,void*b,void*c,unsigned mode);
void ov45_0223F83C(void*a,char*s,void*b);
void ov45_0223F92C(void*a,int n);
void ov45_0223F9C4(void*a,void*b,char*out);
void ov45_0223FA2C(void*out,void*p);
void ov45_0223FC5C(void*out,void*p);
void ov45_02240F6C(void*out,void*p);
void ov45_02240F7C(void*out,void*p);
int ov45_0223FD6C(void*p);
void *ov45_0223FD74(void*p);
void ov45_0223FD88(void);
void *ov45_0223FDE8(void*c,void*key,void*node,void*best);
void ov45_0223FD8C(void**out,void*tree,void*key);
void ov45_0223FE20(void*a,int n);
void ov45_0223FFE8(void*p,u32*first,u32*last);
int ov45_02240850(void);
int ov45_02240A58(void);
void *ov45_02240A74(void*p);
int ov45_02240A80(char*s);
void ov45_0223F1E4(void*a,char*s,char*name,void*arg,int mode);
void ov45_02240620(void*a,char*s,char*name,void*arg,void*extra);
void *ov45_0223EC64(void*p){W(p,4)=0;ov45_0223ECB0(p);return p;}

void ov45_0223EC80(void*p,unsigned n){if(n>0x3fffffff)_ZNSt15__vector_commonILb1EE18throw_length_errorEv();P(p,0)=ov45_02242394(n*4);U(p,8)=n;}
