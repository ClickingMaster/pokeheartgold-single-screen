#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
void CARDi_SetRomOp(u32,u32);int CARDi_ReadFromCache(CARDRomStat*);
void CARDi_ReadCard(CARDRomStat*card){CARDiCommon*common=&cardi_common;for(;;){u32 address=common->src&~511U;u32*destination;u32 status,index,bytes;if(address==common->src&&!((u32)common->dst&3)&&common->len>=512)destination=(u32*)common->dst;else{card->cache_page=(u8*)address;destination=(u32*)card->cache_buf;}CARDi_SetRomOp((address>>8)|0xb7000000,address<<24);index=0;bytes=index;*(vu32*)0x040001a4=card->ctrl;do{status=*(vu32*)0x040001a4;if(status&0x800000){u32 data=*(vu32*)0x04100010;if(bytes<512){destination[index]=data;bytes+=4;index++;}}}while(status&0x80000000);if((u32)destination==common->dst){cardi_common.src+=512;cardi_common.dst+=512;cardi_common.len-=512;if(!cardi_common.len)break;}else if(!CARDi_ReadFromCache(card))return;}}
