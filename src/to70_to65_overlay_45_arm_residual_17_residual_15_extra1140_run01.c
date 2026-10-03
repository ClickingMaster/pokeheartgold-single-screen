#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {void (*callback)(void *,int,s8 *,int,int,int,void *);void *data;} Callback;
extern char ov45_02256BA0[],ov45_02256B94[],ov45_02256A90[],ov45_02256E9C[],ov45_02256EAC[],ov45_02257018[],ov45_02257028[],ov45_02257038[];
extern void __msl_assertion_failed(const char *,const char *,const char *,int);
void ov45_022510D4(void *,s8 *,int,int,int,Callback *);
void ov45_022510D4(void *owner,s8 *name,int length,int arg,int extra,Callback *volatile cb){if(owner==0)__msl_assertion_failed(ov45_02256BA0,ov45_02256B94,ov45_02256A90,2066);if(name==0)__msl_assertion_failed(ov45_02256E9C,ov45_02256B94,ov45_02256A90,2070);if(*name==0)__msl_assertion_failed(ov45_02256EAC,ov45_02256B94,ov45_02256A90,2070);if(length<0)__msl_assertion_failed(ov45_02257018,ov45_02256B94,ov45_02256A90,2071);if(cb==0)__msl_assertion_failed(ov45_02257028,ov45_02256B94,ov45_02256A90,2087);{Callback *context=cb;if(context->callback==0)__msl_assertion_failed(ov45_02257038,ov45_02256B94,ov45_02256A90,2092);context->callback(owner,1,name,length,arg,extra,context->data);}}
extern char ov45_022569F8[],ov45_02256FA0[],ov45_02256C2C[],ov45_02257050[];
extern int ov45_02248238(void *,s8 *),ov45_0224A978(void *,s8 *,void *,int),ov45_0224F660(void *,int);extern void ov45_022480EC(void *,s8 *,void (*)(void *,s8 *,int,int,int,Callback *),void *),ov45_02252F74(void *,char *,s8 *),ov45_0224F770(void *,int),ov00_021FF3C0(int);
void ov45_02251208(void *,s8 *,void *,int,int);
void ov45_02251208(void *owner,s8 *name,void *callback,int extra,int wait){struct {void *callback;int extra;} args;int result;if(owner==0)__msl_assertion_failed(ov45_02256BA0,ov45_02256B94,ov45_022569F8,2119);if(owner==0||*(void**)owner==0)return;if(callback==0)__msl_assertion_failed(ov45_02256FA0,ov45_02256B94,ov45_022569F8,2123);if(name==0)name=(s8*)ov45_02256C2C;if(*name&&ov45_02248238(owner,name)){args.callback=callback;args.extra=extra;ov45_022480EC(owner,name,ov45_022510D4,&args);return;}ov45_02252F74((u8*)owner+28,ov45_02257050,name);if(*name==0)name=0;result=ov45_0224A978(owner,name,callback,extra);if(wait){do{ov45_0224F770(owner,result);ov00_021FF3C0(10);}while(ov45_0224F660(owner,result));}}
