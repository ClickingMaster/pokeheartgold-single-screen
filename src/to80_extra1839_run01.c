#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{u32 a,b,c;}String;
typedef struct{int a,b,type,kind,arg,spare;String text;}Message;
typedef struct{u8 pad[20];void(*cb0)(int,int,int,void*);void(*cb1)(int,int,int,void*,u32);u8 gap[156];String first;u8 gap2[12];String second;u8 gap3[12];u8 map[4];}Context;
extern Context*_022577C4;extern void ov45_02237B58(String*,void*),ov45_022320B4(String*,String*),ov45_02237BC8(String*),ov45_022379D4(String*),ov45_0223454C(Message*,int,int,void*,u32);extern int ov45_022340F8(String*,String*),ov45_0223D27C(String*,Message*);extern u8*ov45_0223DEC4(void*,int);extern void*ov45_02231C78(String*);
void ov45_02236090(void*,int,void*);
static inline void zeroString(String*s){u32*p=(u32*)s;*p++=0;*p++=0;*p=0;}
#pragma force_active on
#pragma opt_lifetimes off
#pragma opt_common_subs off
static inline int validEntry(u8*p){if(p && *(u32*)(p+56))return 1;return 0;}
void ov45_02236090(void*key,int index,void*name){Message m;String k,tmp,b,a;int language=5;if(key){Context*c;String*kp;int match;ov45_02237B58(&k,key);c=_022577C4;kp=&k;ov45_022320B4(&a,&c->first);match=ov45_022340F8(&a,kp);ov45_02237BC8(&a);if(match)language=c->first.a;else{ov45_022320B4(&b,&c->second);match=ov45_022340F8(&b,kp);ov45_02237BC8(&b);if(match)language=c->second.a;else language=5;}ov45_02237BC8(&k);}if(key && language==5)return;{int match;int zero=0;m.a=zero;m.b=6;m.type=0;m.kind=0;m.arg=0;m.spare=0;zeroString(&m.text);ov45_02237B58(&tmp,name);match=ov45_0223D27C(&tmp,&m)==0?1:zero;ov45_02237BC8(&tmp);if(match){ov45_022379D4(&m.text);return;}switch(m.kind){case 1:{u8*p=ov45_0223DEC4(_022577C4->map,index);if(!validEntry(p)){ov45_022379D4(&m.text);return;}switch(m.type){case 1:{Context*c=_022577C4;void*t=ov45_02231C78(&m.text);c->cb1(index,language,m.arg,t,m.text.b);break;}case 0:{Context*c=_022577C4;void*t=ov45_02231C78(&m.text);c->cb0(index,language,m.arg,t);break;}}break;}case 0:{void*t=ov45_02231C78(&m.text);ov45_0223454C(&m,index,language,t,m.text.b);break;}}ov45_022379D4(&m.text);}}
