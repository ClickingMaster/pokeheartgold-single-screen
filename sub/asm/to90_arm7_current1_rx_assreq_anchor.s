	.include "asm/macros.inc"
	.include "global.inc"

	.extern CAM_GetAID
	.extern CAM_GetStaState
	.extern CAM_SetCapaInfo
	.extern CAM_SetStaState
	.extern CAM_SetSupRate
	.extern ElementChecker
	.extern IsExistManFrame
	.extern MLME_IssueDisAssIndication
	.extern MakeAssResFrame
	.extern MakeDeAuthFrame
	.extern TxManCtrlFrame
	.extern __VENEER_MIi_CpuClear32


.text
