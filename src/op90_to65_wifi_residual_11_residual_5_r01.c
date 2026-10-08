#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern void OSi_ReferSymbol(const void*);
typedef union{u64 value;struct{u32 lo,hi;}words;}Value64;

typedef struct{u8 a,b;u16 mtu;u32 x;u16 port;u8 pad[6];u32 fixedFlag,allocWord,receiveWord;u8 gap[0x24-0x1c];u32 sleepInterval;u32 zero28,x2c,zero30;u32 requestedAddress;u8 q[8];u32 freeWord;u32 x44;u32 linkWord;u8 r[0x58-0x4c];void*buf;u32 size;u8 s[8];Value64 seed;u64 mult,add;}State;
extern State _021D4620;extern const char _version_UBIQUITOUS_CPS[];extern void empty_func(void);extern int default_link_is_on(void);extern u8 _021D4B08[],_021D5E60[],_021D5660[];extern OSThread _021D4888,_021D47C8;extern u32 _02110684;extern void tcpip(void*),scavenger(void*);void CPS_Startup(void*);
typedef struct { u64 x, mul, add; } Rand64;
static inline void InitRand64(Rand64 *r, u64 seed){r->x=seed;r->mul=0x5D588B656C078965ULL;r->add=0x269EC3ULL;}
static inline u32 Rand32(Rand64 *r, u32 max){r->x=r->mul*r->x+r->add;return (u32)(((r->x>>32)*max)>>32);}
typedef struct { u32 fixed; u32 alloc; u32 free; u32 recv; u32 link; union{struct{u32 seedLo; u32 seedHi;}; u64 seed64;}; void *buf; u32 size; u32 mtu; u32 addr; u32 sleep; } Cfg;
typedef char CfgOff[(offsetof(Cfg,seed64)==0x14)?1:-1];
// @fn CPS_Startup
void CPS_Startup(void *opaque){
 Cfg *c=opaque;
 OSi_ReferSymbol(_version_UBIQUITOUS_CPS);
 if(c->seed64)InitRand64((Rand64*)&_021D4620.seed,c->seed64);else InitRand64((Rand64*)&_021D4620.seed,OS_GetTick());
 if(c->alloc&&c->free){_021D4620.allocWord=c->alloc;_021D4620.freeWord=c->free;}else{_021D4620.allocWord=(u32)empty_func;_021D4620.freeWord=(u32)empty_func;}
 _021D4620.fixedFlag=c->fixed;
 if(c->mtu)_021D4620.mtu=c->mtu;else _021D4620.mtu=1460;
 _021D4620.requestedAddress=c->addr;
 _021D4620.sleepInterval=c->sleep;
 if(c->recv)_021D4620.receiveWord=c->recv;else _021D4620.receiveWord=(u32)empty_func;
 if(c->link)_021D4620.linkWord=c->link;else _021D4620.linkWord=(u32)default_link_is_on;
 _021D4620.buf=c->buf;_021D4620.size=c->size;
 _021D4620.zero30=0;_021D4620.zero28=0;
 _021D4620.port=(u16)(Rand32((Rand64*)&_021D4620.seed,3976)+1024);
 OS_GetMacAddress(_021D4B08);
 _021D4620.b=0;
 OS_CreateThread(&_021D4888,tcpip,0,_021D5E60,2048,_02110684);
 OS_CreateThread(&_021D47C8,scavenger,0,_021D5660,2048,_02110684);
 OS_WakeupThreadDirect(&_021D4888);OS_WakeupThreadDirect(&_021D47C8);
}
