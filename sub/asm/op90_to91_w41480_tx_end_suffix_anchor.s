	.include "asm/macros.inc"
	.include "global.inc"

	.extern AddTask
	.extern AdjustRingPointer
	.extern CheckKeyTxEnd
	.extern IntrCarrierSuppresionSignal
	.extern SetFatalErr
	.extern SetParentTbttTxq
	.extern WClearKSID
	.extern WUpdateCounter
	.extern WlIntrMpEnd
	.extern __VENEER_OS_CancelAlarm
	.extern __VENEER_OS_SetAlarm
	.extern __VENEER__ll_udiv

.text
