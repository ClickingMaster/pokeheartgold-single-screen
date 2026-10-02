#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern int sprintf(char *,const char *,...);
#include <string.h>
#pragma force_active on
#define I(p,o) (*(int *)((char *)(p)+(o)))
#define U(p,o) (*(u32 *)((char *)(p)+(o)))
#define B(p,o) (*(u8 *)((char *)(p)+(o)))
typedef struct Node { u32 x[8]; struct Node *next; } Node;
typedef struct List { Node *first,*last; int count; } List;
extern void *ov00_02219D70;
extern char ov00_02219D74[],ov00_02219EDC[],ov00_02219EE0[],ov00_02219EF0[],ov00_02219F10[],ov00_0221A4FC[],ov00_0221A500[],ov00_0221A508[],ov00_0221A4E4[];
extern void *ov00_0221A0B8[];
extern int ov00_0221B0D0[],ov00_0221B9D0[];
extern u32 ov00_021FF368(void);
extern void ov00_021FF818(int);
extern void ov00_021FF3D0(int);
extern void ov00_021FF3CC(void);
extern int ov00_021FF804(int,int,int);
extern void ov00_021EC3D8(void *);
extern void ov00_02210BA8(void *);
extern void ov00_02210AFC(void *);
extern void ov00_02211DFC(void *,int);
extern void ov00_02211D9C(void *);
extern void ov00_022116C0(void *,void *,int);
extern void ov00_02212404(void *,Node *);
extern void ov00_022128C4(void *,int);
extern int ov00_0221321C(void *);
extern int strnicmp(const char *,const char *,int);
extern void *ov00_021FED4C(int,int,int,int (*)(void **),int (*)(const char **,const char **,int),void (*)(void **));
extern int ov00_021FEF00(void *);
extern void ov00_021FEE8C(void *);
extern void ov00_021FEF74(void *,void *);
extern char *ov00_02213EE8(int,char *);
extern void *ov00_021FF0C0(void *,void *);
extern u8 ov00_022121EC(u8 *,u8);
extern void ov00_02212A58(void *);
extern void __msl_assertion_failed(const char *,const char *,const char *,int);
void ov00_02210A80(void *p,int v);
void ov00_02210A98(void *p,int v);
void ov00_02210AB0(void *p,int v);
void ov00_02210AC8(void *p);
void ov00_02210CBC(void *p);
void ov00_02210D18(void *p);
void ov00_02210D9C(void *p,int v);
void ov00_02210DC8(void *p,int v);
void ov00_02210DFC(void *p,const char *s);
void ov00_02210F74(u8 *a,u8 *b);
u8 ov00_02210F88(u32 n);
void ov00_022111D8(void *p,int v,u8 *s);
void ov00_02211898(void *p,void *b);
int ov00_02211A24(void *p,int v);
void ov00_02212008(int n,void *p);
void ov00_022120DC(u8 *p);
void ov00_022122B8(u8 *p,u8 *b,int n);
void ov00_022122F4(List *p,Node *n);
void ov00_02212328(List *p,Node *n);
Node *ov00_02212350(List *p);
int ov00_02212384(List *p,Node *n);
void ov00_022123F0(List *p);
void ov00_022125E4(void *p,int v,int w,int flag,int cb,int data);
void ov00_02212658(void *p,int v);
void ov00_02212660(void *p);
void ov00_0221267C(void *p);
void ov00_022126A8(void *p,Node *n,int first,int mode);
int ov00_022128BC(void);
void ov00_02212AE8(void *p);
void ov00_02212B28(void *p);
void ov00_02212B88(void *p,int v);
void ov00_02212BA8(void *p,Node *n);
int ov00_02212BD4(void **p);
int ov00_02212BE4(const char **a,const char **b,int n);
void ov00_02212BF8(void **p);
void ov00_02212C6C(void);
void ov00_02212CAC(void **p);
void ov00_02212CD0(void *p,char *key,char *v);
void ov00_02212D10(void *p,char *key,int v);
void *ov00_02212D48(void *p,char *key,void *fallback);
extern char ov00_0221B8D0[],ov00_02219EA4[];
extern int ov00_021FF694(int);
extern int ov00_021FF940(int,void *,int,int,void *,int *);
extern void ov00_02211A88(void *,void *,int,void *);
extern void ov00_021FF984(int,void *,int,int,void *,int);
extern void ov00_02211358(void *,void *,int,int,void *);
extern void ov00_02211094(u8 *,int,u8 *,int);
void ov00_02210A80(void *p,int v);
void ov00_02210A98(void *p,int v);
void ov00_02210AB0(void *p,int v);
void ov00_02210AC8(void *p);
void ov00_02210CBC(void *p);
void ov00_02210D18(void *p);
void ov00_02210D9C(void *p,int v);
void ov00_02210DC8(void *p,int v);
void ov00_02210DFC(void *p,const char *s);
void ov00_02210F74(u8 *a,u8 *b);
u8 ov00_02210F88(u32 n);
void ov00_022111D8(void *p,int v,u8 *s);
void ov00_02211898(void *p,void *b);
int ov00_02211A24(void *p,int v);
void ov00_02212008(int n,void *p);
void ov00_022120DC(u8 *p);
void ov00_022122B8(u8 *p,u8 *b,int n);
void ov00_022122F4(List *p,Node *n);
void ov00_02212328(List *p,Node *n);
Node *ov00_02212350(List *p);
int ov00_02212384(List *p,Node *n);
void ov00_022123F0(List *p);
void ov00_022125E4(void *p,int v,int w,int flag,int cb,int data);
void ov00_02212658(void *p,int v);
void ov00_02212660(void *p);
void ov00_0221267C(void *p);
void ov00_022126A8(void *p,Node *n,int first,int mode);
int ov00_022128BC(void);
void ov00_02212AE8(void *p);
void ov00_02212B28(void *p);
void ov00_02212B88(void *p,int v);
void ov00_02212BA8(void *p,Node *n);
int ov00_02212BD4(void **p);
int ov00_02212BE4(const char **a,const char **b,int n);
void ov00_02212BF8(void **p);
void ov00_02212C6C(void);
void ov00_02212CAC(void **p);
void ov00_02212CD0(void *p,char *key,char *v);
void ov00_02212D10(void *p,char *key,int v);
void *ov00_02212D48(void *p,char *key,void *fallback);
void ov00_02210AFC(void *p);
void ov00_02210BA8(void *p);
void ov00_02210FD0(u8 *p,int n,u8 *out);
void ov00_02211208(void *p,void *b,char *s,int n);
void ov00_022115B0(void *p,void *b,int n0,void *a0,int n1,void *a1,int n2,void *a2);
void ov00_02211608(void *p,void *b,u8 *s,int n);
void ov00_02211D9C(void *p);
u8 ov00_02212028(u8 *p,u32 max,u8 *key,u32 len,u8 *v,u32 *pos);
u8 ov00_022121EC(u8 *p,u8 v);
void ov00_02212A58(void *p);
void *ov00_02212C08(void);

