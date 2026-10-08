	.include "asm/macros.inc"
	.include "global.inc"

	.extern CAM_GetStaState
	.extern CAM_SetPowerMgtMode
	.extern CAM_SetStaState
	.extern DeleteTxFrames
	.extern MLME_IssueDeAuthIndication
	.extern MakeDeAuthFrame
	.extern TxManCtrlFrame
	.extern WClearAids
	.extern WSetStaState

.text
