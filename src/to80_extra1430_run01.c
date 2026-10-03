#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u32 a,b;} Pair;
typedef struct Context {u8 a[0x1a4];Pair pair;u8 b[0x418-0x1ac];u32 value,flag;u8 c[0x438-0x420];struct Request *head,*tail;} Context;
typedef struct {Context *context;} Handle;
typedef struct Request {void (*callback)(Handle *,void *,u32);u32 value;void *data;int type;u32 extra;struct Request *next;} Request;
typedef struct {u32 value;void *p;Context *context;int flag;} Small;
typedef struct {u32 x[14];int count;void **a,**b;} Many;
extern char ov00_022185DC[],ov00_022185F0[],ov00_022185C4[],ov00_02218600[],ov00_02218618[],ov00_02218648[],ov00_02218658[],ov00_022185B4[],ov00_02218678[];
extern void __msl_assertion_failed(char *,char *,char *,int);extern void *ov00_021EC3A8(u32);extern void ov00_021EC3D8(void *);extern void ov00_0220A890(Handle *,char *);
void ov00_02202990(Handle *,u32,int);int ov00_02202A84(Handle *,Pair,void *,Request *,int,...);void ov00_02202B28(Handle *,Request *);
void ov00_02202990(Handle *h,u32 value,int flag){Context *c=h->context;Pair pair;Small *s;if(!c)__msl_assertion_failed(ov00_022185DC,ov00_022185F0,ov00_022185C4,35);if(!value)__msl_assertion_failed(ov00_02218600,ov00_022185F0,ov00_022185C4,36);if(flag!=1&&flag!=0)__msl_assertion_failed(ov00_02218618,ov00_022185F0,ov00_022185C4,37);if(flag==1)c->flag=1;pair.a=c->pair.a;pair.b=c->pair.b;if(pair.a==0)return;s=ov00_021EC3A8(16);if(s){s->value=value;s->flag=flag;s->p=(void *)c->value;s->context=c;}ov00_02202A84(h,pair,s,0,1);}
int ov00_02202A84(Handle *h,volatile Pair pair,void *data,Request *parent,int type,...){Context *c=h->context;Request *r=ov00_021EC3A8(24);if(!r){ov00_0220A890(h,ov00_02218648);return 1;}r->callback=(void *)pair.a;r->value=pair.b;r->data=data;if(parent)r->extra=((u32 *)parent)[6];else r->extra=0;r->type=type;r->next=0;if(!c->head)c->head=r;if(c->tail)c->tail->next=r;c->tail=r;return 0;}
void ov00_02202B28(Handle *h,Request *r){if(!r->callback)__msl_assertion_failed(ov00_02218658,ov00_022185F0,ov00_022185B4,110);if(!r->data)__msl_assertion_failed(ov00_02218678,ov00_022185F0,ov00_022185B4,111);r->callback(h,r->data,r->value);if(r->type==2){void **p=r->data;ov00_021EC3D8(p[2]);((void **)r->data)[2]=0;}else if(r->type==3){int i;Many *m=r->data;for(i=0;i<m->count;i++){ov00_021EC3D8(m->a[i]);m->a[i]=0;ov00_021EC3D8(m->b[i]);m->b[i]=0;}ov00_021EC3D8(m->a);m->a=0;ov00_021EC3D8(m->b);m->b=0;}else if(r->type==4){void **p=r->data;ov00_021EC3D8(p[3]);p[3]=0;}else if(r->type==7){void **p=r->data;if(p[4]){ov00_021EC3D8(p[4]);p[4]=0;}}else if(r->type==8){void **p=r->data;if(p[2]){ov00_021EC3D8(p[2]);p[2]=0;}}else if(r->type==9){int i;struct {u32 x;int count;void **p;} *p=r->data;for(i=0;i<p->count;i++){ov00_021EC3D8(p->p[i]);p->p[i]=0;}ov00_021EC3D8(p->p);p->p=0;}ov00_021EC3D8(r->data);r->data=0;ov00_021EC3D8(r);}
