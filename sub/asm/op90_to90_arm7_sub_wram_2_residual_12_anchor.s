	.include "asm/macros.inc"
	.include "global.inc"

	.extern CTRDGi_ChangeLatestAccessCycle
	.extern CTRDGi_EnableFlag
	.extern CTRDGi_LockByProcessor
	.extern CTRDGi_UnlockByProcessor
	.extern MI_SetCartridgeRomCycle1st
	.extern MI_SetCartridgeRomCycle2nd
	.extern OS_RestoreInterrupts

.text
