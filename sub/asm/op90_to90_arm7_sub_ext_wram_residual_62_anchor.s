	.include "asm/macros.inc"
	.include "global.inc"

	.extern AddTask
	.extern CAM_GetAuthSeed
	.extern CAM_GetStaState
	.extern CAM_SetAuthSeed
	.extern CAM_SetStaState
	.extern ClearTimeOut
	.extern ElementChecker
	.extern IsExistManFrame
	.extern MLME_IssueDeAuthIndication
	.extern MakeAuthFrame
	.extern MatchMacAdrs
	.extern NULL_ADRS
	.extern RND_rand
	.extern RND_seed
	.extern TxManCtrlFrame
	.extern WL_ReadByte
	.extern WL_WriteByte
	.extern WSetMacAdrs1
	.extern WSetStaState
	.extern __VENEER_MIi_CpuClear16
	.extern __VENEER_MIi_CpuClear32
	.extern __VENEER_MIi_CpuCopy16

.text
