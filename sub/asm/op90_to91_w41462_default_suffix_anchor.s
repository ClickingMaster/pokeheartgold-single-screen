	.include "asm/macros.inc"
	.include "global.inc"

	.extern BBP_Write
	.extern FLASH_DirectRead
	.extern FLASH_Read
	.extern RF_Write
	.extern RND_Init
	.extern WSetActiveZoneTime
	.extern WSetAuthAlgo
	.extern WSetBcSsidResponse
	.extern WSetBeaconLostThreshold
	.extern WSetBeaconPeriod
	.extern WSetBeaconSendRecvIndicate
	.extern WSetBeaconType
	.extern WSetCCA_ED
	.extern WSetDiversity
	.extern WSetEnableChannel
	.extern WSetFrameLifeTime
	.extern WSetMacAdrs
	.extern WSetMainAntenna
	.extern WSetMode
	.extern WSetNullKeyMode
	.extern WSetPreambleType
	.extern WSetRate
	.extern WSetRateSet
	.extern WSetRetryLimit
	.extern WSetSsidMask
	.extern WSetWepKey
	.extern WSetWepKeyId
	.extern WSetWepMode
	.extern c_RateSet$4351
	.extern def_SsidMask
	.extern def_WepKey

.text
