#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <string.h>
#pragma force_active on
typedef struct {u8 len,family;u16 port;u32 address;} Address;
typedef struct {char *name;char **aliases;u16 type,length;u32 **addresses;} Host;
typedef struct {int socket;u32 state;char name[256];u8 read[12],write[12];} Connection;
#define I(p,o) (*(int*)((u8*)(p)+(o)))
extern const char ov45_02257334[],ov45_02257258[],ov45_022571D4[],ov45_02257344[],ov45_022573C4[],ov45_022573E4[],ov45_02257408[],ov45_02257438[],ov45_0225745C[],ov45_0225749C[],ov45_022574BC[],ov45_022574E0[],ov45_02257514[],ov45_02257538[],ov45_02257578[],ov45_02257590[],ov45_0225759C[],ov45_022575B0[];
extern void __msl_assertion_failed(const char*,const char*,const char*,int);
extern u32 ov00_021FFA44(const char*);
extern Host *SOC_GetHostByName(const char*);
extern int ov00_021FF804(int,int,int);
extern int ov00_021FF8A0(int,const Address*,int);
extern int ov00_021FF818(int);
#define ASSERT(c,s,line) do{if(!(c))__msl_assertion_failed(s,ov45_02257258,ov45_022571D4,line);}while(0)
int ov45_02251F6C(Connection*,const char*,int);
int ov45_02251F6C(Connection *p,const char *host,int port){u32 ip;Address addr;
 ASSERT(p,ov45_02257334,216);ASSERT(p->state<=2,ov45_02257344,216);
 ASSERT((u32)p+0x108,ov45_022573C4,216);ASSERT(I(p,0x110)>=0,ov45_022573E4,216);ASSERT(I(p,0x110)%8192==0,ov45_02257408,216);ASSERT(I(p,0x10c)>=0,ov45_02257438,216);ASSERT(I(p,0x10c)<=I(p,0x110),ov45_0225745C,216);
 ASSERT((u32)p+0x114,ov45_0225749C,216);ASSERT(I(p,0x11c)>=0,ov45_022574BC,216);ASSERT(I(p,0x11c)%8192==0,ov45_022574E0,216);ASSERT(I(p,0x118)>=0,ov45_02257514,216);ASSERT(I(p,0x118)<=I(p,0x11c),ov45_02257538,216);
 ASSERT(host,ov45_02257578,217);ASSERT(port>=0,ov45_02257590,218);ASSERT(port<=65535,ov45_0225759C,219);ASSERT(p->state==0,ov45_022575B0,220);
 strncpy(p->name,host,255);p->name[254]=0;ip=ov00_021FFA44(host);if(ip==(u32)-1){Host *entry=SOC_GetHostByName(host);if(!entry)return 0;ip=*entry->addresses[0];}
 memset(&addr,0,8);addr.family=2;addr.port=((u16)port>>8 &255)|((u16)port<<8 &0xff00);addr.address=ip;p->socket=ov00_021FF804(2,1,0);if(p->socket==-1)return 0;if(ov00_021FF8A0(p->socket,&addr,8)!=-1){p->state=1;return 1;}ov00_021FF818(p->socket);return 0;}
