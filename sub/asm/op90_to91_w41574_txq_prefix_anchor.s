	.extern CopyTxFrmToMacBuf
	.include "asm/macros.inc"
	.include "global.inc"

	.extern CAM_DecFrameCount
	.extern CAM_GetFrameCount
	.extern CAM_GetStaState
	.extern CAM_IsActive
	.extern CheckFrameTimeout
	.extern DMA_WepWriteHeaderData
	.extern DMA_Write
	.extern DMA_WriteHeaderData
	.extern GetHeapBufNextAdrs
	.extern IssueMaDataConfirm
	.extern WSetPowerState
	.extern WUpdateCounter
	.extern __VENEER_OS_DisableIrqMask
	.extern __VENEER_OS_EnableIrqMask

.text
