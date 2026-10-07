	.include "asm/macros.inc"
	.include "global.inc"

	.extern InitApList
	.extern InitCAM
	.extern InitRxCtrl
	.extern InitTxCtrl
	.extern RND_Init
	.extern StartBeaconFrame
	.extern WDisableTmpttPowerSave
	.extern WSetPowerState
	.extern WSetStaState
	.extern WStop
	.extern WaitLoop_Rxpe
	.extern _ll_udiv

.text
