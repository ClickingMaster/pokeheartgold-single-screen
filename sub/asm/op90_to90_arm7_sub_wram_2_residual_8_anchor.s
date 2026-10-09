	.include "asm/macros.inc"
	.include "global.inc"

	.extern MIC_ExecSampling12
	.extern MIC_ExecSampling8
	.extern SPI_DummyWaitReceive
	.extern SPIi_CheckEntry
	.extern SPIi_CheckException
	.extern SPIi_ReturnResult
	.extern SPIi_SetEntry
	.extern micw

.text
