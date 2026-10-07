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
	.include "asm/macros.inc"
	.include "global.inc"
	.public __VENEER__ll_udiv

.text
	arm_func_start __VENEER__ll_udiv
__VENEER__ll_udiv: ; 0x0600B1CC
	ldr pc, _0600B1D0 ; =_ll_udiv
	.align 2, 0
_0600B1D0: .word _ll_udiv
	arm_func_end __VENEER__ll_udiv
