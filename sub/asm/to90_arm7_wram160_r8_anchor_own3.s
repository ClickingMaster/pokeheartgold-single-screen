	.include "asm/macros.inc"
	.include "global.inc"
	.extern OS_LockMutex
	.extern OS_UnlockMutex
	.extern PXI_SendWordByFifo
	.extern VENEER_SVC_WaitByLoop
	.extern wmspW
	.public rtcInitialized
	.public rtcWork
	.public WVR_BSS_START
	.public wvrStatus
	.public wvrVramImageBuf
	.public wvrHeapHandle
	.public wvrVramImageBufSize
	.public wvrThread
	.public wvrWlStaElement
	.public wvrThreadStack
	.public wvrWlStack
	.public wvrWlWork
	.public wmspRequestThread
	.public wmspIndicateThread
	.public wmspMPAckAlarm
	.public wmspMPIntervalAlarm
	.public wmspVAlarm
	.bss
rtcInitialized: ; 0x038094CC
	.space 0x2

	.balign 4, 0
	.public rtcWork
rtcWork: ; 0x038094D0
	.space 0x3B4

	.public WVR_BSS_START
WVR_BSS_START:
	.public wvrStatus
wvrStatus: ; 0x03809884
	.space 0x1

	.balign 4, 0
wvrVramImageBuf: ; 0x03809888
	.space 0x4

wvrHeapHandle: ; 0x0380988C
	.space 0x4

wvrVramImageBufSize: ; 0x03809890
	.space 0x4
	.public wvrThread

wvrThread: ; 0x03809894
	.space 0xA4
	.public wvrWlStaElement

wvrWlStaElement: ; 0x03809938
	.space 0x1C0

wvrThreadStack: ; 0x03809AF8
	.space 0x400
	.public wvrWlStack

wvrWlStack: ; 0x03809EF8
	.space 0x600
	.public wvrWlWork

wvrWlWork: ; 0x0380A4F8
	.space 0x700

	.public wmspRequestThread
wmspRequestThread: ; 0x0380ABF8
	.space 0xA4

	.public wmspIndicateThread
wmspIndicateThread: ; 0x0380AC9C
	.space 0xA4

	.public wmspMPAckAlarm
wmspMPAckAlarm: ; 0x0380AD40
	.space 0x2C

	.public wmspMPIntervalAlarm
wmspMPIntervalAlarm: ; 0x0380AD6C
	.space 0x2C

	.public wmspVAlarm
wmspVAlarm: ; 0x0380AD98
	.space 0x28

; 0x0380ADC0
