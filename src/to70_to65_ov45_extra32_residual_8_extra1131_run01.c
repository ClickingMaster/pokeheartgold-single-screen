#pragma force_active on
#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <string.h>
extern char *strchr(const char *,int);extern int strncmp(const char *,const char *,u32);
typedef struct {u8 p[4];u32 b,c,d,p2,a;char *text;} Info;typedef struct {u8 p[0x20];Info *info;int kind;} Packet;typedef struct {int kind;u8 p[4];u32 a,p2;void *object;u8 p3[4];u32 x,p4,y;} Result;typedef struct {u32 a,b,c;} Row;typedef struct {u32 a,b,c,d;} Four;typedef struct {u32 a,b,c,d,e;} Five;
extern char ov45_0225687C[],ov45_022565D0[],ov45_02256140[];extern void __msl_assertion_failed(char *,char *,char *,int);extern void ov45_02249460(void *,u32,u32,u32);extern Result *ov45_022496AC(void *,int,Row *);extern void ov45_02249A0C(void *,Result *,Four *);extern void ov45_02244D30(void *,int,void *,void *,u32,u32,int,int);void ov45_0224CF0C(void *,Packet *);
void ov45_0224CF0C(void *owner,Packet *p){Row rows[3];Four one;Four two;Five three;u32 b,a,c,d;Result *r;if(p->kind!=8)__msl_assertion_failed(ov45_0225687C,ov45_022565D0,ov45_02256140,2726);if(p->kind!=8)return;c=p->info->c;d=p->info->d;a=p->info->a;b=p->info->b;ov45_02249460(owner,a,c,d);{memset(rows,0,sizeof(rows));rows[0].a=6;rows[1].a=10;rows[2].a=11;rows[0].b=a;rows[0].c=b;rows[1].b=a;rows[2].b=b;r=ov45_022496AC(owner,3,rows);if(r==0)return;if(r->kind==6){d=(u32)p->info->text;c=0;if(strchr((char *)d,64)!=0)c|=2;if(strchr((char *)d,43)!=0)c|=1;one.a=1;one.b=b;one.c=a;one.d=c;ov45_02249A0C(owner,r,&one);}else if(r->kind==10){two.a=1;two.b=a;two.c=c;two.d=d;ov45_02244D30(owner,21,r->object,&two,r->x,r->y,0,16);r->object=0;}else if(r->kind==11&&r->object!=0){three.a=1;three.b=r->a;three.c=a;three.d=c;three.e=d;ov45_02244D30(owner,22,r->object,&three,r->x,r->y,0,20);}}}
