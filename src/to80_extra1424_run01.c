#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
struct String{union{struct{u32 flag:1;u32 spare:31;}bits;char bytes[4];}head;u32 cap;char *text;};
struct State{int a,state;};
struct Callback{void(*first)(char*,int,void*);void(*second)(char*,int,void*);u8 pad[12];void *arg;};
extern struct State *ov45_022577DC;
extern void ov45_02237B58(struct String*,char*),ov45_02237BC8(struct String*),ov45_022428D8(struct String*,struct State*,struct String*);
extern int ov45_022435F8(struct State*,struct String*),ov45_02243280(struct State*,struct String*,struct String*);
extern struct Callback *ov45_02242C3C(struct State*,struct String*);
void ov45_0223F5C8(int,char*,char*);
static inline char *getText(struct String*s){if(s->head.bits.flag==0)return s->head.bytes+1;return s->text;}
void ov45_0223F5C8(int unused,char*name,char*value){struct String a,b,c,d,e,f;int id;struct Callback *cb;if(ov45_022577DC&&ov45_022577DC->state==5)return;ov45_02237B58(&a,value);id=ov45_022435F8(ov45_022577DC,&a);ov45_02237BC8(&a);if(id==-1)return;ov45_02237B58(&b,name);cb=ov45_02242C3C(ov45_022577DC,&b);ov45_02237BC8(&b);ov45_02237B58(&c,name);ov45_022428D8(&d,ov45_022577DC,&c);cb->second(getText(&d),id,cb->arg);ov45_02237BC8(&d);ov45_02237BC8(&c);ov45_02237B58(&e,value);ov45_02237B58(&f,name);ov45_02243280(ov45_022577DC,&e,&f);ov45_02237BC8(&f);ov45_02237BC8(&e);}
