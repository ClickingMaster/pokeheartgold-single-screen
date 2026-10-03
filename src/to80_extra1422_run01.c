#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {char*key;u32 value;}Pair;
typedef struct {u8 p[8];void*context;u8 q[0x20-12];Pair*pair;int type;}Variant;
typedef struct Object {void*callback;u8 p[0x2c-4];void*context;}Object;
typedef struct {u8 p[0x36c];char name[0x800-0x36c];void*callback;u32 q;void*context;}Work;
typedef struct {void*context;u32 value;int type;}Payload;typedef struct {char*name;void*context;u32 value;int type;}Named;
extern char ov45_022565C0[],ov45_022565D0[],ov45_02255EA4[],ov45_022566F0[];extern void __msl_assertion_failed(const char*,const char*,const char*,int);extern int strnicmp(const char*,const char*);
extern Object*ov45_02247E98(Work*,char*);extern void ov45_02244D30(Work*,int,void*,void*,void*,int,char*,int);
void ov45_0224B5E0(Work*,Variant*);
void ov45_0224B5E0(Work*w,Variant*v){Payload plain;Named named;char*name;u32 value;Object*obj;if(!w)__msl_assertion_failed(ov45_022565C0,ov45_022565D0,ov45_02255EA4,1620);if(v->type!=2)__msl_assertion_failed(ov45_022566F0,ov45_022565D0,ov45_02255EA4,1626);if(v->type!=2)return;name=v->pair->key;value=v->pair->value;if(strnicmp(name,w->name)==0){if(!w->callback)return;if(v->context)plain.context=v->context;else plain.context=0;plain.type=4;plain.value=value;ov45_02244D30(w,2,w->callback,&plain,w->context,0,0,12);}else{obj=ov45_02247E98(w,name);if(!obj||!obj->callback)return;named.name=name;if(v->context)named.context=v->context;else named.context=0;named.value=value;named.type=4;ov45_02244D30(w,4,obj->callback,&named,obj->context,0,name,16);}}