typedef struct Host { u32 x[3]; u8 **list; } Host;
typedef struct Addr { u8 b[2]; u16 port; u32 ip; } Addr;
typedef struct Four { u8 b[4]; } Four;
typedef struct Range { u8 pad[0xd8]; int items[10]; int pos; } Range;
typedef struct State { u8 pad[0x2c]; u8 keys[20]; int count; } State;
extern Host *ov00_021FF6E4(void *);
extern Host *SOC_GetHostByName(char *);
extern u32 ov00_021FFA44(char *);
extern int ov00_0221B878;
extern Four ov00_0221B87C[];
extern int sscanf(const char *,const char *,...);
extern char ov00_0221A4CC[];
extern void ov00_02212FC4(Node *,char *);
extern int ov00_02213FD4(void *,int);
extern void ov00_02213044(Node *,void *,int);
void ov00_02210A80(void *p,int v);
void ov00_02210A98(void *p,int v);
void ov00_02210AB0(void *p,int v);
void ov00_02210AC8(void *p);
void ov00_02210AFC(void *p);
void ov00_02210BA8(void *p);
void ov00_02210CBC(void *p);
void ov00_02210D18(void *p);
void ov00_02210D9C(void *p,int v);
void ov00_02210DC8(void *p,int v);
void ov00_02210DFC(void *p,const char *s);
void ov00_02210F74(u8 *a,u8 *b);
u8 ov00_02210F88(u32 n);
void ov00_02210FD0(u8 *p,int n,u8 *out);
void ov00_022111D8(void *p,int v,u8 *s);
void ov00_02211208(void *p,void *b,char *s,int n);
void ov00_022115B0(void *p,void *b,int n0,void *a0,int n1,void *a1,int n2,void *a2);
void ov00_02211608(void *p,void *b,u8 *s,int n);
void ov00_02211898(void *p,void *b);
int ov00_02211A24(void *p,int v);
void ov00_02211D9C(void *p);
void ov00_02212008(int n,void *p);
u8 ov00_02212028(u8 *p,u32 max,u8 *key,u32 len,u8 *v,u32 *pos);
void ov00_022120DC(u8 *p);
u8 ov00_022121EC(u8 *p,u8 v);
void ov00_022122B8(u8 *p,u8 *b,int n);
void ov00_022122F4(List *p,Node *n);
void ov00_02212328(List *p,Node *n);
Node *ov00_02212350(List *p);
int ov00_02212384(List *p,Node *n);
void ov00_022123F0(List *p);
void ov00_022125E4(void *p,int v,int w,int flag,int cb,int data);
void ov00_02212658(void *p,int v);
void ov00_02212660(void *p);
void ov00_0221267C(void *p);
void ov00_022126A8(void *p,Node *n,int first,int mode);
int ov00_022128BC(void);
void ov00_02212A58(void *p);
void ov00_02212AE8(void *p);
void ov00_02212B28(void *p);
void ov00_02212B88(void *p,int v);
void ov00_02212BA8(void *p,Node *n);
int ov00_02212BD4(void **p);
int ov00_02212BE4(const char **a,const char **b,int n);
void ov00_02212BF8(void **p);
void *ov00_02212C08(void);
void ov00_02212C6C(void);
void ov00_02212CAC(void **p);
void ov00_02212CD0(void *p,char *key,char *v);
void ov00_02212D10(void *p,char *key,int v);
void *ov00_02212D48(void *p,char *key,void *fallback);
void ov00_02210E58(void *p);
int ov00_02210ECC(char *s,int port,Addr *a,Host **hp);
void ov00_022112A8(void *p,char *s);
void ov00_02212124(u8 *p,u8 *key,u32 n);
void ov00_02212824(void *p,Node *n,char *s);
void ov00_02212724(void *p,Node *n,u8 *s,int len);

