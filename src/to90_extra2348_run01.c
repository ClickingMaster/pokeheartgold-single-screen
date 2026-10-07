#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u8 pad[0x50];u32 ip;} Network;
typedef struct {u32 ip;u8 mac[6];u16 tick;} Cache;
extern Network _021D4620;extern Cache _021D46A0[8];extern u16 _021D46AA[];extern u8 _021D4B08[],_02110688[];
extern int is_broadcast(u32),is_multicast(u32);
u8 *inq_arpcache(u32);
u8 *inq_arpcache(u32 ip){OSIntrMode irq;u32 i;u8 *result;irq=OS_DisableInterrupts();result=0;if(ip==0x7f000001||ip==_021D4620.ip)result=_021D4B08;else if(is_broadcast(ip)||is_multicast(ip))result=_02110688;else{for(i=0;i<8;i++){if(_021D46A0[i].ip==ip){u32 offset;ip=(u32)(OS_GetTick()>>16);offset=i*12;*(u16*)((u8*)_021D46AA+offset)=ip;result=(u8*)_021D46A0+offset+4;break;}}}OS_RestoreInterrupts(irq);return result;}
