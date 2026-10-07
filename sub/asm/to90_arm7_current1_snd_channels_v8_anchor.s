	.include "asm/macros.inc"
	.include "global.inc"

	.extern CalcSurroundDecay
	.extern MI_StopDma
	.extern PXI_IsCallbackReady
	.extern SND_Enable
	.extern SND_SetMasterVolume
	.extern SND_SetOutputSelector
	.extern sMasterPan
	.extern sOrgPan
	.extern sOrgVolume
	.extern sSurroundDecay

	.public MI_StopDma
	.public PXI_IsCallbackReady
	.public SND_Enable
	.public SND_SetMasterVolume
	.public SND_SetOutputSelector


.text
