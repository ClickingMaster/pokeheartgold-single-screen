#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u8 p[0x20];int state;u8 q[0x7f8-0x24];void*callback;u8 r[0xc];int arg;} Work;
extern void __msl_assertion_failed(const char*,const char*,const char*,int);extern char ov45_02256BA0[],ov45_02256B94[],ov45_022569A0[],ov45_02256BB0[];
extern void ov45_02252AA0(void*);extern int*ov45_02253B5C(void*);extern void ov45_02244D30(Work*,int,void*,int*,int,int,int,int);extern void ov45_0224F5BC(Work*,int*);extern void ov45_0224F69C(Work*,char*);extern void ov45_0224A578(Work*);extern void ov45_022471A0(Work*,int);
void ov45_0224F770(Work*,int);
void ov45_0224F770(Work*w,int arg){int *p,value;if(!w)__msl_assertion_failed(ov45_02256BA0,ov45_02256B94,ov45_022569A0,154);if(w->state==1){ov45_02252AA0((u8*)w+0x1c);p=ov45_02253B5C((u8*)w+0x1c);while(p){if(w->callback){value=*p;ov45_02244D30(w,0,w->callback,&value,w->arg,0,0,4);}ov45_0224F5BC(w,p);p=ov45_02253B5C((u8*)w+0x1c);}if(w->state==2)ov45_0224F69C(w,ov45_02256BB0);}ov45_0224A578(w);ov45_022471A0(w,arg);}
