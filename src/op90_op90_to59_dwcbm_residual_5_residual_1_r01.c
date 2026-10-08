#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 nibble[16];} NibbleTable;extern const NibbleTable DWCi_util_wifiid_ttable;extern const u8 DWCi_util_wifiid_exctable[];

extern u64 DWCi_Util_WiFiId_scrambleUid(u32,u32,u32,u32);

extern void *DWCi_BM_GetWiFiInfo(void);

extern void RTC_Init(void);

extern BOOL OS_IsTickAvailable(void);
extern RTCResult RTC_GetDate(RTCDate*),RTC_GetTime(RTCTime*);

typedef struct WifiInfo {u64 primary,secondary;
u16 counter,salt;
} WifiInfo;

extern int DWCi_AUTH_GetNewWiFiInfo(WifiInfo*);

// @fn DWCi_Util_WiFiId_scrambleUid
u64 DWCi_Util_WiFiId_scrambleUid(u32 a,u32 b,u32 c,u32 d){union Word {u64 value;struct {u32 low,high;} words;u8 bytes[8];};union Word current,snapshot;
u8 *p;int i;
current.value=((u64)d&3)|(((u64)c&1)<<2)|(((u64)b&0xffffff)<<3)|(((u64)a&0xffff)<<27);
p=current.bytes;
for(i=0;
i<6;
i++){*p^=0xd6;p++;};
p=current.bytes;
for(i=0;
i<5;
i++){u8 x=*p;
*p++=(DWCi_util_wifiid_ttable.nibble[(x>>4)&15]<<4)|DWCi_util_wifiid_ttable.nibble[x&15];
}MI_CpuCopy8(&current,&snapshot,8);
for(i=0;i<5;i++){current.bytes[DWCi_util_wifiid_exctable[i]]=snapshot.bytes[i];}
p=current.bytes;current.bytes[5]&=7;
current.bytes[7]=0;
current.bytes[6]=0;
current.value<<=1;
current.bytes[0]|=(current.bytes[5]>>3)&1;
for(i=0;
i<6;
i++){*p^=0x67;p++;};
(current.bytes)[7]=0;
(current.bytes)[6]=0;
(current.bytes)[5]&=7;
return current.value;
}
