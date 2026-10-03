#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <string.h>
extern int sprintf(char *,const char *,...);
typedef struct {u8 pad[0x18];int value;} Lookup;
typedef struct {int state;u32 a,b;int id;u8 pad[0x28];void *handle;} Work;
typedef struct {volatile int a,b,c,d,id,end;} Request;
extern int ov00_0220657C(void *,int,int,Lookup **,int,int,int);extern int ov00_02205E50(void *,int,int);extern int ov00_022020EC(void *,Request *,char *);extern int ov00_0220211C(void *,Request *,int);extern int ov00_02201FE0(void *,Request *,const char *,int);extern int ov00_02201F48(void *,Request *,int);extern void ov00_021FE754(void *,Request *);extern u32 ov00_021FFA70(int);extern int ov00_02202424(void *,Work *,char *);
extern void __msl_assertion_failed(char *,char *,char *,int);
extern char ov00_02219118[],ov00_022190A8[],ov00_02218F70[],ov00_022191D0[],ov00_022191E0[],ov00_022191E4[],ov00_022191EC[],ov00_022191F4[],ov00_02218F84[],ov00_02219200[];
typedef struct {u8 pad[16];u32 address,port;} Peer;
typedef struct {u8 pad[8];Peer *peer;} Resolved;
typedef struct {u8 len,family;u16 port;u32 address;} SocketAddress;
extern int ov00_02207DD0(void *,int,Resolved **);extern void ov00_0220A890(void *,char *);extern int ov00_021FF804(int,int,int);extern void ov00_0220A86C(void *,int,char *);extern void ov00_02202990(void *,int,int);extern int ov00_021FF44C(int,int);extern void ov00_02207370(int);extern int ov00_021FF8A0(int,SocketAddress *,int);extern int ov00_021FFA60(int);
extern char ov00_02219008[],ov00_0221914C[],ov00_02219174[],ov00_022191A8[];
int ov00_02207610(void *,Work *);
static inline void InitRequest(Request *r){r->id=0;r->a=0;r->b=0;r->c=0;r->d=0;r->end=0;}
int ov00_022075AC(void *,Work *);int ov00_0220779C(void *,Work *,int,const char *);int ov00_0220792C(void *,Work *,int,int *);
int ov00_0220779C(void *context,Work *w,int id,const char *text){int length,result;Request request;if(w==NULL)__msl_assertion_failed(ov00_02219118,ov00_022190A8,ov00_02218F70,0x340);if(text==NULL)__msl_assertion_failed(ov00_022191D0,ov00_022190A8,ov00_02218F70,0x341);length=strlen(text);memset((void*)&request,0,sizeof(request));request.id=id;result=ov00_022020EC(context,&request,ov00_022191E0);if(result)return result;result=ov00_0220211C(context,&request,id);if(result)return result;result=ov00_022020EC(context,&request,ov00_022191E4);if(result)return result;result=ov00_0220211C(context,&request,length);if(result)return result;result=ov00_022020EC(context,&request,ov00_022191EC);if(result)return result;request.end=request.c;result=ov00_02201FE0(context,&request,text,length);if(result)return result;result=ov00_02201F48(context,&request,0);if(result)return result;ov00_021FE754(w->handle,&request);*(u32 *)((u8 *)w+0x10)=ov00_021FFA70(0)+300;return 0;}

#pragma force_active on
