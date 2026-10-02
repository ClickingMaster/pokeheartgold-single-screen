#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
#include <string.h>
// clang-format on
#pragma force_active on
extern u8 Wifi[];
extern volatile struct NvStatus {u16 error;u16 pad;s32 ready;} _021D43C8;
extern u8 DWCi_SETTING_NONE[];
extern int NVRAMm_ExecuteCommand(s32,u32,u32,void*),DWC_BACKUPlCheckSsid(void*),DWC_BACKUPlCheckAddress(void*),DWC_BACKUPlCheckIp(void*,void*);
extern void DWCi_BACKUPlConvMaskAddr(u8,void*);
extern int DWCi_AUTH_GetNewWiFiInfo(void*);

typedef struct WiInfo {u32 primary[2],secondary[2];u16 counter,salt;} WiInfo;

int readNvram(u32 address,u32 size,void *out);
void writeNvram(u32 address,u32 size,void *data);
int verify(void *expected,u32 address,u32 size,void *scratch);
int writeDisable(void);
u16 Callback_NVRAM(PXIFifoTag tag,u32 word,BOOL error);
void *DWCi_BACKUPlConvWifiInfo(WiInfo *info);
void *DWCi_BACKUPlGetWifi(void);
int checkAp(u8 *page);
int init(u8 *buffer);
int checkAp(u8 *page){u32 mask;if(page[0xe7]==255)return 1;if(page[0xe7]>2)return 0;if(!DWC_BACKUPlCheckSsid(page+0x40))return 0;if(memcmp(page+0xc0,DWCi_SETTING_NONE,4)!=0){if(!DWC_BACKUPlCheckAddress(page+0xc4))return 0;if(page[0xd0]>32)return 0;DWCi_BACKUPlConvMaskAddr(page[0xd0],&mask);if(!DWC_BACKUPlCheckIp(page+0xc0,&mask))return 0;}if(memcmp(page+0xc8,DWCi_SETTING_NONE,4)!=0){if(!DWC_BACKUPlCheckAddress(page+0xc8)&&!DWC_BACKUPlCheckAddress(page+0xcc))return 0;}return 1;}
int init(u8 *buffer){WiInfo info;u8 *wifi;int i;MIi_CpuClear16(0,(u16*)buffer,0x400);for(i=0;i<3;i++)buffer[((u32)i<<8)+0xe7]=255;DWCi_AUTH_GetNewWiFiInfo(&info);wifi=DWCi_BACKUPlConvWifiInfo(&info);for(i=0;i<2;i++){MI_CpuCopy8(wifi,buffer+0xf0,14);buffer+=0x100;}return 0;}
