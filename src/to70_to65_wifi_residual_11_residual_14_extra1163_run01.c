#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 size,tos;u16 length,id,fragment;u8 ttl,protocol;u16 sum;u16 source[2],dest[2];} IP;
typedef struct {u16 source,dest,length,sum;} UDP;
typedef struct Endpoint Endpoint;
struct Endpoint {OSThread *thread;int state;u8 kind,pad;u16 port;u8 pad2[8];u32 local;u16 remotePort,pad3;u32 remote;u8 pad4[0x38-0x20];int (*callback)(void *,u32,Endpoint *);u32 capacity;void *buffer;u32 size;};
typedef struct {u8 pad[0x40];void (*free)(void *);u8 pad2[12];u32 ip;} Globals;
extern Globals _021D4620;
extern int check_tcpudpsum(UDP *,u32,IP *,int);extern int ip_isme(u32);extern u16 calc_checksum(void *,u32);extern void reg_arpcache(void *,u32,int);extern IP *check_frag(IP *,int *);extern void dispatch_icmp(IP *,void *,u32);extern void dispatch_tcp(IP *,void *,u32);
static inline u16 Swap(u16 x){int a=x;int high=a<<8;return (a>>8)|high;}
static inline u32 Address(u16 *p){return ((u32)Swap(p[0])<<16)|Swap(p[1]);}
void dispatch_udp(IP *,UDP *,u32);void dispatch_ip(IP *,u32);
void dispatch_udp(IP *ip,UDP *udp,u32 len){OSThread *t;Endpoint *e;OSIntrMode mode;if(udp->sum&&check_tcpudpsum(udp,len,ip,17)!=0)return;mode=OS_DisableInterrupts();for(t=OSi_ThreadInfo.list;t;t=t->next){e=t->specific[0];if(e&&e->thread){if(e->kind==10&&e->port==Swap(udp->dest)){if(e->remotePort&&e->remotePort!=Swap(udp->source))continue;if(e->remote&&e->remote!=~0U&&e->remote!=Address(ip->source))continue;e->local=Address(ip->dest);if(e->remote==0){e->remote=Address(ip->source);e->remotePort=Swap(udp->source);}if(e->size==0){if(len-8>e->capacity)e->size=e->capacity;else e->size=len-8;MI_CpuCopy8(udp+1,e->buffer,e->size);if(e->state==3){e->state=0;OS_WakeupThreadDirect(e->thread);}else if(e->callback&&e->callback(e->buffer,e->size,e)!=0)e->size=0;}break;}}}OS_RestoreInterrupts(mode);}
