#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 pad[0x214];int id;char first[256],second[256];} D;
typedef struct {D *d;} P;
typedef struct {int state;P *p;u8 pad[0x10];char *entries;u8 pad2[0x18];void (*cb)(int,int,char*,void*);void *arg;u8 pad3[8];void (*cb2)(int,void*);void *arg2;} S;
extern struct {u32 pad[2];S *s;} ov00_0221A918;
extern int ov00_021EFC6C(S*),ov00_02200654(P*,int,char*,char*),ov00_021EFC98(void*,char*);
int ov00_021F04A0(int,char*,char*);void ov00_021F0550(int);
int ov00_021F04A0(int id,char *a,char *b){if(!ov00_0221A918.s||!ov00_021EFC6C(ov00_0221A918.s))return 0;if(id==-1)id=ov00_0221A918.s->p->d->id;if(!a)a=ov00_0221A918.s->p->d->first;if(!b)b=ov00_0221A918.s->p->d->second;return ov00_02200654(ov00_0221A918.s->p,id,a,b);}
void ov00_021F0550(int id){struct{char unused[264];char text[264];} buf;if(ov00_0221A918.s->cb2&&ov00_0221A918.s->state!=1)ov00_0221A918.s->cb2(id,ov00_0221A918.s->arg2);if(!ov00_0221A918.s->cb)return;{int n=ov00_021EFC98(ov00_0221A918.s->entries+12*id,buf.text);ov00_0221A918.s->cb(id,n,buf.text,ov00_0221A918.s->arg);} }