u8 ov00_02212028(u8 *p,u32 max,u8 *key,u32 len,u8 *v,u32 *pos) { u32 count,mask,r; if(!max)return 0; count=0; mask=1; while(mask<max)mask=(mask<<1)+1; do { u32 at=(*pos)++; *v=p[*v]+key[at]; if(*pos>=len) { *pos=0; *v+=len; } r=mask&*v; if(++count>11)r%=max; } while(r>max); return r; }

void ov00_022120DC(u8 *p) { int i,j; p[256]=1; p[257]=3; p[258]=5; p[259]=7; p[260]=11; for(i=0,j=255;i<256;i++,j--)p[i]=j; }
void ov00_02212124(u8 *p,u8 *key,u32 n) { struct { u8 val; u32 pos; } state; int i; if(n<1) { ov00_022120DC(p); return; } for(i=0;i<256;i++)p[i]=i; state.pos=0; state.val=0; for(i=255;i>=0;i--) { u8 idx=ov00_02212028(p,i,key,n,&state.val,&state.pos); u8 tmp=p[i]; p[i]=p[idx]; p[idx]=tmp; } p[256]=p[1]; p[257]=p[3]; p[258]=p[5]; p[259]=p[7]; p[260]=p[state.val]; state.val=0; state.pos=0; }

u8 ov00_022121EC(u8 *p,u8 v) { u8 old=p[256]++; u8 tmp; p[257]+=p[old]; tmp=p[p[260]]; p[p[260]]=p[p[257]]; p[p[257]]=p[p[259]]; p[p[259]]=p[p[256]]; p[p[256]]=tmp; p[258]+=p[tmp]; p[259]=v^p[(u8)(p[p[258]]+p[p[256]])]^p[p[(u8)(p[p[257]]+(p[p[259]]+p[p[260]]))]]; p[260]=v; return p[259]; }

void ov00_022122B8(u8 *p,u8 *b,int n) { int i; for(i=0;i<n;i++)b[i]=ov00_022121EC(p,b[i]); }
void ov00_022122F4(List *p,Node *n) { if(p->last)p->last->next=n; p->last=n; n->next=0; if(!p->first)p->first=n; p->count++; }
void ov00_02212328(List *p,Node *n) { n->next=p->first; p->first=n; if(!p->last)p->last=n; p->count++; }
Node *ov00_02212350(List *p) { Node *n=p->first; if(n) { p->first=n->next; if(!p->first)p->last=0; p->count--; } return n; }
int ov00_02212384(List *p,Node *n) { Node *cur=p->first,*prev=0; while(cur) { if(cur==n) { if(prev)prev->next=cur->next; if(p->first==cur)p->first=cur->next; if(p->last==cur)p->last=prev; p->count--; return 1; } prev=cur; cur=cur->next; } return 0; }
void ov00_022123F0(List *p) { p->last=0; p->first=0; p->count=0; }
