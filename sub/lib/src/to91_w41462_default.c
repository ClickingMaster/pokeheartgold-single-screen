#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define BASE (*(u8 **)0x0380fff4)
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define P(p,o) (*(u8 **)((u8*)(p)+(o)))
#define HW(a) (*(volatile u16 *)(a))
extern u8 def_WepKey[];extern u16 def_SsidMask[],c_RateSet$4351[];void FLASH_Read(u32,u32,void*);u32 WSetMacAdrs(const u16*);u32 WSetRetryLimit(u16);u32 WSetEnableChannel(u32);u32 WSetMode(u16);u32 WSetRate(u32);u32 WSetWepMode(u32);u32 WSetWepKeyId(u32);u32 WSetWepKey(const u8*);u32 WSetBeaconPeriod(u16);u32 WSetBeaconType(u32);u32 WSetBcSsidResponse(u32);u32 WSetBeaconLostThreshold(u32);u32 WSetActiveZoneTime(u16,u32);u32 WSetSsidMask(const u16*);u32 WSetPreambleType(u32);u32 WSetAuthAlgo(u32);int WSetRateSet(const u16*);u32 WSetCCA_ED(u32,u32);int WSetFrameLifeTime(u32);u32 WSetDiversity(u32,u32);u32 WSetMainAntenna(u32);u32 WSetBeaconSendRecvIndicate(u32);u32 WSetNullKeyMode(u32);void RND_Init(u32,u32);void WSetDefaultParameters(void);
void WSetDefaultParameters(void){union {struct {u16 channel;u16 mac[3];} fields;u32 initialized[2];} flash;u32 a,b,c;FLASH_Read(0x36,6,flash.fields.mac);FLASH_Read(0x3c,2,&flash.fields.channel);WSetMacAdrs(flash.fields.mac);WSetRetryLimit(7);WSetEnableChannel((u16)(flash.fields.channel&0x7ffe));WSetMode(2);WSetRate(0);WSetWepMode(0);WSetWepKeyId(0);WSetWepKey(def_WepKey);WSetBeaconPeriod(500);WSetBeaconType(0);WSetBcSsidResponse(0);WSetBeaconLostThreshold(16);WSetActiveZoneTime(65535,0);WSetSsidMask(def_SsidMask);WSetPreambleType(1);WSetAuthAlgo(0);WSetRateSet(c_RateSet$4351);WSetCCA_ED(0,31);WSetFrameLifeTime(5);WSetDiversity(0,0);WSetMainAntenna(0);WSetBeaconSendRecvIndicate(0);WSetNullKeyMode(0);a=HW(0x04808044);b=HW(0x04808044);c=HW(0x04808044);RND_Init(a+(b<<8),c);H(BASE,0x358)=1;}
