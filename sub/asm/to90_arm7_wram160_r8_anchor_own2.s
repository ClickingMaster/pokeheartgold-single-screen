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
	.bss
micIntrInfo: ; 0x03809438
	.space 0x10
	.public CTRDGi_EnableFlag

CTRDGi_EnableFlag: ; 0x03809448
	.space 0x4
	.public CTRDGi_Work

CTRDGi_Work: ; 0x0380944C
	.space 0x4
	.public nvramw

