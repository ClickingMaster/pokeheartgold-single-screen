#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{u8 pad[0x30];volatile u32 head;u8 pad2[0x1c];u32 ip,unused;u8*buffer;u32 limit;}Work;
extern Work _021D4620;extern u8 _021D4B08[6],_02110688[6];
typedef struct{u32 ip;u8 mac[6];u16 time;}Cache;
extern Cache _021D46A0[8];extern u16 _021D46AA[];
extern int is_broadcast(u32),is_multicast(u32);extern void send_packet(void*,int,int,int);
void throw_packet(void);u8*inq_arpcache(u32);void send_arprequest(u32);
static inline u32 Swap16(int v){int swapped=v<<8;return (v>>8)|swapped;}
void throw_packet(void){OSIntrMode irq=OS_DisableInterrupts();_021D4620.head+=*(u16*)(_021D4620.buffer+_021D4620.head);if(_021D4620.head>=_021D4620.limit)_021D4620.head=0;OS_RestoreInterrupts(irq);}
