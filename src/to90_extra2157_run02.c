#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u8 pad[10];u16 sourcePort;u8 padc[12];u16 destPort;u16 pad1a;volatile u32 remote;u8 pad20[44];u8 *packet;} Socket;
typedef struct {u16 sourceHigh,sourceLow,destHigh,destLow,proto,length;} Pseudo;
typedef struct {u16 source,dest,length,checksum;} Udp;
extern volatile u32 _021D4620[];
extern u32 calc_checksum_do(const void*,int,u32);
extern u16 invert_checksum(u16);
extern void send_ip(void*,int,const void*,int,u32,int);
void send_udp(const void*,int,Socket*);
static inline u16 Swap(u16 x){return (x>>8)|(x<<8);}
#pragma opt_common_subs off
void send_udp(const void *data,int length,Socket *s){Udp *udp=(Udp*)(s->packet+0x22);u32 checksum;
 ((Pseudo*)((u8*)udp-12))->sourceHigh=Swap((u16)(_021D4620[0x50/4]>>16));((Pseudo*)((u8*)udp-12))->sourceLow=Swap((u16)_021D4620[0x50/4]);((Pseudo*)((u8*)udp-12))->destHigh=Swap((u16)(s->remote>>16));((Pseudo*)((u8*)udp-12))->destLow=Swap((u16)s->remote);((Pseudo*)((u8*)udp-12))->proto=0x1100;udp->length=Swap((u16)(length+8));((Pseudo*)((u8*)udp-12))->length=udp->length;udp->dest=Swap(s->destPort);udp->source=Swap(s->sourcePort);udp->checksum=0;checksum=calc_checksum_do((u8*)udp-12,20,0);checksum=calc_checksum_do(data,length,checksum);udp->checksum=Swap(invert_checksum((u16)checksum));send_ip(udp,8,data,length,s->remote,17);}
