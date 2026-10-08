#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct WiInfo {
    u64 id;
    s64 password;
    u16 counter;
    u16 salt;
} WiInfo;
void DWCi_BM_GetWiFiInfo(WiInfo *info);
u64 DWCi_Util_WiFiId_scrambleUid(u32 salt, u32 mac, u8 flag, u32 zero);
RTCResult RTC_GetDate(RTCDate *date);
RTCResult RTC_GetTime(RTCTime *time);
BOOL DWCi_AUTH_GetNewWiFiInfo(WiInfo *info);
static inline u32 Mac24b(u8 a, u8 b, u8 c) { return (a << 16) | (b << 8) | c; }
#pragma opt_propagation off
// @fn DWCi_AUTH_GetNewWiFiInfo
BOOL DWCi_AUTH_GetNewWiFiInfo(WiInfo *info)
{
    u8 mac[6];
    RTCDate date;
    RTCTime time;
    s64 sec;
    u32 mul;
    u32 add;
    u8 flag;
    u32 macHigh;
    u32 macLow;
    u32 seed;
    DWCi_BM_GetWiFiInfo(info);
    RTC_Init();
    if (RTC_GetDate(&date) != RTC_RESULT_SUCCESS) {
        return FALSE;
    }
    if (RTC_GetTime(&time) != RTC_RESULT_SUCCESS) {
        return FALSE;
    }
    sec = RTC_ConvertDateTimeToSecond(&date, &time);
    if (sec < 0) {
        return FALSE;
    }
    if (OS_IsTickAvailable()) {
        sec += (u32)OS_GetTick;
    }
    OS_GetMacAddress(mac);
    mul = 0x5D588B65;
    add = 0x269EC3;
    macLow = Mac24b(mac[0], mac[1], mac[2]);
    flag = (macLow != 0x9BF) ? 1 : 0;
    macHigh = Mac24b(mac[3], mac[4], mac[5]);
    seed = (u32)sec * mul + add;
    info->counter = ((seed >> 16) * 1000) >> 16;
    info->id = 0;
    if (info->salt == 0) {
        info->password = 0;
        while (info->password == 0) {
            seed = seed * mul;
            seed += add;
            while (seed == 0) {
                seed = seed * mul;
                seed += add;
            }
            info->salt = (u16)seed;
            info->password = DWCi_Util_WiFiId_scrambleUid(info->salt, macHigh, flag, 0);
        }
    } else {
        info->password = 0;
        while (info->password == 0) {
            info->salt++;
            info->password = DWCi_Util_WiFiId_scrambleUid(info->salt, macHigh, flag, 0);
        }
    }
    return TRUE;
}
#pragma opt_propagation reset
