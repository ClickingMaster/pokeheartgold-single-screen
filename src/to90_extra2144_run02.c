#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u32 word;u8 *status;} Sys;
typedef struct {u16 word[2];volatile u16 count;u16 stride,reserved;u8 data[1];} Packet;
extern Sys *WMi_GetSystemWork(void);
extern int WMi_CheckInitialized(void);
void *WM_ReadMPData(Packet *,u16);
void *WM_ReadMPData(Packet *packet,u16 aid){Sys *sys=WMi_GetSystemWork();u8 *entry[15];int i;
 if(WMi_CheckInitialized())return NULL;
 if(aid<1 || aid>15)return NULL;
 DC_InvalidateRange(sys->status+0x182,2);
 if(!(*(u16*)(sys->status+0x182)&(1<<aid)))return NULL;
 if(packet->count==0)return NULL;
 entry[0]=packet->data;
 i=0;do{if(*(u16*)(entry[i]+4)==aid)return entry[i];i++;entry[i]=(u8*)(packet->stride+(u32)entry[i-1]);}while(i<packet->count);
 return NULL;
}
