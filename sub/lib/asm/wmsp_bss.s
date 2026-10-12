	.include "asm/macros.inc"
	.include "global.inc"

	.public wmspIndicateThread
	.public wmspRequestThread

    .public WL_GetThreadStruct
    .public WL_InitDriver
    .public WMSP_IndicateThread
    .public WMSP_RequestThread
    .public WMSP_WL_ParamSetAll


.text

	.bss

	.public wmspW
wmspW: ; 0x060188A4
	.space 0x1590
