#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 pad[0x10c];int protocol;u8 p2[0xc4];int sock,state;u8 p3[0x28];int listen,port;} Work;
typedef struct {Work*work;} Ctx;
typedef struct {u8 pad[0x14];int state;} Req;
typedef union {struct{u8 len,family;u16 port;u32 address;} f;u32 words[2];} Addr;
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
int ov00_022031E4(Ctx*c,char*a,char*b,char*d,char*password,char*user,char*secret,char*optional,int protocol,int value,void*x,void*y,void*z){
 Work*w=c->work;u8*buf;Result*out;int ret;
 if(w->state==4){ret=ov00_02200A98(c);if(ret)return ret;}
 if(w->state){ov00_0220A890(c,ov00_02218898);return 2;}
 switch(protocol){case 1:w->protocol=1;break;case 0:w->protocol=0;break;default:ov00_0220A890(c,ov00_022188AC);return 2;}
 w->protocol=1;
 ov00_0220A348((u8*)w+0x110,a,0x1f);ov00_0220A348((u8*)w+0x12f,b,0x15);ov00_0220A348((u8*)w+0x144,d,0x33);ov00_0220A348((u8*)w+0x177,password,0x1f);ov00_021FF408((char*)w+0x144);
 buf=ov00_021EC3A8(0x308);if(!buf){ov00_0220A890(c,ov00_022188C0);return 1;}
 memset(buf,0,0x308);*(int*)(buf+0x304)=value;
 if(*user&&*secret){ov00_0220A348(buf+0xc2,user,0x100);ov00_0220A348(buf+0x1c2,secret,0x100);}
 if(optional)ov00_0220A348(buf+0x2c2,optional,0x41);
 ret=ov00_0220657C(c,0,buf,&out,x,y,z);if(ret)return ret;
 ret=ov00_02202E70(c,(Req*)out);if(ret){out->error=ret;ov00_02206230(c,out);ov00_02204284(c,0);return ret;}
 if(out->active){ret=ov00_02200ED8(c,out->handle);if(ret)return ret;}
 return 0;
}
