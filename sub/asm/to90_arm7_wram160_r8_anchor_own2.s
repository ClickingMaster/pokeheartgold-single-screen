	.include "asm/macros.inc"
	.include "global.inc"
	.extern OS_LockMutex
	.extern OS_UnlockMutex
	.extern PXI_SendWordByFifo
	.extern VENEER_SVC_WaitByLoop
	.extern wmspW
	.public micIntrInfo
	.public CTRDGi_EnableFlag
	.public CTRDGi_Work
	.public lock_id
	.public current_vib
	.public isInitialized$3644
	.public isCartridgePullOut$3768
	.public skipCheck$3770
	.public isInitialized$3633
	.public ctw_sp
	.public pulse_edge_alarm
	.bss
micIntrInfo: ; 0x03809438
	.space 0x10
	.public CTRDGi_EnableFlag

CTRDGi_EnableFlag: ; 0x03809448
	.space 0x4
	.public CTRDGi_Work

CTRDGi_Work: ; 0x0380944C
	.space 0x4
	.public lock_id

lock_id: ; 0x03809450
	.space 0x2

	.balign 4, 0
current_vib: ; 0x03809454
	.space 0x4

isInitialized$3644: ; 0x03809458
	.space 0x4

isCartridgePullOut$3768: ; 0x0380945C
	.space 0x4

skipCheck$3770: ; 0x03809460
	.space 0x4

isInitialized$3633: ; 0x03809464
	.space 0x4

ctw_sp: ; 0x03809468
	.space 0x18
	.public pulse_edge_alarm

pulse_edge_alarm: ; 0x03809480
	.space 0x2C
	.public nvramw

