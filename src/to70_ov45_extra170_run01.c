#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <stdarg.h>
#pragma force_active on
#pragma warn_uninitializedvar off
#define V(p,o) (*(volatile u32*)((p)+(o)))
#define W(p,o) (*(u32*)((p)+(o)))
#define H(p,o) (*(u16*)((p)+(o)))
typedef union S {struct{u32 flag:1;u32 cap:31;u32 len;char*ptr;}l;struct{u8 flag:1;u8 len:7;char data[11];}s;} S;
typedef struct Vec {S*data;u32 len,cap,z,extra;} Vec;
typedef struct Node {struct Node*left,*right,*parent;u32 key;} Node;
typedef struct Small {u8 x;} Small;
typedef struct E {int key,val;} E;
extern void ov45_02237800(Vec*,u32,Small);
extern S*ov45_02238024(S*,u32,u32,char*,u32);
extern S*ov45_02238104(S*,u32,u32,char*,char*,Small);
extern void*ov45_02242394(u32);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
extern u8*ov45_02231C78(S*);
extern int ov00_021EC2EC();
extern int ov45_022377AC();
extern int ov45_02237BC8();
void ov45_02237EB0(S*p,S*q,u32 pos);
u32 ov45_02237EE4(S*p,char*q,u32 pos,u32 len);
void ov45_02237F7C(S*p,S*q);
void ov45_02237FD8(S*p,char*q);
void* ov45_02238450(Node*p,u32*key,Node**out,u8*less,u8*empty);
void ov45_022384BC(S*p,char*a,char*b);
void ov45_02238558(void*ctx,Node*p);
void ov45_022387E4(Vec*p,u32 n,Small tag);
void ov45_02238ACC(Vec*p,u32 n,Small tag);
Vec* ov45_0223895C(Vec*p);
Vec* ov45_022389B0(Vec*p);
void ov45_022389CC(Vec*p,u32 n,Small tag);
void ov45_02238A3C(Vec*p,u32 n,Small tag);
u8* ov45_02238B08(u8*p);
S* ov45_02238C08(Vec*p);
void ov45_02238C1C(S*p,u8*q);
int ov45_02238B70(u8*p,u8*out,u32*size);
u32 ov45_022385A4(Vec*p,u32 n,...);
void ov45_02239ABC(E**out,E*first,E*last,int*key,...);
void ov45_02239B3C(E**out,E*first,E*last,int*key,...);
void ov45_022383E0(Vec*p,char*first,char*last);
u8* ov45_02238508(u8*p);
u8* ov45_022389EC(u8*p);
Vec* ov45_02238A74(Vec*p);

#pragma optimization_level 3
void ov45_022387E4(Vec*p,u32 n,Small tag) {S*end=p->data+p->len;p->len-=n;if(!n)return;do{end--;ov45_02237BC8(end);}while(--n);}
