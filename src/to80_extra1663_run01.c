#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u8 pad[0x10c];int protocol;u8 p2[0xc4];int sock,state;u8 p3[0x28];int listen,port;} Work;
typedef struct {Work*work;} Ctx;
typedef struct {u8 pad[0x14];int state;} Req;
typedef struct {struct{u8 len,family;u16 port;u32 address;} f;} Addr;
typedef struct {void*name,*aliases;int type;u32**addresses;} Host;
int ov00_021FF804(int,int,int);
int ov00_021FF44C(int,int);
int ov00_021FF840(int,Addr*,int);
int ov00_021FF8EC(int,int);
int ov00_021FFA18(int,Addr*,int*);
int ov00_021FF8A0(int,Addr*,int);
int ov00_021FFA60(int);
Host *SOC_GetHostByName(const char*);
void ov00_0220A86C(Ctx*,int,char*);
void ov00_02202990(Ctx*,int,int);
void __msl_assertion_failed(char*,char*,char*,int);
extern char ov00_0221872C[],ov00_02218754[],ov00_02218788[],ov00_022187B0[],ov00_022187DC[],ov00_022186AC[],ov00_0221880C[],ov00_02218840[],ov00_02218860[],ov00_0221868C[],ov00_02218870[];
int ov00_02202E70(Ctx*,Req*);
#define FAIL(msg) {ov00_0220A86C(c,5,msg);ov00_02202990(c,3,1);return 3;}
int ov00_02200A98(Ctx*);
void ov00_0220A890(Ctx*,char*);
void ov00_0220A348(void*,const char*,int);
void ov00_021FF408(char*);
void*ov00_021EC3A8(int);
void*memset(void*,int,u32);
typedef struct {u8 p[8];int active;u8 q[8];int state;int handle,error;} Result;
int ov00_0220657C(Ctx*,int,void*,Result**,void*,void*,void*);
void ov00_02206230(Ctx*,Result*);
void ov00_02204284(Ctx*,int);
int ov00_02200ED8(Ctx*,int);
extern char ov00_02218898[],ov00_022188AC[],ov00_022188C0[];
int ov00_022031E4(Ctx*,char*,char*,char*,char*,char*,char*,char*,int,int,void*,void*,void*);
#include <string.h>
void ov00_021FFB40(u32);
int ov00_021FFB5C(int,int);
void ov00_021FFC18(char*,char*,u32,int);
void ov00_022020EC(Ctx*,void*,const char*);
void ov00_0220211C(Ctx*,void*,int);
extern char ov00_022189C8[],ov00_022189D4[],ov00_022189DC[],ov00_022189E4[],ov00_02218990[],ov00_0221899C[],ov00_0221B16C[],ov00_022189A8[],ov00_0221893C[],ov00_022189F4[],ov00_022189B8[],ov00_022189C0[];
int ov00_022037CC(Ctx*,u8*);
int ov00_02202E70(Ctx*c,Req*r){
 Work*w=c->work;Addr a;int size;
 if(w->protocol==0){

 w->listen=ov00_021FF804(2,1,0);if(w->listen==-1)FAIL(ov00_0221872C)
 if(!ov00_021FF44C(w->listen,0))FAIL(ov00_02218754)
 {u32*p=(u32*)&a;p[0]=0;p[1]=0;a.f.family=2;
 if(ov00_021FF840(w->listen,&a,8)==-1)FAIL(ov00_02218788)
 if(ov00_021FF8EC(w->listen,5)==-1)FAIL(ov00_022187B0)
 size=8;if(ov00_021FFA18(w->listen,&a,&size)==-1)FAIL(ov00_022187DC)
 w->port=a.f.port;}
 }else{w->listen=-1;w->port=0;}
 w->sock=ov00_021FF804(2,1,0);if(w->sock==-1)FAIL(ov00_0221872C)
 if(!ov00_021FF44C(w->sock,0))FAIL(ov00_02218754)
 {Host*h=SOC_GetHostByName(ov00_022186AC);
 if(!h)FAIL(ov00_0221880C)
 {u32*p=(u32*)&a;p[0]=p[1]=0;a.f.family=2;a.f.address=**h->addresses;
 if(!a.f.address)__msl_assertion_failed(ov00_02218840,ov00_02218860,ov00_0221868C,0x90);
 a.f.port=0xcc74;
 if(ov00_021FF8A0(w->sock,&a,8)==-1){int e=ov00_021FFA60(w->sock);if(e!=-6&&e!=-26&&e!=-76)FAIL(ov00_02218870)}
 }}
 r->state=1;w->state=1;return 0;
}
