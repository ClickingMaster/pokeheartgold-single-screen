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
void dispatch_ip(IP *ip,u32 len){int allocated;u32 source=Address(ip->source),dest=Address(ip->dest);if(dest!=source){if(ip_isme(dest)==0)return;if(len<Swap(ip->length))return;if(calc_checksum(ip,(ip->size&15)*4)!=0xffff)return;if(_021D4620.ip==Address(ip->dest))reg_arpcache((u8 *)ip-8,Address(ip->source),1);}ip=check_frag(ip,&allocated);if(ip==NULL)return;{u32 header=(ip->size&15)*4;void *payload=(u8 *)ip+header;u32 amount=Swap(ip->length)-header;if(ip->protocol==17)dispatch_udp(ip,payload,amount);else if(_021D4620.ip){if(ip->protocol==1)dispatch_icmp(ip,payload,amount);else if(ip->protocol==6)dispatch_tcp(ip,payload,amount);}}if(allocated)_021D4620.free((u8 *)ip-14);}
