	.include "asm/macros.inc"
	.include "global.inc"

	.extern AllocateHeapBuf
	.extern DMA_Write
	.extern MP_ADRS
	.extern ReleaseHeapBuf
	.extern WSetMacAdrs3
	.extern WUpdateCounter
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER__u32_div_f

.text
