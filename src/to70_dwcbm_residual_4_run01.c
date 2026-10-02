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
void writeNvram(u32 address,u32 size,void *data){while(!PXI_IsCallbackReady(PXI_FIFO_TAG_NVRAM,PXI_PROC_ARM7)){}PXI_SetFifoRecvCallback(PXI_FIFO_TAG_NVRAM,(PXIFifoCallback)Callback_NVRAM);DC_StoreRange(data,size);for(;;){if(NVRAMm_ExecuteCommand(2,address,size,data)==1)return;SVC_WaitByLoop(0x40000);}}
int verify(void *expected,u32 address,u32 size,void *scratch){if(readNvram(address,size,scratch)==0)return 0;return memcmp(expected,scratch,size)==0;}
int writeDisable(void){while(!PXI_IsCallbackReady(PXI_FIFO_TAG_NVRAM,PXI_PROC_ARM7)){}PXI_SetFifoRecvCallback(PXI_FIFO_TAG_NVRAM,(PXIFifoCallback)Callback_NVRAM);for(;;){if(NVRAMm_ExecuteCommand(7,0,0,0)==1)break;SVC_WaitByLoop(0x40000);}return 1;}
u16 Callback_NVRAM(PXIFifoTag tag,u32 word,BOOL error){_021D43C8.error=word&255;_021D43C8.ready=1;if(error)_021D43C8.error=255;return _021D43C8.error;}
