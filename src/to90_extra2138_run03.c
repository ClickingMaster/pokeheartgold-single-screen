#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u16 packets[4][256];u16 sequence[4];u16 writeIndex;u8 p[10];u16 packetSize;u8 q[6];u16 state;} Sharing;
typedef struct{u16 api,error,event,pad;u32 unused;const u16*data;u16 length;u8 p[8];u16 sequence;Sharing*sharing;} Callback;
u16 WM_GetAID(void);void WmDataSharingReceiveCallback_Child(Callback*);
void WmDataSharingReceiveCallback_Child(Callback*cb){Sharing*sharing=cb->sharing;u16 length,bitmap;const u16*data;u16 aid;if(!sharing)return;if(!cb->error){switch(cb->event){case 7:case 9:case 25:case 26:default:return;case 21:break;}data=cb->data;length=cb->length;bitmap=*data;aid=WM_GetAID();if(length!=sharing->packetSize){if(length>512)length=512;}if(length<4)return;if(!(bitmap&(1<<aid)))return;MIi_CpuCopy16(data,sharing->packets[sharing->writeIndex],length);sharing->sequence[sharing->writeIndex]=cb->sequence>>1;sharing->writeIndex=(sharing->writeIndex+1)&3;}else sharing->state=5;}
