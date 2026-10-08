	.include "asm/macros.inc"
	.include "global.inc"

	.extern AddTask
	.extern AdjustRingPointer
	.extern MoveHeapBuf
	.extern ReleaseHeapBuf
	.extern RxBeaconFrame
	.extern RxKeyDataFrame
	.extern RxMpAckFrame
	.extern RxMpFrame
	.extern SetFatalErr
	.extern TakeoutRxFrame
	.extern WUpdateCounter

.text
